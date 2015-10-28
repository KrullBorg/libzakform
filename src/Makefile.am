LIBS = $(ZAKFORM_LIBS)

AM_CPPFLAGS = $(ZAKFORM_CFLAGS) \
              -DG_LOG_DOMAIN=\"ZakForm\"

lib_LTLIBRARIES = libzakform.la

libzakform_la_SOURCES = \
                        form.c \
                        formelementifilter.c \
                        formelementfiltertrim.c \
                        formelementivalidator.c \
                        formelementvalidatornotempty.c \
                        formelementvalidatorregex.c

libzakform_la_LDFLAGS = -no-undefined

libzakform_include_HEADERS = \
                             libzakform.h \
                             form.h \
                             formelementifilter.h \
                             formelementfiltertrim.h \
                             formelementivalidator.h \
                             formelementvalidatornotempty.h \
                             formelementvalidatorregex.h

libzakform_includedir = $(includedir)/libzakform
