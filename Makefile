CC=gcc
CFLAGS=-I.
DEPS=dictionary.h
OBJ=spell.o dictionary.o

get-deps:
	sudo apt install check

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

spell_check: $(OBJ) spell_check.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o spell_check

test: $(OBJ) tests/check_load_dictionary.c
	$(CC) -o $@ $^ $(CFLAGS) -lcheck
