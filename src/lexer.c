#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "token.h"
#include "lexer.h"

// 🔥 VARIÁVEIS GLOBAIS
static const char* codigo;
static int pos;

// 🔧 Inicializa o lexer
void iniciar_lexer(const char* codigo_fonte) {
    codigo = codigo_fonte;
    pos = 0;
}

// 🚀 Função principal
Token proximo_token() {
    Token token;
    int i = 0;

    while (codigo[pos] != '\0') {

        // Ignorar espaços
        if (isspace(codigo[pos])) {
            pos++;
            continue;
        }

        i = 0; // reset índice

        // Identificadores ou palavras-chave
        if (isalpha(codigo[pos])) {
            while (isalnum(codigo[pos])) {
                token.valor[i++] = codigo[pos++];
            }
            token.valor[i] = '\0';

            if (strcmp(token.valor, "uai") == 0)
                token.tipo = TK_UAI;
            else if (strcmp(token.valor, "se") == 0)
                token.tipo = TK_SE;
            else if (strcmp(token.valor, "entao") == 0)
                token.tipo = TK_ENTAO;
            else if (strcmp(token.valor, "fim") == 0)
                token.tipo = TK_FIM;
            else if (strcmp(token.valor, "fala") == 0)
                token.tipo = TK_FALA;
            else
                token.tipo = TK_ID;

            return token;
        }

        // Números
        if (isdigit(codigo[pos])) {
            while (isdigit(codigo[pos])) {
                token.valor[i++] = codigo[pos++];
            }
            token.valor[i] = '\0';
            token.tipo = TK_NUM;
            return token;
        }

        // Strings
        if (codigo[pos] == '"') {
            pos++; // pula "

            while (codigo[pos] != '"' && codigo[pos] != '\0') {
                token.valor[i++] = codigo[pos++];
            }

            token.valor[i] = '\0';

            if (codigo[pos] == '"')
                pos++; // fecha "

            token.tipo = TK_STRING;
            return token;
        }

        // Operadores
        char c = codigo[pos++];
        token.valor[0] = c;
        token.valor[1] = '\0';

        switch (c) {
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
        }

        // Erro
        token.tipo = TK_ERRO;
        return token;
    }

    // Fim do código
    token.tipo = TK_EOF;
    strcpy(token.valor, "EOF");
    return token;
}