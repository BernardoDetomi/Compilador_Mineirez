#ifndef AST_H
#define AST_H

typedef struct AST {

    char nome[50];

    struct AST* esquerda;
    struct AST* direita;

} AST;

AST* criar_no(const char* nome);

void imprimir_ast(AST* raiz, int nivel);

#endif