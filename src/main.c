#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"

char* ler_arquivo(const char* nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");

    if (!f) {
        printf("Deu erro no arquivo sô:%s\n", nome_arquivo);
        
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    char *buffer = (char*) malloc(tamanho + 1);

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

    Token t;

    do {
        t = proximo_token();
        printf("<%s, %s>\n", tipo_para_string(t.tipo), t.valor);
    } while (t.tipo != TK_EOF);

    free(codigo);

    return 0;
}