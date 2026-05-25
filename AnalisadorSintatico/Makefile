CC = gcc

CFLAGS = -Wall -Wextra -std=c99

OBJ = main.o lexer.o token.o parser.o ast.o

EXEC = mineirez

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

main.o: main.c lexer.h parser.h
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h token.h
	$(CC) $(CFLAGS) -c lexer.c

token.o: token.c token.h
	$(CC) $(CFLAGS) -c token.c

parser.o: parser.c parser.h lexer.h token.h
	$(CC) $(CFLAGS) -c parser.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

run: all
	./$(EXEC) teste.mz

clean:
	rm -f *.o $(EXEC)