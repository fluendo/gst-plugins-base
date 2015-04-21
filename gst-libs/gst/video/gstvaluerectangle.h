/*
    This file is provided under a dual BSD/LGPLv2.1 license.  When using
    or redistributing this file, you may do so under either license.

    LGPL LICENSE SUMMARY

    Copyright(c) 2009, 2010. Fluendo S.A. All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2.1 of the
    License.

    This library is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    USA. The full GNU Lesser General Public License is included in this
    distribution in the file called LICENSE.LGPL.

    Contat Information for Fluendo:
        FLUENDO S.A.
        World Trade Center Ed Norte 4 pl.
        Moll de Barcelona
        08039 BARCELONA - SPAIN

    BSD LICENSE

    Copyright(c) 2009, 2010. Fluendo S.A. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

      - Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      - Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in
        the documentation and/or other materials provided with the
        distribution.
      - Neither the name of FLUENDO S.A. nor the names of its
        contributors may be used to endorse or promote products derived
        from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __GST_VALUE_RECTANGLE_H__
#define __GST_VALUE_RECTANGLE_H__

/**
 * GST_VALUE_HOLDS_RECTANGLE:
 * @x: the #GValue to check
 *
 * Checks if the given #GValue contains a #GST_TYPE_RECTANGLE value.
 */
#define GST_VALUE_HOLDS_RECTANGLE(x)	(G_VALUE_HOLDS((x), gst_rectangle_get_type ()))

/**
 * GST_TYPE_RECTANGLE:
 *
 * a #GValue type that represents a rectangle of integer x, y coordinates
 * and integer width, height dimensions
 *
 * Returns: the #GType of GstRectangle (which is not explicitly typed)
 */

#define GST_TYPE_RECTANGLE                gst_rectangle_get_type ()

GType gst_rectangle_get_type (void);

/* rectangle */
void		gst_value_set_rectangle		(GValue		*value,
						 gint		x,
						 gint		y,
						 gint		width,
						 gint		height);
void            gst_value_get_rectangle         (const GValue   *value,
                                                 gint          *x,
                                                 gint          *y,
                                                 gint          *width,
                                                 gint          *height);

gint		gst_value_get_rectangle_x (const GValue	*value);
gint		gst_value_get_rectangle_y (const GValue	*value);
gint		gst_value_get_rectangle_width (const GValue	*value);
gint		gst_value_get_rectangle_height (const GValue	*value);

void    gst_value_rectangle_register (void);
G_END_DECLS

/* GParamSpec */

#define	GST_TYPE_PARAM_RECTANGLE	        (gst_param_spec_rectangle_get_type())
#define GST_IS_PARAM_SPEC_RECTANGLE(pspec)    (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), \
                                                 GST_TYPE_PARAM_RECTANGLE))
#define GST_PARAM_SPEC_RECTANGLE(pspec)       (G_TYPE_CHECK_INSTANCE_CAST ((pspec), \
                                                 GST_TYPE_PARAM_RECTANGLE, \
						 GstParamSpecRectangle))

typedef struct _GstParamSpecRectangle GstParamSpecRectangle;

/**
 * GstParamSpecRectangle:
 * @parent_instance: private %GParamSpec portion
 * 
 * A %GParamSpec derived structure that contains the meta data
 * for %GstRectangle properties.
 */
struct _GstParamSpecRectangle
{
  GParamSpec parent_instance;
};


GType gst_param_spec_rectangle_get_type (void);

GParamSpec* 	gst_param_spec_rectangle 	(const char *name, const char *nick,
    						 const char *blurb, GParamFlags flags) G_GNUC_MALLOC;

#endif /* __GST_VALUE_RECTANGLE_H__ */



