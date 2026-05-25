#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"
#include "token.h"

static Token token_atual;

static void avancar() {

    token_atual = proximo_token();

    while (token_atual.tipo == TK_COMENTARIO) {
        token_atual = proximo_token();
    }
}

static void erro(const char* mensagem) {

    printf("\nERRO SINTATICO\n");

    printf("%s\n", mensagem);

    printf(
        "linha %d coluna %d\n",
        token_atual.linha,
        token_atual.coluna
    );
}

static void consumir(TipoToken tipo) {

    if (token_atual.tipo == tipo) {

        avancar();

    } else {

        printf(
            "\nEsperado: %s\n",
            tipo_para_string(tipo)
        );

        printf(
            "Recebido: %s\n",
            tipo_para_string(token_atual.tipo)
        );

        erro("token inesperado");

        avancar();
    }
}

void iniciar_parser() {

    avancar();
}

static void expressao();

static void comando();

static void declaracao() {

    consumir(TK_UAI);

    consumir(TK_ID);
}

static void atribuicao() {

    consumir(TK_ID);

    consumir(TK_ATRIB);

    expressao();
}

static void fala() {

    consumir(TK_FALA);

    consumir(TK_STRING);
}

static void fator() {

    if (token_atual.tipo == TK_NUM) {

        consumir(TK_NUM);

    } else if (token_atual.tipo == TK_ID) {

        consumir(TK_ID);

    } else if (token_atual.tipo == TK_ABRE_PAR) {

        consumir(TK_ABRE_PAR);

        expressao();

        consumir(TK_FECHA_PAR);

    } else {

        erro("fator invalido");

        avancar();
    }
}

static void termo() {

    fator();

    while (
        token_atual.tipo == TK_MULT ||
        token_atual.tipo == TK_DIV
    ) {

        avancar();

        fator();
    }
}

static void expressao() {

    termo();

    while (
        token_atual.tipo == TK_SOMA ||
        token_atual.tipo == TK_SUB ||
        token_atual.tipo == TK_MAIOR ||
        token_atual.tipo == TK_MENOR
    ) {

        avancar();

        termo();
    }
}

static void condicional() {

    consumir(TK_SE);

    expressao();

    consumir(TK_ENTAO);

    while (
        token_atual.tipo != TK_FIM &&
        token_atual.tipo != TK_EOF
    ) {
        comando();
    }

    consumir(TK_FIM);
}

static void repeticao() {

    consumir(TK_ENQUANTO);

    expressao();

    consumir(TK_FACA);

    while (
        token_atual.tipo != TK_FIM &&
        token_atual.tipo != TK_EOF
    ) {
        comando();
    }

    consumir(TK_FIM);
}

static void comando() {

    switch (token_atual.tipo) {

        case TK_UAI:
            declaracao();
            break;

        case TK_ID:
            atribuicao();
            break;

        case TK_SE:
            condicional();
            break;

        case TK_ENQUANTO:
            repeticao();
            break;

        case TK_FALA:
            fala();
            break;

        default:
            erro("comando invalido");
            avancar();
    }
}

void programa() {

    while (token_atual.tipo != TK_EOF) {

        comando();
    }

    printf("\nSUCESSO SINTATICO!\n");
}