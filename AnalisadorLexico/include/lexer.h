#ifndef LEXER_H
#define LEXER_H

#include "token.h"

void iniciar_lexer(const char* codigo_fonte);
Token proximo_token();

#endif