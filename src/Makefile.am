LIBS = $(ZAKFORM_LIBS)

AM_CPPFLAGS = $(ZAKFORM_CFLAGS) \
              -DLOCALEDIR=\"$(localedir)\" \
              -DMODULESDIR=\""$(libdir)/$(PACKAGE)/modules"\" \
              -DG_LOG_DOMAIN=\"ZakForm\"

lib_LTLIBRARIES = libzakform.la

libzakform_la_SOURCES = \
                        form.c \
                        formelement.c \
                        formelementfilter.c \
                        formelementfilterluc.c \
                        formelementfiltertrim.c \
                        formelementvalidator.c \
                        formelementvalidatordate.c \
                        formelementvalidatornotempty.c \
                        formelementvalidatorregex.c \
                        formiprovider.c \
                        formvalidator.c

libzakform_la_LDFLAGS = -no-undefined

libzakform_include_HEADERS = \
                             libzakform.h \
                             form.h \
                             formelement.h \
                             formelementfilter.h \
                             formelementfilterluc.h \
                             formelementfiltertrim.h \
                             formelementvalidator.h \
                             formelementvalidatordate.h \
                             formelementvalidatornotempty.h \
                             formelementvalidatorregex.h \
                             formiprovider.h \
                             formvalidator.h

libzakform_includedir = $(includedir)/libzakform

install-exec-hook:
	mkdir -p "$(libdir)/$(PACKAGE)/modules"

uninstall-hook:
	rm -rf "$(libdir)/$(PACKAGE)"
