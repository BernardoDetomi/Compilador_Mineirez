#include <stdio.h>      // Biblioteca padrão de entrada e saída
#include <ctype.h>      // Funções para classificação de caracteres (isalpha, isdigit, etc.)
#include <string.h>     // Funções para manipulação de strings

#include "token.h"      // Definição da struct Token e tipos de token
#include "lexer.h"      // Declarações do lexer

static const char* codigo;  // Ponteiro para o código fonte que será analisado
static int pos;             // Posição atual no código
static int linha;           // Linha atual
static int coluna;          // Coluna atual

// Avança para o próximo caractere do código
static void avancar(void) {
    if (codigo[pos] == '\n') {  // Se for quebra de linha
        linha++;                // Incrementa a linha
        coluna = 1;             // Reseta a coluna
    } else {
        coluna++;               // Senão, apenas incrementa a coluna
    }

    pos++;                      // Avança a posição no código
}

// Cria um token base inicializado
static Token criar_token_base(TipoToken tipo, int linha_token, int coluna_token) {
    Token token;                                 // Declara o token
    memset(&token, 0, sizeof(token));            // Zera toda a estrutura
    token.tipo = tipo;                           // Define o tipo do token
    token.linha = linha_token;                   // Define a linha onde o token começa
    token.coluna = coluna_token;                 // Define a coluna onde o token começa
    return token;                                // Retorna o token criado
}

// Copia o texto (lexema) para dentro do token
static void copiar_lexema(Token* token, const char* inicio, int tamanho) {
    int limite = tamanho < 99 ? tamanho : 99;    // Limita o tamanho a 99 caracteres
    int j;

    for (j = 0; j < limite; j++) {               // Percorre cada caractere
        char c = inicio[j];                      // Pega o caractere atual

        if (c == '\n' || c == '\r' || c == '\t') { // Se for caractere especial
            token->valor[j] = ' ';               // Substitui por espaço
        } else {
            token->valor[j] = c;                 // Copia normalmente
        }
    }

    token->valor[limite] = '\0';                 // Finaliza a string
}

// Copia comentário removendo espaços no início e fim
static void copiar_comentario(Token* token, const char* inicio, int tamanho) {
    int inicio_texto = 0;                        // Índice inicial
    int fim_texto = tamanho;                     // Índice final

    // Remove espaços do início
    while (inicio_texto < fim_texto && isspace((unsigned char) inicio[inicio_texto])) {
        inicio_texto++;
    }

    // Remove espaços do fim
    while (fim_texto > inicio_texto && isspace((unsigned char) inicio[fim_texto - 1])) {
        fim_texto--;
    }

    copiar_lexema(token, inicio + inicio_texto, fim_texto - inicio_texto); // Copia o texto limpo
}

// Define mensagem de erro no token
static void definir_erro(Token* token, const char* mensagem) {
    strncpy(token->erro, mensagem, sizeof(token->erro) - 1); // Copia mensagem com limite
    token->erro[sizeof(token->erro) - 1] = '\0';             // Garante finalização da string
}

// Inicializa o lexer com o código fonte
void iniciar_lexer(const char* codigo_fonte) {
    codigo = codigo_fonte;   // Guarda o código
    pos = 0;                 // Começa na posição 0
    linha = 1;               // Linha inicial
    coluna = 1;              // Coluna inicial
}

// Retorna o próximo token do código
Token proximo_token() {
    while (codigo[pos] != '\0') {                     // Enquanto não chegar ao fim do código
        unsigned char atual = (unsigned char) codigo[pos]; // Caractere atual
        int linha_inicio = linha;                     // Guarda linha inicial do token
        int coluna_inicio = coluna;                   // Guarda coluna inicial do token

        if (isspace(atual)) {                         // Se for espaço em branco
            avancar();                                // Ignora
            continue;                                 // Continua o loop
        }

        // Comentário iniciado com #
        if (codigo[pos] == '#') {
            int inicio;
            Token token;

            avancar();                                // Pula o '#'
            inicio = pos;                             // Marca início do comentário

            // Lê até fim da linha
            while (codigo[pos] != '\0' && codigo[pos] != '\n' && codigo[pos] != '\r') {
                avancar();
            }

            token = criar_token_base(TK_COMENTARIO, linha_inicio, coluna_inicio); // Cria token
            copiar_comentario(&token, codigo + inicio, pos - inicio);             // Copia conteúdo
            return token;                                                         // Retorna token
        }

        // Comentário //
        if (codigo[pos] == '/' && codigo[pos + 1] == '/') {
            int inicio;
            Token token;

            avancar(); // Pula '/'
            avancar(); // Pula '/'
            inicio = pos;

            while (codigo[pos] != '\0' && codigo[pos] != '\n' && codigo[pos] != '\r') {
                avancar();
            }

            token = criar_token_base(TK_COMENTARIO, linha_inicio, coluna_inicio);
            copiar_comentario(&token, codigo + inicio, pos - inicio);
            return token;
        }

        // Comentário /* */
        if (codigo[pos] == '/' && codigo[pos + 1] == '*') {
            int inicio;
            Token token;

            avancar(); // Pula '/'
            avancar(); // Pula '*'
            inicio = pos;

            while (codigo[pos] != '\0') {
                if (codigo[pos] == '*' && codigo[pos + 1] == '/') { // Encontrou fim
                    token = criar_token_base(TK_COMENTARIO, linha_inicio, coluna_inicio);
                    copiar_comentario(&token, codigo + inicio, pos - inicio);
                    avancar(); // Pula '*'
                    avancar(); // Pula '/'
                    return token;
                }

                avancar();
            }

            // Se não encontrou fechamento
            token = criar_token_base(TK_ERRO, linha_inicio, coluna_inicio);
            copiar_lexema(&token, codigo + inicio, pos - inicio);
            definir_erro(&token, "comentario_nao_fechado");
            return token;
        }

        // Identificadores ou palavras-chave
        if (isalpha(atual) || codigo[pos] == '_') {
            int inicio = pos;
            Token token;

            // Lê letras, números e _
            while (isalnum((unsigned char) codigo[pos]) || codigo[pos] == '_') {
                avancar();
            }

            token = criar_token_base(TK_ID, linha_inicio, coluna_inicio);
            copiar_lexema(&token, codigo + inicio, pos - inicio);

            // Verifica palavras reservadas
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

        // Números
        if (isdigit(atual)) {
            int inicio = pos;
            Token token;

            while (isdigit((unsigned char) codigo[pos])) {
                avancar();
            }

            token = criar_token_base(TK_NUM, linha_inicio, coluna_inicio);
            copiar_lexema(&token, codigo + inicio, pos - inicio);

            // Verifica erro: número seguido de letra
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

        // Strings
        if (codigo[pos] == '"') {
            int inicio;
            int fechado;
            Token token;

            avancar();   // Pula "
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
            avancar(); // Pula "
            return token;
        }

        // Símbolos simples
        {
            char c = codigo[pos];
            Token token = criar_token_base(TK_ERRO, linha_inicio, coluna_inicio);

            avancar(); // Avança posição
            token.valor[0] = c;  // Armazena símbolo
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

    // Fim do arquivo
    {
        Token token = criar_token_base(TK_EOF, linha, coluna);
        strcpy(token.valor, "EOF");
        return token;
    }
}