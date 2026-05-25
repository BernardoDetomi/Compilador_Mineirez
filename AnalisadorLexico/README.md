# 🧀 Mineirez Compiler

Um compilador educacional desenvolvido em C para a linguagem **Mineirez**, uma linguagem de programação inspirada no dialeto mineiro do interior de Minas Gerais 🇧🇷

> *"Uai sô, aqui até código tem sotaque!"*

---

## 📌 Sobre o projeto

O **Mineirez Compiler** é um projeto acadêmico desenvolvido para a disciplina de **Compiladores**, com foco inicial na implementação de um **analisador léxico (lexer)**.

A proposta do projeto é unir conceitos formais de compiladores com uma abordagem criativa, criando uma linguagem própria com identidade cultural.

---

## 🧠 Objetivos

* Implementar um **analisador léxico em C**
* Definir uma **linguagem de programação própria**
* Aplicar conceitos como:

  * Tokens
  * Expressões regulares
  * Autômatos finitos
* Estruturar um compilador modular e escalável

---

## 🔤 Linguagem Mineirez

A linguagem Mineirez possui uma sintaxe simples, inspirada em linguagens tradicionais, mas com palavras-chave baseadas no vocabulário mineiro.

### 🧾 Exemplo de código:

```mineirez
uai trem = 10

se trem > 5 entao
    fala "trem bão demais"
fim
```

---

## 🧩 Tokens suportados

### 🟡 Palavras-chave

* `uai` → declaração
* `se` → condicional
* `entao` → bloco condicional
* `fim` → fim de bloco
* `fala` → saída (print)

### 🔵 Identificadores e literais

* Variáveis: `trem`, `x`, `valor`
* Números: `10`, `200`
* Strings: `"uai sô"`
* Comentários: `#`, `//` ou `/* ... */`

### 🔴 Operadores

* `=` atribuição
* `>` maior que
* `<` menor que
* `+` soma
* `-` subtração

### 🟣 Comentários

* `# comentário` → comentário de linha
* `// comentário` → comentário de linha
* `/* comentário */` → comentário de bloco

### ⚠️ Erros léxicos

O lexer agora devolve `TK_ERRO` para casos como:

* símbolo inválido
* string não terminada
* comentário não fechado
* número seguido de letra ou `_`

---

## ⚙️ Estrutura do projeto

```bash
mineirez-compiler/
│
├── src/            # Código fonte
│   ├── main.c
│   ├── lexer.c
│   ├── token.c
│
├── include/        # Headers
│   ├── lexer.h
│   ├── token.h
│
├── tests/          # Arquivos de teste (.mz)
│
├── Makefile        # Automação de build
└── README.md
```

---

## 🚀 Como executar

### 🔧 Pré-requisitos

* GCC instalado

### ▶️ Compilar

```bash
make
```

### ▶️ Executar

```bash
make run
```

---

## 🧪 Exemplo de saída

Entrada:

```mineirez
fala "uai sô"
```

Saída do lexer:

```text
<TK_FALA, fala>
<TK_STRING, uai sô>
<TK_EOF, EOF>
```

---

## 🛠️ Tecnologias utilizadas

* Linguagem C
* GCC (compilador)
* Makefile

---

## 📈 Próximos passos

* [ ] Implementar analisador sintático (parser)
* [ ] Construir árvore sintática (AST)
* [ ] Adicionar suporte a `senao`
* [ ] Implementar interpretador ou geração de código
* [ ] Melhorar tratamento de erros

---

## 🎓 Contexto acadêmico

Projeto desenvolvido para a disciplina de **Compiladores** no curso de Ciência da Computação.

---

## 🤝 Contribuição

Sinta-se à vontade para contribuir, sugerir melhorias ou adaptar a linguagem!

---

## 📜 Licença

Este projeto é de uso acadêmico.
Você pode adaptar e utilizar livremente para fins educacionais.

---

## 👨‍💻 Autor

**Bernardo Maia Detomi**

---

> *"Compilar é transformar ideia em execução… mesmo que seja com um trem bão de linguagem!"* 🚀
