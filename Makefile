#Construction des listes de dépendances
CC=gcc
CFLAGS=-W -Wall -g
LDFLAGS=
EXEC=budget
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)


all: $(EXEC)

budget: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: interpreter.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
