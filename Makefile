CC=gcc
CFLAGS=-I.
DEPS=dictionary.h
OBJ=spell.o dictionary.o
CHECKFLAGS=$(shell pkg-config --cflags --libs check)
TESTDIR=tests

get-deps:
	sudo apt-get update && sudo apt-get install check

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

spell_check: $(OBJ) spell_check.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o spell_check

test: $(OBJ) $(TESTDIR)/*
	$(CC) -o $@ $^ $(CFLAGS) $(CHECKFLAGS)
