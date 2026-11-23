# 🏦 Carteira Digital — Sistema Bancário em C

## 📌 Sobre o Projeto

O **Carteira Digital** é um sistema bancário desenvolvido em linguagem C, com o objetivo de simular operações financeiras e aplicar conceitos fundamentais de programação.

O projeto permite cadastrar clientes, criar contas bancárias, realizar movimentações financeiras, solicitar empréstimos, adquirir títulos de capitalização e consultar históricos de operações.

O sistema foi desenvolvido de forma modular, utilizando estruturas de dados, funções, ponteiros e alocação dinâmica de memória.

## 🎯 Objetivo

Desenvolver uma aplicação bancária funcional para praticar os conhecimentos adquiridos em linguagem C, especialmente:

- Estruturas (`struct` e `typedef`).
- Funções e modularização.
- Ponteiros e manipulação de memória.
- Alocação dinâmica com `calloc()`, `malloc()` e `realloc()`.
- Estruturas condicionais e de repetição.
- Manipulação de datas e horários com `time.h`.
- Vetores, matrizes e registros.
- Validação de entradas e tratamento de erros.

## ⚙️ Funcionalidades

### 👤 Gerenciamento de Clientes
- Cadastrar novos clientes.
- Criar contas bancárias.
- Listar clientes cadastrados.
- Consultar informações e saldos.

### 💳 Tipos de Conta
O sistema oferece três modalidades:

- **Conta Corrente (01):** permite movimentações financeiras e saldo negativo.
- **Conta Salário (02):** permite movimentações e aquisição de títulos de capitalização.
- **Conta Poupança (03):** permite movimentações e participa das condições para empréstimos.

Cada conta possui uma numeração única, iniciando em `1000`, seguida pelo código correspondente ao tipo de conta.

**Exemplo:** `1000-01`

**Regra:** um cliente não pode possuir simultaneamente conta corrente e conta salário.

### 💰 Operações Bancárias
- Realizar depósitos.
- Efetuar saques.
- Transferir valores entre contas.
- Consultar saldo individual e total.
- Validar valores informados.
- Registrar movimentações no histórico.

### 🏦 Empréstimos
O cliente poderá solicitar um empréstimo caso:

- Possua conta corrente.
- Possua conta poupança com saldo positivo.
- Não tenha realizado outro empréstimo anteriormente.

O limite corresponde a **25% de dez vezes o saldo total do cliente**.

### 🎟️ Títulos de Capitalização
Clientes com conta salário podem adquirir um título quando possuírem saldo entre **R$ 1.000,00 e R$ 1.500,00**.

### 🏆 Sorteio
O sistema possui uma funcionalidade de sorteio entre os cinco clientes com maiores saldos, desde que todos tenham adquirido títulos de capitalização.

A seleção utiliza uma adaptação da sequência de Fibonacci, e o vencedor recebe um prêmio equivalente a dez vezes o saldo da conta utilizada para o sorteio.

### 📋 Histórico de Operações
O sistema registra informações como:

- Abertura de contas.
- Depósitos e saques.
- Transferências.
- Empréstimos.
- Aquisição de títulos.
- Premiações de sorteios.

Cada operação é registrada com data e horário.

### ⚠️ Alertas
O sistema apresenta alertas para clientes com saldo negativo na conta corrente.

## 🗂️ Estrutura do Projeto

```text
Carteira-Digital/
│
├── main.c
├── banco.c
├── meubanco.h
└── README.md
```

**main.c:** responsável pelo menu principal e pelas chamadas das funções.

**banco.c:** contém a implementação das operações bancárias e a lógica do sistema.

**meubanco.h:** contém as bibliotecas, estruturas e protótipos das funções.

**README.md:** documentação do projeto.

## 💻 Exemplo de Execução

```text
===== CARTEIRA DIGITAL =====

1 - Abrir banco
2 - Consultar historico
3 - Efetuar sorteio
4 - Sair

Escolha uma opcao:
```

Ao abrir o banco:

```text
===== BANCO ABERTO =====

1 - Cadastrar cliente
2 - Criar conta
3 - Depositar
4 - Sacar
5 - Transferir
6 - Consultar saldo
7 - Listar clientes
8 - Solicitar emprestimo
9 - Comprar titulo
0 - Voltar

Opcao:
```
O terminal é limpo após cada operação, mantendo a navegação organizada.

## 📚 Aprendizados

O desenvolvimento deste projeto permitiu colocar em prática conceitos importantes da linguagem C, como gerenciamento de memória, manipulação de estruturas, organização de funções e implementação de regras de negócio.

Além disso, contribuiu para o desenvolvimento do raciocínio lógico, resolução de problemas e construção de aplicações modulares.

## 📄 Finalidade

Projeto acadêmico desenvolvido para aplicar os conhecimentos de programação em linguagem C, com foco em modularização, estruturas de dados, gerenciamento de memória e simulação de operações bancárias.
