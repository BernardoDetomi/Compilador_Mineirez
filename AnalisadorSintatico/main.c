#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

char* ler_arquivo(const char* nome_arquivo) {

    FILE *f = fopen(nome_arquivo, "r");

    if (!f) {

        printf("Erro ao abrir arquivo\n");

        exit(1);
    }

    fseek(f, 0, SEEK_END);

    long tamanho = ftell(f);

    rewind(f);

    char *buffer = malloc(tamanho + 1);

    fread(buffer, 1, tamanho, f);

    buffer[tamanho] = '\0';

    fclose(f);

    return buffer;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {

        printf("Uso: ./mineirez arquivo.mz\n");

        return 1;
    }

    char *codigo = ler_arquivo(argv[1]);

    iniciar_lexer(codigo);

    iniciar_parser();

    programa();

    free(codigo);

    return 0;
}