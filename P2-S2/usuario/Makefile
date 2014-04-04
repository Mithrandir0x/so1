#
# usuario/Makefile
#   Makefile de programas de usuario
#

MAKEFLAGS  = -k
INCLUDEDIR = include
LIBDIR     = lib

BIBLIOTECA = $(LIBDIR)/libserv.a
HEADERS    = $(INCLUDEDIR)/servicios.h

CC         = cc
CFLAGS     = -Wall -fPIC -Werror -g -I$(INCLUDEDIR)

SOURCES    = $(wildcard *.c)
OBJECTS    = $(patsubst %.c,%.o,$(SOURCES))
EXE        = $(patsubst %.c,%,$(SOURCES))

all: biblioteca $(EXE)

biblioteca:
	cd lib; make

$(OBJECTS): $(HEADERS)
$(EXE): $(OBJECTS) $(BIBLIOTECA)
	$(CC) -shared -o $@ $@.o -L$(LIBDIR) -lserv

clean:
	rm -f *.o $(EXE)
	cd lib; make clean
