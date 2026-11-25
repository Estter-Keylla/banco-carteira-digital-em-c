#include "meubanco.h"

void limparTela(void) {
    system("cls");
}

// Le um numero inteiro
int lerInteiro(const char *mensagem) {
    char linha[100], extra;
    int valor;
//while(1) cria uma repetição contínua
    while (1) {
        printf("%s", mensagem);
        
        
        if (!fgets(linha, sizeof(linha), stdin))
            exit(EXIT_FAILURE);

        if (sscanf(linha, " %d %c", &valor, &extra) == 1)
            return valor;

        printf("Digite um numero inteiro valido.\n");
    }
}

// Le um valor decimal
double lerValor(const char *mensagem) {
    char linha[100], extra;
    double valor;

    while (1) {
        printf("%s", mensagem);

        if (!fgets(linha, sizeof(linha), stdin))
            exit(EXIT_FAILURE);
// l e extrai dados formatados a partir de uma string / sscanf
        if (sscanf(linha, " %lf %c", &valor, &extra) == 1 &&
            valor >= 0 && valor <= 1000000000.0)
            return valor;

        printf("Digite um valor valido e nao negativo.\n");
    }
}

// Inicializa o banco
void iniciarBanco(Banco *banco) {
    banco->quantidade = 0;
    banco->capacidade = 2;
    banco->proximoNumero = 1000;
    banco->aberto = 0;

    banco->clientes = (Cliente *) calloc(
        banco->capacidade, sizeof(Cliente)
    );

    if (banco->clientes == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }
}


// Dobra a capacidade do vetor
int aumentarCapacidade(Banco *banco) {
    int nova = banco->capacidade * 2;

    Cliente *temp = (Cliente *) realloc(
        banco->clientes, nova * sizeof(Cliente)
    );

    if (temp == NULL) {
        printf("Erro ao aumentar memoria.\n");
        return 0;
    }

    banco->clientes = temp;
    banco->capacidade = nova;
    return 1;
}

// Libera a memoria
void liberarMemoria(Banco *banco) {
    free(banco->clientes);
    banco->clientes = NULL;
    banco->quantidade = 0;
    banco->capacidade = 0;
}

// Registra uma operacao com data e hora
void registrar(Conta *conta, const char *texto) {
    time_ttime_t agora = time(NULL);
    char data[30] = "Data indisponivel";
    struct tm *info = localtime(&agora);

    if (info != NULL)
        strftime(data, sizeof(data),
                 "%d/%m/%Y %H:%M", info);

    conta->ultimaOperacao = agora;

    if (conta->qtdHistorico < MAX_HIST) {
        snprintf(
            conta->historico[conta->qtdHistorico],
            TAM_TEXTO,
            "%s - %.75s", data, texto
        );
        conta->qtdHistorico++;
    }
}

// Procura cliente pelo nome
static Cliente *buscarCliente(Banco *banco) {
    char nome[80];

    printf("Nome do cliente: ");
    if (!fgets(nome, sizeof(nome), stdin)) //ler o nome digitado pelo usuário e verificar se ocorreu alguma falha durante a leitura.
        return NULL;

    nome[strcspn(nome, "\n")] = '\0'; //remover a quebra de linha

    for (int i = 0; i < banco->quantidade; i++) {
        if (strcmp(banco->clientes[i].nome, nome) == 0)
            return &banco->clientes[i];
    }

    printf("Cliente nao encontrado.\n");
    return NULL;
}

// Seleciona uma conta existente
static Conta *selecionarConta(Cliente *cliente) {
    int tipo = lerInteiro(
        "Tipo (1-Corrente, 2-Salario, 3-Poupanca): "
    );

    if (tipo < 1 || tipo > 3 ||
        !cliente->possui[tipo - 1]) {
        printf("Conta inexistente.\n");
        return NULL;
    }

    return &cliente->contas[tipo - 1];
}

// Calcula o saldo total de um cliente
static double saldoTotal(Cliente *cliente) {
    double total = 0;

    for (int i = 0; i < 3; i++) {
        if (cliente->possui[i])
            total += cliente->contas[i].saldo;
    }

    return total;
}

// Cadastra um cliente
void cadastrarCliente(Banco *banco) {
    if (banco->quantidade == banco->capacidade) {
        if (!aumentarCapacidade(banco))
            return;
    }

    char nome[80];

    printf("Nome do novo cliente: ");
    if (!fgets(nome, sizeof(nome), stdin))//verifica se falhou
        return;

//calcula o tamanho do segmento inicial de uma string que não contém nenhum caractere de um segundo conjunto de caracteres.
//retorna o tamanho da string (a posição do terminador atual).
    nome[strcspn(nome, "\n")] = '\0';
//calcula o tamanho de uma string
    if (strlen(nome) == 0) {
        printf("Nome nao pode ser vazio.\n");
        return;
    }

    for (int i = 0; i < banco->quantidade; i++) {
        if (strcmpstrcmp(banco->clientes[i].nome, nome) == 0) {
            printf("Cliente ja cadastrado.\n");
            return;
        }
    }

    Cliente *cliente = &banco->clientes[banco->quantidade];

    memset(cliente, 0, sizeof(Cliente)); //preenche todo o espaço de memória da variável cliente com zeros, limpando qualquer "lixo" de memória antes de você começar a usá-la.
    strcpy(cliente->nome, nome); //Copia o conteúdo da string nome para o campo nome do cliente.

    banco->quantidade++;

    printf("Cliente cadastrado com sucesso!\n");
}

// Cria uma conta para um cliente
void criarConta(Banco *banco) {
    Cliente *cliente = buscarCliente(banco);
    if (cliente == NULL) return;

    int tipo = lerInteiro(
        "Tipo (1-Corrente, 2-Salario, 3-Poupanca): "
    );

    if (tipo < 1 || tipo > 3) {
        printf("Tipo invalido.\n");
        return;
    }

    if (cliente->possui[tipo - 1]) {
        printf("Cliente ja possui esse tipo de conta.\n");
        return;
    }

    if ((tipo == 1 && cliente->possui[1]) ||
        (tipo == 2 && cliente->possui[0])) {
        printf("Conta corrente e salario nao podem coexistir.\n");
        return;
    }

    Conta *conta = &cliente->contas[tipo - 1];

    memset(conta, 0, sizeof(Conta));

//Pega o próximo número disponível no banco e dá para essa conta. O ++ no final avisa o banco para somar +1 logo depois, garantindo que o próximo cliente receba um número novo.
    conta->numero = banco->proximoNumero++;
    conta->tipo = tipo;
    conta->saldo = 0;
    conta->abertura = time(NULL);

    cliente->possui[tipo - 1] = 1; //Vai naquela gaveta de controle (possui) e coloca o número 1

    registrar(conta, "Abertura da conta");

    printf("Conta %04d-%02d criada com saldo R$ 0,00.\n",
           conta->numero, conta->tipo);
}

// Deposita dinheiro
void depositar(Banco *banco) {
    Cliente *cliente = buscarCliente(banco);
    if (cliente == NULL) return;

    Conta *conta = selecionarConta(cliente);
    if (conta == NULL) return;

    double valor = lerValor("Valor do deposito: R$ ");

    if (valor <= 0) {
        printf("O deposito deve ser positivo.\n");
        return;
    }

    conta->saldo += valor;

    char texto[100];
    snprintf(texto, sizeof(texto), //escreve o texto formatado dentro da variável texto.
             "Deposito de R$ %.2f", valor);
    registrar(conta, texto);

    printf("Deposito realizado. Saldo: R$ %.2f\n",
           conta->saldo);
}

// Realiza saque
void sacar(Banco *banco) {
    Cliente *cliente = buscarCliente(banco);
    if (cliente == NULL) return;

    Conta *conta = selecionarConta(cliente);
    if (conta == NULL) return;

    double valor = lerValor("Valor do saque: R$ ");

    if (valor <= 0) {
        printf("O saque deve ser positivo.\n");
        return;
    }

    // Apenas corrente pode ficar negativa
    if (valor > conta->saldo && conta->tipo != 1) {
        printf("Saldo insuficiente.\n");
        return;
    }

    conta->saldo -= valor; //Subtrai o valor do saque diretamente do saldo da conta

    char texto[100];
    snprintf(texto, sizeof(texto),
             "Saque de R$ %.2f", valor);
    registrar(conta, texto);

    printf("Saque realizado. Saldo: R$ %.2f\n",
           conta->saldo);

    if (conta->saldo < 0)
        printf("ALERTA: conta corrente negativa!\n");
}

// Transfere entre contas - busca o cliente de origem com buscarCliente(banco). Se retornar NULL, encerra a função.
void transferir(Banco *banco) {
    printf("\nCONTA DE ORIGEM\n");
    Cliente *origem = buscarCliente(banco);
    if (origem == NULL) return;

    Conta *contaOrigem = selecionarConta(origem);
    if (contaOrigem == NULL) return;

    printf("\nCONTA DE DESTINO\n");
    Cliente *destino = buscarCliente(banco);
    if (destino == NULL) return;

    Conta *contaDestino = selecionarConta(destino);
    if (contaDestino == NULL) return;

    if (contaOrigem == contaDestino) {
        printf("Origem e destino devem ser diferentes.\n");
        return;
    }

    double valor = lerValor("Valor da transferencia: R$ ");

    if (valor <= 0 || valor > contaOrigem->saldo) {
        printf("Valor invalido ou saldo insuficiente.\n");
        return;
    }

    contaOrigem->saldo -= valor;
    contaDestino->saldo += valor;

    char texto[100];
// Montar e guardar um texto formatado dentro de uma variável (string)
    snprintf(texto, sizeof(texto), 
             "Transferencia enviada: R$ %.2f", valor);
    registrar(contaOrigem, texto);

    snprintf(texto, sizeof(texto),
             "Transferencia recebida: R$ %.2f", valor);
    registrar(contaDestino, texto);

    printf("Transferencia realizada com sucesso!\n");
}

// Consulta saldo
void consultarSaldo(Banco *banco) {
    Cliente *cliente = buscarCliente(banco);
    if (cliente == NULL) return;

    printf("\nCliente: %s\n", cliente->nome);

    for (int i = 0; i < 3; i++) {
        if (cliente->possui[i]) {
            Conta *conta = &cliente->contas[i]; //  ponteiro de atalho apontando diretamente para o endereço de memória

            printf("Conta %04d-%02d | Saldo: R$ %.2f\n",
                   conta->numero, conta->tipo, conta->saldo);
        }
    }

    printf("Saldo total: R$ %.2f\n", saldoTotal(cliente));
}

// Lista clientes e contas
void listarClientes(Banco *banco) {
    printf("\n===== CLIENTES =====\n");

    if (banco->quantidade == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    for (int i = 0; i < banco->quantidade; i++) {
        Cliente *cliente = &banco->clientes[i];

        printf("\n%s\n", cliente->nome);

        for (int j = 0; j < 3; j++) {
            if (cliente->possui[j]) {
                Conta *conta = &cliente->contas[j];

                printf("  %04d-%02d | R$ %.2f\n",
                       conta->numero, conta->tipo,
                       conta->saldo);
            }
        }
    }
}

// Emprestimo: 25% de 10 vezes o saldo total
void solicitarEmprestimo(Banco *banco) {
    Cliente *cliente = buscarCliente(banco);
    if (cliente == NULL) return;

    if (cliente->emprestimoFeito) {
        printf("Cliente ja realizou um emprestimo.\n");
        return;
    }
// contas[2] é um elemento direto do array, não um ponteiro.
    
    if (!cliente->possui[0] || !cliente->possui[2] ||
        cliente->contas[2].saldo <= 0) {
        printf("Necessario ter conta corrente e "
               "poupanca com saldo positivo.\n");
        return;
    }

    double limite = saldoTotal(cliente) * 10 * 0.25;

//se o cliente não tiver saldo suficiente para gerar limite
    if (limite <= 0) {
        printf("Limite indisponivel.\n");
        return;
    }

    printf("Limite disponivel: R$ %.2f\n", limite);

    double valor = lerValor("Valor solicitado: R$ ");

    if (valor <= 0 || valor > limite) {
        printf("Valor fora do limite.\n");
        return;
    }

    Conta *corrente = &cliente->contas[0];
    corrente->saldo += valor;
    cliente->emprestimoFeito = 1; 
//1 significa verdadeiro)
    char texto[100];
    snprintf(texto, sizeof(texto),
             "Emprestimo de R$ %.2f", valor);
    registrar(corrente, texto);

    printf("Emprestimo concedido!\n");
}

// Compra titulo de capitalizacao
void comprarTitulo(Banco *banco) {
    Cliente *cliente = buscarCliente(banco);
    if (cliente == NULL) return;

    if (cliente->titulo) {
        printf("Cliente ja possui titulo.\n");
        return;
    }

    if (!cliente->possui[1]) {
        printf("Necessaria conta salario.\n");
        return;
    }

    Conta *conta = &cliente->contas[1];

    if (conta->saldo < 1000 || conta->saldo > 1500) {
        printf("Saldo deve estar entre R$ 1000 e R$ 1500.\n");
        return;
    }
//  cliente agora possui um título (1).
    cliente->titulo = 1;
    registrar(conta, "Aquisicao de titulo");

    printf("Titulo adquirido com sucesso!\n");
}

// Exibe alertas de saldo negativo
void alertas(Banco *banco) {
    for (int i = 0; i < banco->quantidade; i++) {
        Cliente *cliente = &banco->clientes[i];

        if (cliente->possui[0] &&
            cliente->contas[0].saldo < 0) {
            printf("ALERTA: %s possui saldo negativo: "
                   "R$ %.2f\n",
                   cliente->nome,
                   cliente->contas[0].saldo);
        }
    }
}

// Exibe o historico e os relatorios
void consultarHistorico(Banco *banco) {
    if (!banco->aberto) {
        printf("Abra o banco primeiro.\n");
        return;
    }
//calcula e exibe o saldo somado de todos os clientes de um banco.
    double total = 0;

    for (int i = 0; i < banco->quantidade; i++)
        total += saldoTotal(&banco->clientes[i]);

    printf("\n== SALDO TOTAL DO BANCO ==\n");
    printf("%d clientes | R$ %.2f\n",
           banco->quantidade, total);

    printf("\n== CLIENTES APTOS A EMPRESTIMOS ==\n");

    for (int i = 0; i < banco->quantidade; i++) {
        Cliente *c = &banco->clientes[i];

        if (c->possui[0] && c->possui[2] &&
            c->contas[2].saldo > 0 && !c->emprestimoFeito &&
            saldoTotal(c) > 0) {
            printf("%s | %04d-01 | R$ %.2f\n",
                   c->nome, c->contas[0].numero,
                   saldoTotal(c) * 2.5);
        }
    }

    printf("\n== CONTAS COM SALDO NEGATIVO ==\n");
//Número base da conta
    for (int i = 0; i < banco->quantidade; i++) {
        Cliente *c = &banco->clientes[i];
        for (int j = 0; j < 3; j++) {
            if (c->possui[j] && c->contas[j].saldo < 0) {
                printf("%s | %04d-%02d | R$ %.2f\n",
                       c->nome, c->contas[j].numero,
                       c->contas[j].tipo,
                       c->contas[j].saldo);
            }
        }
    }

    printf("\n== CLIENTES APTOS A TITULOS ==\n");

    for (int i = 0; i < banco->quantidade; i++) {
        Cliente *c = &banco->clientes[i];

        if (c->possui[1] && !c->titulo &&
            c->contas[1].saldo >= 1000 &&
            c->contas[1].saldo <= 1500) {
            printf("%s | %04d-02 | R$ %.2f\n",
                   c->nome, c->contas[1].numero,
                   c->contas[1].saldo);
        }
    }
//Caso tenha recebido, exibimos seus dados e utilizamos o break para evitar informações duplicadas.
    printf("\n== CONTEMPLADOS NO SORTEIO ==\n");

    for (int i = 0; i < banco->quantidade; i++) {
        Cliente *c = &banco->clientes[i];

        if (c->premio > 0) {
            for (int j = 0; j < 3; j++) {
                if (c->possui[j]) {
                    printf("%s | %04d-%02d | R$ %.2f\n",
                           c->nome, c->contas[j].numero,
                           c->contas[j].tipo, c->premio);
                    break;
                }
            }
        }
    }

    printf("\n== MOVIMENTACOES ==\n");

    for (int i = 0; i < banco->quantidade; i++) {
        Cliente *c = &banco->clientes[i];

        for (int j = 0; j < 3; j++) {
            if (!c->possui[j]) continue;

            Conta *conta = &c->contas[j];

            printf("\n%s | Conta %04d-%02d\n",
                   c->nome, conta->numero, conta->tipo);

            for (int k = 0; k < conta->qtdHistorico; k++)
                printf("%s\n", conta->historico[k]);
//k representa a posição de cada registro na matriz.
        }
    }
}

// Sorteio entre os cinco maiores saldos
void sortear(Banco *banco) {
    if (!banco->aberto) {
        printf("Abra o banco primeiro.\n");
        return;
    }

    if (banco->quantidade < 5) {
        printf("Necessarios pelo menos cinco clientes.\n");
        return;
    }

    int *indices = (int *) malloc(
        banco->quantidade * sizeof(int)
    );

    if (indices == NULL) {
        printf("Erro de memoria no sorteio.\n");
        return;
    }

    for (int i = 0; i < banco->quantidade; i++)
        indices[i] = i; 
       // preencher o vetor indices com as posições dos clientes cadastrados.

    // Ordena os clientes pelo saldo total
    for (int i = 0; i < banco->quantidade - 1; i++) {
        for (int j = i + 1; j < banco->quantidade; j++) {
            if (saldoTotal(&banco->clientes[indices[j]]) >
                saldoTotal(&banco->clientes[indices[i]])) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
  // para no penúltimo porque, quando todos os anteriores estiverem no lugar certo, o último já estará automaticamente ordenado.
            }
        }
    }

    // Todos os cinco maiores precisam ter titulo
    for (int i = 0; i < 5; i++) {
        if (!banco->clientes[indices[i]].titulo) {
            printf("Sorteio indisponivel: os cinco "
                   "maiores saldos precisam ter titulo.\n");
            free(indices);
            return;
        }
    }

    // Fibonacci: gera um indice de 0 a 4
    unsigned long long a = 0, b = 1; // espaço na memória para não estourarem o limite da variável.

    int passos = (int)(time(NULL) % 20) + 1;

    for (int i = 0; i < passos; i++) {
        unsigned long long proximo = a + b;
        a = b;
        b = proximo;
    }

    int posicao = (int)(a % 5);

    Cliente *vencedor = &banco->clientes[indices[posicao]];

    // Credita o premio na conta salario
    Conta *conta = &vencedor->contas[1];

    double premio = conta->saldo * 10;

    conta->saldo += premio;
    vencedor->premio += premio;

    char texto[100];
    snprintf(texto, sizeof(texto),
             "Premio do sorteio: R$ %.2f", premio);
    registrar(conta, texto);

    printf("\nVencedor: %s\n", vencedor->nome);
    printf("Premio: R$ %.2f\n", premio);

    free(indices);
}

// Menu de operacoes bancarias

void abrirBanco(Banco *banco) {
    banco->aberto = 1;

    int opcao;

    do {
        limparTela();

        alertas(banco);

        printf("\n===== BANCO ABERTO =====\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Criar conta\n");
        printf("3 - Depositar\n");
        printf("4 - Sacar\n");
        printf("5 - Transferir\n");
        printf("6 - Consultar saldo\n");
        printf("7 - Listar clientes\n");
        printf("8 - Solicitar emprestimo\n");
        printf("9 - Comprar titulo\n");
        printf("0 - Voltar\n");

        opcao = lerInteiro("Opcao: ");

        limparTela();

        switch (opcao) {
            case 1:
                cadastrarCliente(banco);
                break;

            case 2:
                criarConta(banco);
                break;

            case 3:
                depositar(banco);
                break;

            case 4:
                sacar(banco);
                break;

            case 5:
                transferir(banco);
                break;

            case 6:
                consultarSaldo(banco);
                break;

            case 7:
                listarClientes(banco);
                break;

            case 8:
                solicitarEmprestimo(banco);
                break;

            case 9:
                comprarTitulo(banco);
                break;

            case 0:
                printf("Voltando ao menu principal...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);
}
