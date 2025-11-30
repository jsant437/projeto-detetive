#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------------------------------------------
// Estrutura da sala (nó da árvore da mansão)
// -------------------------------------------------------------
typedef struct sala {
    char nome[50];
    char pista[100];
    struct sala* esquerda;
    struct sala* direita;
} Sala;

// -------------------------------------------------------------
// Estrutura do nó da árvore de pistas (BST)
// -------------------------------------------------------------
typedef struct pistaNode {
    char pista[100];
    struct pistaNode* esquerda;
    struct pistaNode* direita;
} PistaNode;

// -------------------------------------------------------------
// criarSala()
// Cria uma sala dinamicamente com nome e pista opcional.
// -------------------------------------------------------------
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// -------------------------------------------------------------
// inserirPista()
// Insere uma pista na BST comparando strings.
// -------------------------------------------------------------
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

// -------------------------------------------------------------
// explorarSalasComPistas()
// Permite navegação e coleta de pistas automaticamente.
// -------------------------------------------------------------
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda) printf(" (e) Ir para a esquerda\n");
        if (atual->direita)  printf(" (d) Ir para a direita\n");
        printf(" (s) Sair da exploração\n> ");

        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda.\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita)
                atual = atual->direita;
            else
                printf("Não há sala à direita.\n");
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("\nExploração encerrada.\n");
            return;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

// -------------------------------------------------------------
// exibirPistas()
// Imprime a BST em ordem alfabética (in-order).
// -------------------------------------------------------------
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// -------------------------------------------------------------
// main()
// Monta o mapa, inicia exploração e exibe as pistas coletadas.
// -------------------------------------------------------------
int main() {
    // Construção da mansão (árvore fixa)
    Sala* hall = criarSala("Hall de Entrada", "Pegadas misteriosas");

    hall->esquerda = criarSala("Sala de Estar", "Foto rasgada");
    hall->direita  = criarSala("Cozinha", "Xícara quebrada");

    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro fora do lugar");
    hall->esquerda->direita  = criarSala("Jardim Interno", NULL);

    hall->direita->esquerda = criarSala("Despensa", "Luva caída");
    hall->direita->direita  = criarSala("Porão", "Cofre aberto");

    PistaNode* arvorePistas = NULL;

    explorarSalasComPistas(hall, &arvorePistas);

    printf("\nPistas coletadas (ordenadas):\n");
    exibirPistas(arvorePistas);

    return 0;
}
