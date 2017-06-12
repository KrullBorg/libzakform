LIBS = $(ZAKFORM_LIBS)

AM_CPPFLAGS = $(ZAKFORM_CFLAGS) \
              -DLOCALEDIR=\"$(localedir)\" \
              -DMODULESDIR=\""$(libdir)/$(PACKAGE)/modules"\" \
              -DG_LOG_DOMAIN=\"ZakForm\"

lib_LTLIBRARIES = libzakform.la

zakformmarshal.c: zakformmarshal.h zakform_marshal.list $(GLIB_GENMARSHAL)
	$(GLIB_GENMARSHAL) $(srcdir)/zakform_marshal.list --body --prefix=_zak_form_marshal > $(srcdir)/$@

zakformmarshal.h: zakform_marshal.list $(GLIB_GENMARSHAL)
	$(GLIB_GENMARSHAL) $(srcdir)/zakform_marshal.list --header --prefix=_zak_form_marshal > $(srcdir)/$@

libzakform_la_SOURCES = \
                        commons.c \
                        zakformmarshal.c \
                        form.c \
                        formelement.c \
                        formelementextension.c \
                        formelementfilter.c \
                        formelementfilterluc.c \
                        formelementfiltertrim.c \
                        formelementvalidator.c \
                        formelementvalidatordate.c \
                        formelementvalidatornotempty.c \
                        formelementvalidatorregex.c \
                        formiprovider.c \
                        formvalidator.c \
                        formvalidatorcompare.c \
                        formvalidatorcomparedate.c \
                        formvalidatorcomposite.c

libzakform_la_LDFLAGS = -no-undefined

gir_include = \
                             commons.h \
                             zakformmarshal.h \
                             form.h \
                             formelement.h \
                             formelementextension.h \
                             formelementfilter.h \
                             formelementfilterluc.h \
                             formelementfiltertrim.h \
                             formelementvalidator.h \
                             formelementvalidatordate.h \
                             formelementvalidatornotempty.h \
                             formelementvalidatorregex.h \
                             formiprovider.h \
                             formvalidator.h \
                             formvalidatorcompare.h \
                             formvalidatorcomparedate.h \
                             formvalidatorcomposite.h

libzakform_include_HEADERS = \
                             libzakform.h \
                             $(gir_include)

libzakform_includedir = $(includedir)/libzakform

install-exec-hook:
	mkdir -p "$(libdir)/$(PACKAGE)/modules"

uninstall-hook:
	rm -rf "$(libdir)/$(PACKAGE)"

CLEANFILES =

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir) --warn-all
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(libzakform_la_SOURCES) $(gir_include)

ZakForm-1.0.gir: libzakform.la
ZakForm_1_0_gir_INCLUDES = GLib-2.0 GObject-2.0 GModule-2.0 libxml2-2.0
ZakForm_1_0_gir_CFLAGS = $(AM_CPPFLAGS)
ZakForm_1_0_gir_LIBS = libzakform.la
ZakForm_1_0_gir_FILES = $(introspection_sources)
INTROSPECTION_GIRS += ZakForm-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif
