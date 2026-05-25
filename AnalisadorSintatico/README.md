# 🧀 Mineirez Compiler

Um compilador educacional desenvolvido em C para a linguagem **Mineirez**, uma linguagem de programação inspirada no dialeto mineiro do interior de Minas Gerais 🇧🇷

> *"Uai sô, aqui até código tem sotaque!"*

---

## 📌 Sobre o projeto

O **Mineirez Compiler** é um projeto acadêmico desenvolvido para a disciplina de **Compiladores**, com foco na implementação das primeiras etapas de um compilador:

* ✅ Analisador Léxico (Lexer)
* ✅ Analisador Sintático (Parser)

A proposta do projeto é unir conceitos formais de compiladores com uma abordagem criativa, criando uma linguagem própria com identidade cultural.

---

## 🧠 Objetivos

* Implementar um **analisador léxico em C**
* Implementar um **analisador sintático recursivo descendente**
* Construir uma linguagem própria
* Aplicar conceitos como:

  * Tokens
  * Expressões regulares
  * Gramáticas livres de contexto
  * Parsing
  * Árvores sintáticas
  * Recuperação de erros

---

# 🏗️ Etapas implementadas

## ✅ Analisador Léxico

O lexer é responsável por:

* Ler o código fonte caractere por caractere
* Identificar tokens válidos
* Detectar erros léxicos
* Gerar a sequência de tokens utilizada pelo parser

---

## ✅ Analisador Sintático

O parser é responsável por:

* Validar a estrutura gramatical da linguagem
* Verificar comandos válidos
* Validar expressões
* Identificar erros sintáticos
* Informar linha e coluna do erro
* Continuar análise após erros simples
* Preparar a construção da AST (Árvore Sintática)

O parser foi implementado utilizando a técnica de:

### 🔹 Parser Descendente Recursivo

Cada regra da gramática é representada por uma função em C.

---

# 🔤 Linguagem Mineirez

A linguagem Mineirez possui uma sintaxe simples inspirada em linguagens tradicionais, mas utilizando expressões mineiras como palavras-chave.

---

# 🧾 Exemplos de código

## Declaração

```mineirez
uai x
```

---

## Atribuição

```mineirez
x = 10 + 20
```

---

## Condicional

```mineirez
se x > 5 entao
    fala "trem bão"
fim
```

---

## Repetição

```mineirez
enquanto x > 0 faca
    x = x - 1
fim
```

---

# 🧩 Tokens suportados

## 🟡 Palavras-chave

| Palavra    | Significado        |
| ---------- | ------------------ |
| `uai`      | declaração         |
| `se`       | condicional        |
| `entao`    | início do bloco    |
| `fim`      | fim do bloco       |
| `fala`     | saída              |
| `enquanto` | repetição          |
| `faca`     | bloco de repetição |

---

## 🔵 Identificadores e literais

* Variáveis:

```mineirez
x
valor
trem
```

* Números:

```mineirez
10
200
999
```

* Strings:

```mineirez
"uai sô"
```

---

## 🔴 Operadores

| Operador | Função        |
| -------- | ------------- |
| `=`      | atribuição    |
| `>`      | maior que     |
| `<`      | menor que     |
| `+`      | soma          |
| `-`      | subtração     |
| `*`      | multiplicação |
| `/`      | divisão       |

---

## 🟣 Delimitadores

| Símbolo | Função          |
| ------- | --------------- |
| `(`     | abre expressão  |
| `)`     | fecha expressão |

---

## ⚠️ Erros léxicos tratados

O lexer detecta:

* símbolo inválido
* string não fechada
* comentário não fechado
* número seguido de letra
* token inválido

---

## ⚠️ Erros sintáticos tratados

O parser detecta:

* token inesperado
* expressão inválida
* comando inválido
* fechamento incorreto de blocos
* ausência de `fim`
* ausência de `entao`
* ausência de `faca`

Exemplo:

```text
ERRO SINTATICO
Esperado: TK_ENTAO
Recebido: TK_ID
linha 5 coluna 12
```

---

# 📚 Gramática simplificada

```text
programa -> comando*

comando ->
      declaracao
    | atribuicao
    | condicional
    | repeticao
    | fala

declaracao -> UAI ID

atribuicao -> ID '=' expressao

condicional ->
    SE expressao ENTAO
    comando*
    FIM

repeticao ->
    ENQUANTO expressao FACA
    comando*
    FIM

expressao ->
    termo (('+' | '-' | '>' | '<') termo)*

termo ->
    fator (('*' | '/') fator)*

fator ->
      NUM
    | ID
    | '(' expressao ')'
```

---

# 🌳 Árvore Sintática (AST)

O projeto já possui a estrutura inicial da AST.

A árvore sintática será utilizada futuramente para:

* interpretação
* análise semântica
* geração de código
* otimizações

---

# ⚙️ Estrutura do projeto

```bash
mineirez-compiler/
│
├── src/
│   ├── main.c
│   ├── lexer.c
│   ├── parser.c
│   ├── token.c
│   ├── ast.c
│
├── include/
│   ├── lexer.h
│   ├── parser.h
│   ├── token.h
│   ├── ast.h
│
├── tests/
│   ├── teste.mz
│
├── Makefile
└── README.md
```

---

# 🚀 Como executar

## 🔧 Pré-requisitos

* GCC instalado
* Make instalado

---

# ▶️ Compilar

```bash
make
```

---

# ▶️ Executar

```bash
make run
```

---

# ▶️ Limpar arquivos compilados

```bash
make clean
```

---

# 🧪 Exemplo de entrada

Arquivo `.mz`:

```mineirez
uai x

x = 10 + 20

se x > 5 entao
    fala "uai sô"
fim
```

---

# ✅ Saída esperada

```text
SUCESSO SINTATICO!
```

---

# ❌ Exemplo com erro

Entrada:

```mineirez
x = 10 +
```

Saída:

```text
ERRO SINTATICO
fator invalido
linha 1 coluna 9
```

---

# 🛠️ Tecnologias utilizadas

* Linguagem C
* GCC
* Makefile

---

# 📈 Próximos passos

* [ ] Construção completa da AST
* [ ] Implementar análise semântica
* [ ] Adicionar suporte a `senao`
* [ ] Tabela de símbolos
* [ ] Geração de código intermediário
* [ ] Interpretador da linguagem
* [ ] Melhor recuperação de erros

---

# 🎓 Contexto acadêmico

Projeto desenvolvido para a disciplina de **Compiladores** no curso de Ciência da Computação.

---

# 🤝 Contribuição

Sinta-se à vontade para contribuir, sugerir melhorias ou adaptar a linguagem.

---

# 📜 Licença

Projeto de uso acadêmico e educacional.

---

# 👨‍💻 Autor

**Bernardo Maia Detomi**

---

> *"Compilar é transformar ideia em execução… mesmo que seja com um trem bão de linguagem!"* 🚀