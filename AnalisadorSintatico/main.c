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

static void imprimir_tabela_lexica(void) {

    printf("\nTABELA LEXICA\n");
    printf("%-15s %-20s %-8s %-8s %-20s\n", "TIPO", "LEXEMA", "LINHA", "COLUNA", "ERRO");
    printf("-------------------------------------------------------------------------------\n");

    Token token;

    do {

        token = proximo_token();

        printf(
            "%-15s %-20s %-8d %-8d %-20s\n",
            tipo_para_string(token.tipo),
            token.valor[0] != '\0' ? token.valor : "(vazio)",
            token.linha,
            token.coluna,
            token.tipo == TK_ERRO
                ? (token.erro[0] != '\0' ? token.erro : "erro_lexico")
                : "-"
        );

    } while (token.tipo != TK_EOF);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {

        printf("Uso: ./mineirez arquivo.mz\n");

        return 1;
    }

    char *codigo = ler_arquivo(argv[1]);

    iniciar_lexer(codigo);

    imprimir_tabela_lexica();

    iniciar_lexer(codigo);

    iniciar_parser();

    programa();

    free(codigo);

    return 0;
}