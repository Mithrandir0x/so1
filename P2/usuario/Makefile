#
# usuario/Makefile
#   Makefile de programas de usuario
#


MAKEFLAGS=-k
INCLUDEDIR=include
LIBDIR=lib

BIBLIOTECA=$(LIBDIR)/libserv.a

CC=cc
CFLAGS=-Wall -fPIC -Werror -g -I$(INCLUDEDIR)

PROGRAMAS=init excep_arit excep_mem simplon dormilon

all: biblioteca $(PROGRAMAS)

biblioteca:
	cd lib; make

init.o: $(INCLUDEDIR)/servicios.h
init: init.o $(BIBLIOTECA)
	$(CC) -shared -o $@ init.o -L$(LIBDIR) -lserv

excep_arit.o: $(INCLUDEDIR)/servicios.h
excep_arit: excep_arit.o $(BIBLIOTECA)
	$(CC) -shared -o $@ excep_arit.o -L$(LIBDIR)  -lserv 

excep_mem.o: $(INCLUDEDIR)/servicios.h
excep_mem: excep_mem.o $(BIBLIOTECA)
	$(CC) -shared -o $@ excep_mem.o -L$(LIBDIR) -lserv 

simplon.o: $(INCLUDEDIR)/servicios.h
simplon: simplon.o $(BIBLIOTECA)
	$(CC) -shared -o $@ simplon.o -L$(LIBDIR) -lserv 

dormilon.o: $(INCLUDEDIR)/servicios.h
dormilon: dormilon.o $(BIBLIOTECA)
	$(CC) -shared -o $@ dormilon.o -L$(LIBDIR) -lserv 

clean:
	rm -f *.o $(PROGRAMAS)
	cd lib; make clean

