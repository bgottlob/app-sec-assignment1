CC=gcc
CFLAGS=-I.
DEPS=dictionary.h
OBJ=spell.o dictionary.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

spell_check: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o spell_check
