
#ifndef MEUBANCO_H
#define MEUBANCO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HIST 100
#define TAM_TEXTO 120

typedef struct {
    int numero;
    int tipo;
    double saldo;
    time_t abertura;
    time_t ultimaOperacao;
    char historico[MAX_HIST][TAM_TEXTO];
    int qtdHistorico;
} Conta;

typedef struct {
    char nome[80];
    Conta contas[3];
    int possui[3];
    int emprestimoFeito;
    int titulo;
    double premio;
} Cliente;

typedef struct {
    Cliente *clientes;
    int quantidade;
    int capacidade;
    int proximoNumero;
    int aberto;
} Banco;
void limparTela(void);
// Entrada de dados
int lerInteiro(const char *mensagem);
double lerValor(const char *mensagem);

// Memoria e cadastro
void iniciarBanco(Banco *banco);
int aumentarCapacidade(Banco *banco);
void liberarMemoria(Banco *banco);
void cadastrarCliente(Banco *banco);
void criarConta(Banco *banco);

// Operacoes bancarias
void depositar(Banco *banco);
void sacar(Banco *banco);
void transferir(Banco *banco);
void consultarSaldo(Banco *banco);
void listarClientes(Banco *banco);
void solicitarEmprestimo(Banco *banco);
void comprarTitulo(Banco *banco);

// Historico e sorteio
void registrar(Conta *conta, const char *texto);
void consultarHistorico(Banco *banco);
void alertas(Banco *banco);
void sortear(Banco *banco);

// Menu interno
void abrirBanco(Banco *banco);

void limparTela(void);

#endif
