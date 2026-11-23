
#include "meubanco.h"

int main(void) {
    Banco banco;
    int opcao;

    iniciarBanco(&banco);

    do {
        limparTela();

        printf("\n===== CARTEIRA DIGITAL =====\n");
        printf("1 - Abrir banco\n");
        printf("2 - Consultar historico\n");
        printf("3 - Efetuar sorteio\n");
        printf("4 - Sair\n");

        opcao = lerInteiro("Escolha uma opcao: ");

        limparTela();

        switch (opcao) {
            case 1:
                abrirBanco(&banco);
                break;

            case 2:
                consultarHistorico(&banco);
                break;

            case 3:
                sortear(&banco);
                break;

            case 4:
                printf("Encerrando o banco...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

        if (opcao != 1 && opcao != 4) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 4);

    liberarMemoria(&banco);

    return 0;
}
