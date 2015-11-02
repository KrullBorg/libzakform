LIBS = $(ZAKFORM_LIBS)

AM_CPPFLAGS = $(ZAKFORM_CFLAGS) \
              -DG_LOG_DOMAIN=\"ZakForm\"

lib_LTLIBRARIES = libzakform.la

libzakform_la_SOURCES = \
                        form.c \
                        formelement.c \
                        formelementfilter.c \
                        formelementfiltertrim.c \
                        formelementvalidator.c \
                        formelementvalidatornotempty.c \
                        formelementvalidatorregex.c

libzakform_la_LDFLAGS = -no-undefined

libzakform_include_HEADERS = \
                             libzakform.h \
                             form.h \
                             formelement.h \
                             formelementfilter.h \
                             formelementfiltertrim.h \
                             formelementvalidator.h \
                             formelementvalidatornotempty.h \
                             formelementvalidatorregex.h

libzakform_includedir = $(includedir)/libzakform
