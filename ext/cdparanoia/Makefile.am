plugin_LTLIBRARIES = libgstcdparanoia.la

libgstcdparanoia_la_SOURCES = gstcdparanoiasrc.c
libgstcdparanoia_la_CFLAGS = \
	$(GST_PLUGINS_BASE_CFLAGS) \
	$(GST_BASE_CFLAGS) \
	$(GST_CFLAGS) \
	$(CDPARANOIA_CFLAGS)
libgstcdparanoia_la_LIBADD = \
	$(top_builddir)/gst-libs/gst/cdda/libgstcdda-$(GST_MAJORMINOR).la \
	$(GST_BASE_LIBS) \
	$(GST_LIBS) \
	$(CDPARANOIA_LIBS)
libgstcdparanoia_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
if !GST_PLUGIN_BUILD_STATIC
libgstcdparanoia_la_LIBTOOLFLAGS = --tag=disable-static
endif

noinst_HEADERS = gstcdparanoiasrc.h
