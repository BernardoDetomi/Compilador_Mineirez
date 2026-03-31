#include "token.h"

const char* tipo_para_string(TipoToken tipo) {
    switch (tipo) {
        case TK_UAI: return "TK_UAI";
        case TK_SE: return "TK_SE";
        case TK_ENTAO: return "TK_ENTAO";
        case TK_FIM: return "TK_FIM";
        case TK_FALA: return "TK_FALA";
        case TK_ID: return "TK_ID";
        case TK_NUM: return "TK_NUM";
        case TK_STRING: return "TK_STRING";
        case TK_COMENTARIO: return "TK_COMENTARIO";
        case TK_ATRIB: return "TK_ATRIB";
        case TK_MAIOR: return "TK_MAIOR";
        case TK_MENOR: return "TK_MENOR";
        case TK_IGUAL: return "TK_IGUAL";
        case TK_SOMA: return "TK_SOMA";
        case TK_SUB: return "TK_SUB";
        case TK_EOF: return "TK_EOF";
        case TK_ERRO: return "TK_ERRO";
        default: return "DESCONHECIDO";
    }
}