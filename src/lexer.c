#include <ctype.h>
#include <string.h>
#include "lexer.h"

static const char* codigo;
static int pos;

void iniciar_lexer(const char* codigo_fonte) {
    codigo = codigo_fonte;
    pos = 0;
}