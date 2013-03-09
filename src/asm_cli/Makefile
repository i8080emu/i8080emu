CC=gcc
CFLAGS= -Wall -O2 -g
CURVERSION=1.1

DIST = Makefile main.c asm.c asm.h test.sh test

EXTRA_DIST = COPYING AUTHORS INSTALL README TODO

default : all

all: i8080asm

i8080asm: main.o asm.o
	$(CC) -o i8080asm main.o asm.o

asm.o: asm.c asm.h
main.o: main.c asm.h

asm.c:
	ln -s ../core/asm.c
asm.h:
	ln -s ../core/asm.h


clean:
	rm -f *~ *.o
	rm -f test/res/*
	rm -f test/pre.log/*
	rm -f test/asm.log/*

check: all
	./test.sh

dist: clean
	tar -czf i8080asm-$(CURVERSION).tgz $(DIST) $(EXTRA_DIST)
