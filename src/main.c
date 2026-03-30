#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

int main() {
    char codigo[] = "uai trem = 10\nfala \"uai sô\"";

    iniciar_lexer(codigo);

    Token t;

    do {
        t = proximo_token();
        printf("<%s, %s>\n", tipo_para_string(t.tipo), t.valor);
    } while (t.tipo != TK_EOF);

    return 0;
}