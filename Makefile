CFLAGS=-O2 -Wall -W -fPIC
LDFLAGS=-shared -ldl
VERSION=0.0.1

all: fakeresolv.so

fakeresolv.so: fakeresolv.o
	$(LD) fakeresolv.o $(LDFLAGS) -o fakeresolv.so

install: all

clean:
	rm -f fakeresolv.o fakeresolv.so
