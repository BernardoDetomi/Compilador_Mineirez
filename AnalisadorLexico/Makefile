CC = gcc
CFLAGS = -Iinclude

SRC = src/main.c src/lexer.c src/token.c

all:
	$(CC) $(SRC) -o mineirez $(CFLAGS)

clean:
	rm -f mineirez

run:
	./mineirez