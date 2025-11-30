#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------------------------------------------
// Estrutura da sala (nó da árvore binária)
// -------------------------------------------------------------
typedef struct sala {
    char nome[50];
    struct sala* esquerda;
    struct sala* direita;
} Sala;

// -------------------------------------------------------------
// criarSala()
// Aloca dinamicamente uma sala e define seu nome.
// -------------------------------------------------------------
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// -------------------------------------------------------------
// explorarSalas()
// Permite que o jogador navegue pela mansão usando e/d/s.
// Mostra o nome de cada sala visitada.
// A exploração termina quando chegar a um nó-folha.
// -------------------------------------------------------------
void explorarSalas(Sala* atual) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Nó-folha → sem caminhos
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou ao fim do caminho. Exploração encerrada.\n");
            return;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf(" (e) Ir para a esquerda\n");
        if (atual->direita != NULL) printf(" (d) Ir para a direita\n");
        printf(" (s) Sair da exploração\n> ");

        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda.\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há sala à direita.\n");
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("Exploração encerrada pelo jogador.\n");
            return;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

// -------------------------------------------------------------
// main()
// Monta a árvore binária da mansão e inicia a exploração.
// -------------------------------------------------------------
int main() {
    // Construção da mansão (árvore fixa)

    // Raiz (Hall)
    Sala* hall = criarSala("Hall de Entrada");

    // Segundo nível
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita  = criarSala("Cozinha");

    // Terceiro nível — descendentes da Sala de Estar
    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita  = criarSala("Jardim Interno");

    // Terceiro nível — descendentes da Cozinha
    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita  = criarSala("Porão");

    // Início da exploração
    explorarSalas(hall);

    return 0;
}
