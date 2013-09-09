PREFIX=/usr
LIBDIR=$(PREFIX)/lib/gawk
AWKDIR=$(PREFIX)/share/awk

CC=gcc
CFLAGS=-c -Wall -fPIC
HEADERS=
SOURCES=awkunit.c
OBJECTS=$(SOURCES:.c=.o)
LIBS=$(SOURCES:.c=.so)

AWKFILES=awkunit.awk

all: $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -shared $(SOURCES) -I${AWKSRCPATH}/ -I${AWKSRCPATH}/extension

%.so: %.o
	$(CC) -o $(LIBS) -shared $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(LIBS)

install: $(LIBS)
	cp $(LIBS) $(LIBDIR)
	cp $(AWKFILES) $(AWKDIR)

.PHONY: all clean install
