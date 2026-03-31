#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "token.h"
#include "lexer.h"

static const char* codigo;
static int pos;
static int linha;
static int coluna;

static void avancar(void) {
    if (codigo[pos] == '\n') {
        linha++;
        coluna = 1;
    } else {
        coluna++;
    }

    pos++;
}

static Token criar_token_base(TipoToken tipo, int linha_token, int coluna_token) {
    Token token;
    memset(&token, 0, sizeof(token));
    token.tipo = tipo;
    token.linha = linha_token;
    token.coluna = coluna_token;
    return token;
}

static void copiar_lexema(Token* token, const char* inicio, int tamanho) {
    int limite = tamanho < 99 ? tamanho : 99;
    int j;

    for (j = 0; j < limite; j++) {
        char c = inicio[j];

        if (c == '\n' || c == '\r' || c == '\t') {
            token->valor[j] = ' ';
        } else {
            token->valor[j] = c;
        }
    }

    token->valor[limite] = '\0';
}

static void copiar_comentario(Token* token, const char* inicio, int tamanho) {
    int inicio_texto = 0;
    int fim_texto = tamanho;

    while (inicio_texto < fim_texto && isspace((unsigned char) inicio[inicio_texto])) {
        inicio_texto++;
    }

    while (fim_texto > inicio_texto && isspace((unsigned char) inicio[fim_texto - 1])) {
        fim_texto--;
    }

    copiar_lexema(token, inicio + inicio_texto, fim_texto - inicio_texto);
}

static void definir_erro(Token* token, const char* mensagem) {
    strncpy(token->erro, mensagem, sizeof(token->erro) - 1);
    token->erro[sizeof(token->erro) - 1] = '\0';
}

void iniciar_lexer(const char* codigo_fonte) {
    codigo = codigo_fonte;
    pos = 0;
    linha = 1;
    coluna = 1;
}

Token proximo_token() {
    while (codigo[pos] != '\0') {
        unsigned char atual = (unsigned char) codigo[pos];
        int linha_inicio = linha;
        int coluna_inicio = coluna;

        if (isspace(atual)) {
            avancar();
            continue;
        }

        if (codigo[pos] == '#') {
            int inicio;
            Token token;

            avancar();
            inicio = pos;

            while (codigo[pos] != '\0' && codigo[pos] != '\n' && codigo[pos] != '\r') {
                avancar();
            }

            token = criar_token_base(TK_COMENTARIO, linha_inicio, coluna_inicio);
            copiar_comentario(&token, codigo + inicio, pos - inicio);
            return token;
        }

        if (codigo[pos] == '/' && codigo[pos + 1] == '/') {
            int inicio;
            Token token;

            avancar();
            avancar();
            inicio = pos;

            while (codigo[pos] != '\0' && codigo[pos] != '\n' && codigo[pos] != '\r') {
                avancar();
            }

            token = criar_token_base(TK_COMENTARIO, linha_inicio, coluna_inicio);
            copiar_comentario(&token, codigo + inicio, pos - inicio);
            return token;
        }

        if (codigo[pos] == '/' && codigo[pos + 1] == '*') {
            int inicio;
            Token token;

            avancar();
            avancar();
            inicio = pos;

            while (codigo[pos] != '\0') {
                if (codigo[pos] == '*' && codigo[pos + 1] == '/') {
                    token = criar_token_base(TK_COMENTARIO, linha_inicio, coluna_inicio);
                    copiar_comentario(&token, codigo + inicio, pos - inicio);
                    avancar();
                    avancar();
                    return token;
                }

                avancar();
            }

            token = criar_token_base(TK_ERRO, linha_inicio, coluna_inicio);
            copiar_lexema(&token, codigo + inicio, pos - inicio);
            definir_erro(&token, "comentario_nao_fechado");
            return token;
        }

        if (isalpha(atual) || codigo[pos] == '_') {
            int inicio = pos;
            Token token;

            while (isalnum((unsigned char) codigo[pos]) || codigo[pos] == '_') {
                avancar();
            }

            token = criar_token_base(TK_ID, linha_inicio, coluna_inicio);
            copiar_lexema(&token, codigo + inicio, pos - inicio);

            if (strcmp(token.valor, "uai") == 0) {
                token.tipo = TK_UAI;
            } else if (strcmp(token.valor, "se") == 0) {
                token.tipo = TK_SE;
            } else if (strcmp(token.valor, "entao") == 0) {
                token.tipo = TK_ENTAO;
            } else if (strcmp(token.valor, "fim") == 0) {
                token.tipo = TK_FIM;
            } else if (strcmp(token.valor, "fala") == 0) {
                token.tipo = TK_FALA;
            }

            return token;
        }

        if (isdigit(atual)) {
            int inicio = pos;
            Token token;

            while (isdigit((unsigned char) codigo[pos])) {
                avancar();
            }

            token = criar_token_base(TK_NUM, linha_inicio, coluna_inicio);
            copiar_lexema(&token, codigo + inicio, pos - inicio);

            if (isalpha((unsigned char) codigo[pos]) || codigo[pos] == '_') {
                while (isalnum((unsigned char) codigo[pos]) || codigo[pos] == '_') {
                    avancar();
                }

                token.tipo = TK_ERRO;
                definir_erro(&token, "numero_seguido_de_letra_ou_underscore");
                return token;
            }

            return token;
        }

        if (codigo[pos] == '"') {
            int inicio;
            int fechado;
            Token token;

            avancar();
            inicio = pos;
            fechado = 0;

            while (codigo[pos] != '\0') {
                if (codigo[pos] == '\n' || codigo[pos] == '\r') {
                    break;
                }

                if (codigo[pos] == '"') {
                    fechado = 1;
                    break;
                }

                avancar();
            }

            token = criar_token_base(TK_STRING, linha_inicio, coluna_inicio);

            if (!fechado) {
                copiar_lexema(&token, codigo + inicio, pos - inicio);
                token.tipo = TK_ERRO;
                definir_erro(&token, "string_nao_fechada");
                return token;
            }

            copiar_lexema(&token, codigo + inicio, pos - inicio);
            avancar();
            return token;
        }

        {
            char c = codigo[pos];
            Token token = criar_token_base(TK_ERRO, linha_inicio, coluna_inicio);

            avancar();
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

            definir_erro(&token, "simbolo_invalido");
            return token;
        }
    }

    {
        Token token = criar_token_base(TK_EOF, linha, coluna);
        strcpy(token.valor, "EOF");
        return token;
    }
}
