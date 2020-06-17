CC=gcc
AFLCC=afl-gcc
CFLAGS=-I.
DEPS=dictionary.h free_structs.h
OBJ=spell.o dictionary.o
CHECKFLAGS=$(shell pkg-config --cflags --libs check)
TESTDIR=tests

get-deps:
	sudo apt-get update && sudo apt-get install check valgrind

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

spell_check: $(OBJ) spell_check.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o spell_check test spell_check_fuzz

test: $(OBJ) $(TESTDIR)/*.c $(TESTDIR)/*.h
	$(CC) -o $@ $^ $(CFLAGS) $(CHECKFLAGS)

spell_check_fuzz: $(OBJ) spell_check.c
	$(AFLCC) -o $@ $^ $(CFLAGS)
