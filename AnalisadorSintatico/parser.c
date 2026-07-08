#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"
#include "token.h"

typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_ERRO
} TipoDado;

typedef struct {  // estrutura para representar um símbolo na tabela de símbolos
    char nome[100];
    TipoDado tipo;
    int usada;
    int linha;
    int coluna;
} Simbolo;

#define MAX_SIMBOLOS 100
#define MAX_MSG 200

static Token token_atual;
static int houve_erro_sintatico;
static int houve_erro_semantico;
static Simbolo tabela_simbolos[MAX_SIMBOLOS];
static int quantidade_simbolos;

static char erros_semanticos[MAX_MSG][200];
static int qtd_erros_semanticos = 0;
static char avisos_semanticos[MAX_MSG][200];
static int qtd_avisos_semanticos = 0;

static void avancar() { 
// obter o próximo token, ignorando comentários 
    token_atual = proximo_token();

    while (token_atual.tipo == TK_COMENTARIO) {
        token_atual = proximo_token();
    }
}

static void erro(const char* mensagem) { 
// registrar erro sintático
    houve_erro_sintatico = 1;

    printf("\nERRO SINTATICO\n");
    printf("%s\n", mensagem);
    printf(
        "linha %d coluna %d\n",
        token_atual.linha,
        token_atual.coluna
    );
}

static void erro_semantico(const char* mensagem, int linha, int coluna) { 
// registrar warnings
    houve_erro_semantico = 1;

    if (qtd_erros_semanticos < MAX_MSG) {
        snprintf(erros_semanticos[qtd_erros_semanticos], 200, "%s (linha %d coluna %d)", mensagem, linha, coluna);
        qtd_erros_semanticos++;
    }
}

static void consumir(TipoToken tipo) { 
// consumir um token do tipo esperado ou registrar erro caso seja diferente
    if (token_atual.tipo == tipo) {

        avancar();

    } else {

        printf("\nEsperado: %s\n", tipo_para_string(tipo));
        printf("Recebido: %s\n", tipo_para_string(token_atual.tipo));
        erro("token inesperado");
        avancar();
    }
}

static const char* tipo_dado_para_string(TipoDado tipo) { 
// converter tipo de dado para string para mensagens de erro e tabela de símbolos
    switch (tipo) {
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        case TIPO_ERRO: return "erro";
        default: return "desconhecido";
    }
}

static const char* operacao_para_string(TipoToken operacao) { 
// converter tipo de operação para string para mensagens de erro e tabela de operações
    switch (operacao) {
        case TK_SOMA: return "+";
        case TK_SUB: return "-";
        case TK_MULT: return "*";
        case TK_DIV: return "/";
        default: return "?";
    }
}

static TipoDado tipo_de_numero(const char* lexema) { 
// determinar tipo de dado de um número com base no seu lexema
    if (strchr(lexema, '.') != NULL) {
        return TIPO_FLOAT;
    }

    return TIPO_INT;
}

static int tipo_numerico(TipoDado tipo) { 
// verificar se um tipo de dado é numérico (int ou float)
    return tipo == TIPO_INT || tipo == TIPO_FLOAT;
}

static Simbolo* buscar_simbolo(const char* nome) { 
// buscar um símbolo na tabela de símbolos pelo nome
    int i;

    for (i = 0; i < quantidade_simbolos; i++) {
        if (strcmp(tabela_simbolos[i].nome, nome) == 0) {
            return &tabela_simbolos[i];
        }
    }

    return NULL;
}

static void imprimir_tabela_simbolos(void) { 
// imprimir a tabela de símbolos ao final da análise
    int i;

    printf("\nTABELA DE SIMBOLOS\n");
    printf("%-20s %-12s %-8s\n", "NOME", "TIPO", "USADA");
    printf("--------------------------------------------------\n");

    for (i = 0; i < quantidade_simbolos; i++) {
        printf(
            "%-20s %-12s %-8s\n",
            tabela_simbolos[i].nome,
            tipo_dado_para_string(tabela_simbolos[i].tipo),
            tabela_simbolos[i].usada ? "SIM" : "NAO"
        );
    }

    if (quantidade_simbolos == 0) {
        printf("%-20s %-12s %-8s\n", "(vazia)", "-", "-");
    }
}

static TipoDado resultado_operacao_tabela( 
// função auxiliar para calcular o resultado de uma operação aritmética para a tabela de operações
    TipoToken operacao,
    TipoDado esquerda,
    TipoDado direita
) {

    if (esquerda == TIPO_ERRO || direita == TIPO_ERRO) { 
        // se um dos operandos é erro, o resultado também é erro
        return TIPO_ERRO;
    }

    if (operacao == TK_DIV) { 
        // divisão sempre resulta em float
        return TIPO_FLOAT;
    }

    if (esquerda == TIPO_FLOAT || direita == TIPO_FLOAT) { 
        // se um dos operandos é float, o resultado é float
        return TIPO_FLOAT;
    }

    return TIPO_INT;
}

static void imprimir_tabela_operacoes(void) { 
    // imprimir uma tabela de tipos e resultados para as operações aritméticas ao final da análise

    TipoToken operacoes[] = { TK_SOMA, TK_SUB, TK_MULT, TK_DIV };
    TipoDado tipos[] = { TIPO_INT, TIPO_FLOAT };
    int i;
    int j;
    int k;

    printf("\nTABELA DE TIPOS E OPERACOES\n");
    printf("%-10s %-10s %-10s %-10s\n", "OP", "ESQ", "DIR", "RESULT");
    printf("--------------------------------------------------\n");

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                TipoDado resultado = resultado_operacao_tabela(
                    operacoes[i],
                    tipos[j],
                    tipos[k]
                );

                printf(
                    "%-10s %-10s %-10s %-10s\n",
                    operacao_para_string(operacoes[i]),
                    tipo_dado_para_string(tipos[j]),
                    tipo_dado_para_string(tipos[k]),
                    tipo_dado_para_string(resultado)
                );
            }
        }
    }
}

static Simbolo* inserir_simbolo( 
    // inserir um novo símbolo na tabela de símbolos, verificando duplicatas e limites da tabela
    const char* nome,
    TipoDado tipo,
    int linha,
    int coluna
) {

    Simbolo* existente = buscar_simbolo(nome); 
    // verificar se já existe um símbolo com o mesmo nome

    if (existente != NULL) {
        erro_semantico("variavel ja declarada", linha, coluna);
        return NULL;
    }

    if (quantidade_simbolos >= MAX_SIMBOLOS) {
        erro_semantico("limite da tabela de simbolos atingido", linha, coluna);
        return NULL;
    }

    strncpy(tabela_simbolos[quantidade_simbolos].nome, nome, sizeof(tabela_simbolos[quantidade_simbolos].nome) - 1);
    tabela_simbolos[quantidade_simbolos].nome[sizeof(tabela_simbolos[quantidade_simbolos].nome) - 1] = '\0';
    tabela_simbolos[quantidade_simbolos].tipo = tipo;
    tabela_simbolos[quantidade_simbolos].usada = 0;
    tabela_simbolos[quantidade_simbolos].linha = linha;
    tabela_simbolos[quantidade_simbolos].coluna = coluna;

    quantidade_simbolos++;

    return &tabela_simbolos[quantidade_simbolos - 1];
}

static TipoDado verificar_operacao_aritmetica( 
    // verificar tipos e resultado de uma operação aritmética, registrando erros semânticos se os tipos forem inválidos
    TipoDado esquerda,
    TipoToken operacao,
    TipoDado direita,
    int linha,
    int coluna
) {

    if (esquerda == TIPO_ERRO || direita == TIPO_ERRO) {
        return TIPO_ERRO;
    }

    if (!tipo_numerico(esquerda) || !tipo_numerico(direita)) {
        erro_semantico("operacao aritmetica com tipos invalidos", linha, coluna);
        return TIPO_ERRO;
    }

    if (operacao == TK_DIV) {
        return TIPO_FLOAT;
    }

    if (esquerda == TIPO_FLOAT || direita == TIPO_FLOAT) {
        return TIPO_FLOAT;
    }

    return TIPO_INT;
}

static TipoDado verificar_operacao_relacional( 
    // verificar tipos e resultado de uma operação relacional, registrando erros semânticos se os tipos forem inválidos
    TipoDado esquerda,
    TipoDado direita,
    int linha,
    int coluna
) {

    if (esquerda == TIPO_ERRO || direita == TIPO_ERRO) {
        return TIPO_ERRO;
    }

    if (!tipo_numerico(esquerda) || !tipo_numerico(direita)) {
        erro_semantico("comparacao com tipos invalidos", linha, coluna);
        return TIPO_ERRO;
    }

    return TIPO_INT;
}

static int tipos_compativeis_atribuicao(TipoDado destino, TipoDado origem) { 
    // verificar se os tipos são compatíveis para atribuição, permitindo int para float

    if (destino == origem) {
        return 1;
    }

    if (destino == TIPO_FLOAT && origem == TIPO_INT) {
        return 1;
    }

    return 0;
}

void iniciar_parser() {

    houve_erro_sintatico = 0;
    houve_erro_semantico = 0;
    quantidade_simbolos = 0;

    avancar();
}

static TipoDado expressao();
static TipoDado termo();
static TipoDado fator();

static void comando();

static void declaracao() {

    TipoDado tipo_variavel = TIPO_ERRO; 
    int linha_id;
    int coluna_id;
    char nome[100];

    consumir(TK_UAI);

    if (token_atual.tipo == TK_INT) {
        tipo_variavel = TIPO_INT;
        consumir(TK_INT);
    } else if (token_atual.tipo == TK_FLOAT) {
        tipo_variavel = TIPO_FLOAT;
        consumir(TK_FLOAT);
    }

    if (token_atual.tipo != TK_ID) {
        erro("identificador esperado na declaracao");
        avancar();
        return;
    }

    linha_id = token_atual.linha;
    coluna_id = token_atual.coluna;
    strncpy(nome, token_atual.valor, sizeof(nome) - 1);
    nome[sizeof(nome) - 1] = '\0';

    consumir(TK_ID);

    inserir_simbolo(nome, tipo_variavel, linha_id, coluna_id);
}

static void atribuicao() {

    Simbolo* simbolo;
    TipoDado tipo_expressao;
    int linha_id = token_atual.linha;
    int coluna_id = token_atual.coluna;
    char nome[100];

    strncpy(nome, token_atual.valor, sizeof(nome) - 1);
    nome[sizeof(nome) - 1] = '\0';

    simbolo = buscar_simbolo(nome);

    if (simbolo == NULL) {
        erro_semantico("variavel nao declarada", linha_id, coluna_id);
    }

    if (simbolo != NULL) {
        simbolo->usada = 1;
    }

    consumir(TK_ID);
    consumir(TK_ATRIB);

    tipo_expressao = expressao();

    if (simbolo != NULL) {
        if (simbolo->tipo == TIPO_ERRO) {
            /* inferir tipo na primeira atribuicao */
            simbolo->tipo = tipo_expressao;
        } else if (tipo_expressao != TIPO_ERRO) {
            if (!tipos_compativeis_atribuicao(simbolo->tipo, tipo_expressao)) {
                erro_semantico("tipos incompativeis na atribuicao", linha_id, coluna_id);
            }
        }
    }
}

static void fala() {

    consumir(TK_FALA);
    consumir(TK_STRING);
}

static TipoDado fator() {

    if (token_atual.tipo == TK_NUM) {

        TipoDado tipo = tipo_de_numero(token_atual.valor);

        consumir(TK_NUM);

        return tipo;

    } else if (token_atual.tipo == TK_ID) {

        Simbolo* simbolo = buscar_simbolo(token_atual.valor);
        int linha_id = token_atual.linha;
        int coluna_id = token_atual.coluna;

        if (simbolo == NULL) {
            erro_semantico("variavel nao declarada", linha_id, coluna_id);
            consumir(TK_ID);
            return TIPO_ERRO;
        }

        simbolo->usada = 1;
        consumir(TK_ID);

        return simbolo->tipo;

    } else if (token_atual.tipo == TK_ABRE_PAR) {

        TipoDado tipo;

        consumir(TK_ABRE_PAR);
        tipo = expressao();
        consumir(TK_FECHA_PAR);

        return tipo;

    } else {

        erro("fator invalido");
        avancar();

        return TIPO_ERRO;
    }
}

static TipoDado termo() {

    TipoDado tipo_esquerda = fator();

    while (
        token_atual.tipo == TK_MULT ||
        token_atual.tipo == TK_DIV
    ) {

        TipoToken operacao = token_atual.tipo;
        int linha_op = token_atual.linha;
        int coluna_op = token_atual.coluna;
        TipoDado tipo_direita;

        avancar();

        tipo_direita = fator();

        tipo_esquerda = verificar_operacao_aritmetica(
            tipo_esquerda,
            operacao,
            tipo_direita,
            linha_op,
            coluna_op
        );
    }

    return tipo_esquerda;
}

static TipoDado expressao() {

    TipoDado tipo_esquerda = termo();

    while (
        token_atual.tipo == TK_SOMA ||
        token_atual.tipo == TK_SUB ||
        token_atual.tipo == TK_MAIOR ||
        token_atual.tipo == TK_MENOR
    ) {

        TipoToken operacao = token_atual.tipo;
        int linha_op = token_atual.linha;
        int coluna_op = token_atual.coluna;
        TipoDado tipo_direita;

        avancar();

        tipo_direita = termo();

        if (operacao == TK_MAIOR || operacao == TK_MENOR) {
            tipo_esquerda = verificar_operacao_relacional(
                tipo_esquerda,
                tipo_direita,
                linha_op,
                coluna_op
            );
        } else {
            tipo_esquerda = verificar_operacao_aritmetica(
                tipo_esquerda,
                operacao,
                tipo_direita,
                linha_op,
                coluna_op
            );
        }
    }

    return tipo_esquerda;
}

static void condicional() {

    consumir(TK_SE);
    expressao();
    consumir(TK_ENTAO);

    while (
        token_atual.tipo != TK_FIM &&
        token_atual.tipo != TK_EOF
    ) {
        comando();
    }

    consumir(TK_FIM);
}

static void repeticao() {

    consumir(TK_ENQUANTO);
    expressao();
    consumir(TK_FACA);

    while (
        token_atual.tipo != TK_FIM &&
        token_atual.tipo != TK_EOF
    ) {
        comando();
    }

    consumir(TK_FIM);
}

static void comando() {

    switch (token_atual.tipo) {

        case TK_UAI:
            declaracao();
            break;

        case TK_ID:
            atribuicao();
            break;

        case TK_SE:
            condicional();
            break;

        case TK_ENQUANTO:
            repeticao();
            break;

        case TK_FALA:
            fala();
            break;

        default:
            erro("comando invalido");
            avancar();
    }
}

void programa() {

    while (token_atual.tipo != TK_EOF) {
        comando();
    }

    if (houve_erro_sintatico) {
        printf("\nANALISE SINTATICA FINALIZADA COM ERROS!\n");
    } else {
        printf("\nSUCESSO SINTATICO!\n");
    }

    if (houve_erro_semantico) {
        printf("ANALISE SEMANTICA FINALIZADA COM ERROS!\n");
    } else {
        printf("SUCESSO SEMANTICO!\n");
    }

    /* imprimir erros semanticos coletados */
    if (qtd_erros_semanticos > 0) {
        int i;
        printf("\nERROS SEMANTICOS (%d):\n", qtd_erros_semanticos);
        for (i = 0; i < qtd_erros_semanticos; i++) {
            printf("%s\n", erros_semanticos[i]);
        }
    }

    /* gerar avisos para variaveis nao usadas */
    if (quantidade_simbolos > 0) {
        int i;
        for (i = 0; i < quantidade_simbolos; i++) {
            if (!tabela_simbolos[i].usada) {
                if (qtd_avisos_semanticos < MAX_MSG) {
                    snprintf(avisos_semanticos[qtd_avisos_semanticos], 200, "variavel '%s' declarada e nao usada (linha %d)", tabela_simbolos[i].nome, tabela_simbolos[i].linha);
                    qtd_avisos_semanticos++;
                }
            }
        }
    }

    if (qtd_avisos_semanticos > 0) {
        int i;
        printf("\nWARNINGS (%d):\n", qtd_avisos_semanticos);
        for (i = 0; i < qtd_avisos_semanticos; i++) {
            printf("%s\n", avisos_semanticos[i]);
        }
    }

    imprimir_tabela_operacoes();
    imprimir_tabela_simbolos();
}
