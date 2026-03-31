#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TK_UAI,
    TK_SE,
    TK_ENTAO,
    TK_FIM,
    TK_FALA,

    TK_ID,
    TK_NUM,
    TK_STRING,
    TK_COMENTARIO,

    TK_ATRIB,
    TK_MAIOR,
    TK_MENOR,
    TK_IGUAL,
    TK_SOMA,
    TK_SUB,

    TK_EOF,
    TK_ERRO

} TipoToken;

typedef struct {
    TipoToken tipo;
    char valor[100];
    char erro[100];
    int linha;
    int coluna;
} Token;

const char* tipo_para_string(TipoToken tipo);

#endif