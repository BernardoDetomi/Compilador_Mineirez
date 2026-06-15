#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

static const char* codigo;

static int pos;
static int linha;
static int coluna;

static void avancar() {

    if (codigo[pos] == '\n') {
        linha++;
        coluna = 1;
    } else {
        coluna++;
    }

    pos++;
}

static Token criar_token(TipoToken tipo, int l, int c) {

    Token token;

    memset(&token, 0, sizeof(Token));

    token.tipo = tipo;
    token.linha = l;
    token.coluna = c;

    return token;
}

void iniciar_lexer(const char* codigo_fonte) {

    codigo = codigo_fonte;

    pos = 0;
    linha = 1;
    coluna = 1;
}

Token proximo_token() {

    while (codigo[pos] != '\0') {

        int linha_inicio = linha;
        int coluna_inicio = coluna;

        char atual = codigo[pos];

        if (isspace(atual)) {
            avancar();
            continue;
        }

        if (isalpha(atual) || atual == '_') {

            int inicio = pos;

            while (isalnum(codigo[pos]) || codigo[pos] == '_') {
                avancar();
            }

            Token token = criar_token(TK_ID, linha_inicio, coluna_inicio);

            strncpy(token.valor, codigo + inicio, pos - inicio);

            token.valor[pos - inicio] = '\0';

            if (strcmp(token.valor, "uai") == 0)
                token.tipo = TK_UAI;

            else if (strcmp(token.valor, "int") == 0)
                token.tipo = TK_INT;

            else if (strcmp(token.valor, "float") == 0)
                token.tipo = TK_FLOAT;

            else if (strcmp(token.valor, "se") == 0)
                token.tipo = TK_SE;

            else if (strcmp(token.valor, "entao") == 0)
                token.tipo = TK_ENTAO;

            else if (strcmp(token.valor, "fim") == 0)
                token.tipo = TK_FIM;

            else if (strcmp(token.valor, "fala") == 0)
                token.tipo = TK_FALA;

            else if (strcmp(token.valor, "enquanto") == 0)
                token.tipo = TK_ENQUANTO;

            else if (strcmp(token.valor, "faca") == 0)
                token.tipo = TK_FACA;

            return token;
        }

        if (isdigit(atual)) {

            int inicio = pos;

            while (isdigit(codigo[pos])) {
                avancar();
            }

            if (codigo[pos] == '.' && isdigit(codigo[pos + 1])) {
                avancar();

                while (isdigit(codigo[pos])) {
                    avancar();
                }
            }

            Token token = criar_token(TK_NUM, linha_inicio, coluna_inicio);

            strncpy(token.valor, codigo + inicio, pos - inicio);

            token.valor[pos - inicio] = '\0';

            return token;
        }

        if (codigo[pos] == '"') {

            avancar();

            int inicio = pos;

            while (
                codigo[pos] != '"' &&
                codigo[pos] != '\0'
            ) {
                avancar();
            }

            Token token = criar_token(TK_STRING, linha_inicio, coluna_inicio);

            strncpy(token.valor, codigo + inicio, pos - inicio);

            token.valor[pos - inicio] = '\0';

            if (codigo[pos] == '"') {
                avancar();
            } else {
                token.tipo = TK_ERRO;
                strcpy(token.erro, "string_nao_fechada");
            }

            return token;
        }

        avancar();

        Token token = criar_token(TK_ERRO, linha_inicio, coluna_inicio);

        token.valor[0] = atual;
        token.valor[1] = '\0';

        switch (atual) {

            case '=':
                token.tipo = TK_ATRIB;
                return token;

            case '>':
                token.tipo = TK_MAIOR;
                return token;

            case '<':
                token.tipo = TK_MENOR;
                return token;

            case '+':
                token.tipo = TK_SOMA;
                return token;

            case '-':
                token.tipo = TK_SUB;
                return token;

            case '*':
                token.tipo = TK_MULT;
                return token;

            case '/':
                token.tipo = TK_DIV;
                return token;

            case '(':
                token.tipo = TK_ABRE_PAR;
                return token;

            case ')':
                token.tipo = TK_FECHA_PAR;
                return token;
        }

        strcpy(token.erro, "simbolo_invalido");

        return token;
    }

    Token token = criar_token(TK_EOF, linha, coluna);

    strcpy(token.valor, "EOF");

    return token;
}