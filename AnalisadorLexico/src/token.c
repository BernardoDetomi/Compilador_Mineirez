#include "token.h"

const char* tipo_para_string(TipoToken tipo) {
    switch (tipo) {
        case TK_UAI: return "TK_UAI"; // palavra reservada principal
        case TK_SE: return "TK_SE"; // condicional
        case TK_ENTAO: return "TK_ENTAO"; // parte verdadeira do condicional
        case TK_FIM: return "TK_FIM"; // fim do bloco
        case TK_FALA: return "TK_FALA"; // print
        case TK_ID: return "TK_ID"; // identificador (variável, função, etc.)
        case TK_NUM: return "TK_NUM"; // número (inteiro)
        case TK_STRING: return "TK_STRING"; // string (entre aspas)
        case TK_COMENTARIO: return "TK_COMENTARIO"; // comentário
        case TK_ATRIB: return "TK_ATRIB"; // atribuição
        case TK_MAIOR: return "TK_MAIOR"; // operador de maior
        case TK_MENOR: return "TK_MENOR"; // operador de menor
        case TK_IGUAL: return "TK_IGUAL"; // operador de igualdade
        case TK_SOMA: return "TK_SOMA"; // operador de soma
        case TK_SUB: return "TK_SUB"; // operador de subtração
        case TK_EOF: return "TK_EOF"; // fim do arquivo
        case TK_ERRO: return "TK_ERRO"; // token inválido ou erro de análise
        default: return "DESCONHECIDO"; // caso para tipos de token desconhecidos
    }
}