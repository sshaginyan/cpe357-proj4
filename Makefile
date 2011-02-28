#  CPE 357
#  -------------------
#  Simple makefile for Project 3
#
#  Last Modified: Sun Feb 6 10:24:13 PST 2011
#  @author Luis Castillo, Ricardo Tijero
CC=gcc
CFLAGS=-Wall -g -lm -ansi -pedantic -D_XOPEN_SOURCE=500 -D_FORTIFY_SOURCE -m32
LDFLAGS= -m32 -lm

ALL=mytar

all:	$(ALL)

mytar:	mytar.o tarFunctions.o
	$(CC) $(LDFLAGS) -o $@ $^

mytar.o:	mytar.c tarFunctions.h
	$(CC) $(CFLAGS) -c $<

tarFunctions.o: tarFunctions.c tarFunctions.h
	$(CC) $(CFLAGS) -c $<

.PHONY:	Doxyfile docs

docs:
	doxygen Doxyfile

Doxyfile:
	doxygen -g
	@echo =======================================================
	@echo You MUST modify this Doxyfile to get the right results.
	@echo =======================================================

clean:
	rm -f -rf core* *.o *.s *.gch *~ $(ALL)
