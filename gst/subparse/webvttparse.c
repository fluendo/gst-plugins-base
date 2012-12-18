/* GStreamer QTtext subtitle parser
 * Copyright (c) 2012 Fluendo S.A <support@fluendo.com>
 *  Authors: Andoni Morales Alastruey <amorales@fluendo.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "webvttparse.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN_TO_NSEC  (60 * GST_SECOND)
#define HOUR_TO_NSEC (60 * MIN_TO_NSEC)

#define GST_WEBVTT_CONTEXT(state) ((GstWebVTTContext *) (state)->user_data)

typedef struct _GstWebVTTContext GstWebVTTContext;

struct _GstWebVTTContext
{
  /* timing variables */
  guint64 start_time;
  guint64 stop_time;

  gboolean markup_open;
  gboolean need_markup;
  gboolean info_parsed;

  gchar *font;
  gint font_size;
  gchar *bg_color;
  gchar *fg_color;

  gboolean bold;
  gboolean italic;
};

void
webvtt_context_init (ParserState * state)
{
  GstWebVTTContext *context;

  state->user_data = g_new0 (GstWebVTTContext, 1);

  context = GST_WEBVTT_CONTEXT (state);

  context->markup_open = FALSE;
  context->need_markup = FALSE;
  context->info_parsed = FALSE;

  context->font_size = 12;
}

void
webvtt_context_deinit (ParserState * state)
{
  if (state->user_data != NULL) {
    GstWebVTTContext *context = GST_WEBVTT_CONTEXT (state);
    g_free (context->font);
    g_free (context->bg_color);
    g_free (context->fg_color);

    g_free (state->user_data);
    state->user_data = NULL;
  }
}

static guint64
webvtt_parse_timestamp (ParserState * state, const gchar * line)
{
  int ret;
  gint hour, min, sec, dec;

  GST_LOG ("Scanning line %s for timestamp", line);
  ret = sscanf (line, "%d:%d:%d.%d", &hour, &min, &sec, &dec);
  if (ret != 3 && ret != 4) {
    /* bad timestamp */
    GST_WARNING ("Bad qttext timestamp found: %s", line);
    return 0;
  }

  if (ret == 3) {
    /* be forgiving for missing decimal part */
    dec = 0;
  }

  /* return the result */
  return hour * HOUR_TO_NSEC + min * MIN_TO_NSEC + sec * GST_SECOND + dec;
}

static gboolean
webvtt_parse_info (ParserState * state, const gchar * line)
{
  gchar *p;
  GstWebVTTContext *context = GST_WEBVTT_CONTEXT (state);

  p = g_strrstr (line, "-->");
  if (p == NULL)
    return FALSE;

  context->start_time = webvtt_parse_timestamp (state, line);
  GST_LOG ("Found start time: %" GST_TIME_FORMAT,
      GST_TIME_ARGS (context->start_time));
  context->stop_time = webvtt_parse_timestamp (state, p + 3);
  GST_LOG ("Found stop time: %" GST_TIME_FORMAT,
      GST_TIME_ARGS (context->stop_time));

  /* FIXME: parse size and alignment */

  context->info_parsed = TRUE;
  return TRUE;
}

static void
webvtt_open_markup (ParserState * state)
{
  GstWebVTTContext *context = GST_WEBVTT_CONTEXT (state);

  g_string_append (state->buf, "<span");

  /* add your markup tags here */
  if (context->font)
    g_string_append_printf (state->buf, " font='%s %d'", context->font,
        context->font_size);
  else
    g_string_append_printf (state->buf, " font='%d'", context->font_size);

  if (context->bg_color)
    g_string_append_printf (state->buf, " bgcolor='%s'", context->bg_color);
  if (context->fg_color)
    g_string_append_printf (state->buf, " color='%s'", context->fg_color);

  if (context->bold)
    g_string_append (state->buf, " weight='bold'");
  if (context->italic)
    g_string_append (state->buf, " style='italic'");

  g_string_append (state->buf, ">");
}

static void
webvtt_prepare_text (ParserState * state)
{
  GstWebVTTContext *context = GST_WEBVTT_CONTEXT (state);

  if (state->buf == NULL) {
    state->buf = g_string_sized_new (256);      /* this should be enough */
  } else {
    g_string_append (state->buf, "\n");
  }

  /* if needed, add pango markup */
  if (context->need_markup) {
    if (context->markup_open) {
      g_string_append (state->buf, "</span>");
    }
    webvtt_open_markup (state);
    context->markup_open = TRUE;
  }
}

static void
webvtt_parse_text (ParserState * state, const gchar * line)
{
  GstWebVTTContext *context = GST_WEBVTT_CONTEXT (state);

  if (!context->info_parsed)
    return;

  webvtt_prepare_text (state);
  g_string_append (state->buf, line);
}

static gchar *
webvtt_get_text (ParserState * state)
{
  gchar *ret;
  GstWebVTTContext *context = GST_WEBVTT_CONTEXT (state);

  if (state->buf == NULL)
    return NULL;

  if (context->markup_open) {
    g_string_append (state->buf, "</span>");
  }
  ret = g_string_free (state->buf, FALSE);
  state->buf = NULL;
  context->markup_open = FALSE;
  return ret;
}

gchar *
parse_webvtt (ParserState * state, const gchar * line)
{
  gchar *ret = NULL;
  GstWebVTTContext *context = GST_WEBVTT_CONTEXT (state);

  if (g_str_has_prefix (line, "WEBVTT"))
    return ret;

  if (g_str_has_prefix (line, "X-TIMESTAMP-MAP"))
    return ret;

  if (g_strrstr (line, "-->") != NULL) {
    /* check if we have pending text to send, in case we prepare it */
    if (state->buf) {
      ret = webvtt_get_text (state);
      state->start_time = context->start_time;

    }
    state->buf = NULL;

    webvtt_parse_info (state, line);
  } else if (line[0] == '\0') {
    /* check if we have pending text to send, in case we prepare it */
    if (state->buf) {
      ret = webvtt_get_text (state);
      state->start_time = context->start_time;
    }
    state->buf = NULL;
  } else {
    webvtt_parse_text (state, line);
  }

  if (ret)
    GST_DEBUG ("Pushing buffer with ts:%" GST_TIME_FORMAT,
        GST_TIME_ARGS (context->start_time));
  return ret;
}
