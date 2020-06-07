CC=gcc
CFLAGS=-I.
DEPS=dictionary.h
OBJ=spell.o dictionary.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

spell_check: $(OBJ) spell_check.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o spell_check
