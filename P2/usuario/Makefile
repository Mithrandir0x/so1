#
# usuario/Makefile
#   Makefile de programas de usuario
#


MAKEFLAGS  = -k
INCLUDEDIR =include
LIBDIR     = lib

BIBLIOTECA = $(LIBDIR)/libserv.a
HEADERS    = $(INCLUDEDIR)/servicios.h

CC         = cc
CFLAGS     = -Wall -fPIC -Werror -g -I$(INCLUDEDIR)

PROGRAMAS  = init excep_arit excep_mem simplon dormilon simplon_prio getc longsum color

all: biblioteca $(PROGRAMAS)

biblioteca:
	cd lib; make

init.o: $(HEADERS)
init: init.o $(BIBLIOTECA)
	$(CC) -shared -o $@ init.o -L$(LIBDIR) -lserv

excep_arit.o: $(HEADERS)
excep_arit: excep_arit.o $(BIBLIOTECA)
	$(CC) -shared -o $@ excep_arit.o -L$(LIBDIR)  -lserv 

excep_mem.o: $(HEADERS)
excep_mem: excep_mem.o $(BIBLIOTECA)
	$(CC) -shared -o $@ excep_mem.o -L$(LIBDIR) -lserv 

simplon.o: $(HEADERS)
simplon: simplon.o $(BIBLIOTECA)
	$(CC) -shared -o $@ simplon.o -L$(LIBDIR) -lserv 

dormilon.o: $(HEADERS)
dormilon: dormilon.o $(BIBLIOTECA)
	$(CC) -shared -o $@ dormilon.o -L$(LIBDIR) -lserv 

simplon_prio.o: $(HEADERS)
simplon_prio: simplon_prio.o $(BIBLIOTECA)
	$(CC) -shared -o $@ simplon_prio.o -L$(LIBDIR) -lserv

getc.o: $(HEADERS)
getc: getc.o $(BIBLIOTECA)
	$(CC) -shared -o $@ getc.o -L$(LIBDIR) -lserv 

longsum.o: $(HEADERS)
longsum: longsum.o $(BIBLIOTECA)
	$(CC) -shared -o $@ longsum.o -L$(LIBDIR) -lserv 

color.o: $(HEADERS)
color: color.o $(BIBLIOTECA)
	$(CC) -shared -o $@ color.o -L$(LIBDIR) -lserv 

clean:
	rm -f *.o $(PROGRAMAS)
	cd lib; make clean

