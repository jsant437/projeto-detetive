#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 13  // tamanho da tabela hash (número pequeno por simplicidade)

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
// Nó da BST de pistas
// -------------------------------------------------------------
typedef struct nodePista {
    char pista[100];
    struct nodePista* esquerda;
    struct nodePista* direita;
} NodePista;

// -------------------------------------------------------------
// Estrutura da tabela hash
// Cada chave é uma pista → valor é o suspeito
// -------------------------------------------------------------
typedef struct entradaHash {
    char pista[100];
    char suspeito[50];
    struct entradaHash* prox;
} EntradaHash;

EntradaHash* tabelaHash[TAM_HASH];

// -------------------------------------------------------------
// Função de hash simples baseada na soma dos caracteres
// -------------------------------------------------------------
int hashFunction(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// -------------------------------------------------------------
// criarSala()
// -------------------------------------------------------------
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// -------------------------------------------------------------
// inserirPista() – insere na BST
// -------------------------------------------------------------
NodePista* inserirPista(NodePista* raiz, const char* pista) {
    if (raiz == NULL) {
        NodePista* novo = (NodePista*) malloc(sizeof(NodePista));
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
// inserirNaHash() – insere chave/valor na tabela hash
// -------------------------------------------------------------
void inserirNaHash(const char* pista, const char* suspeito) {
    int idx = hashFunction(pista);

    EntradaHash* nova = (EntradaHash*) malloc(sizeof(EntradaHash));
    strcpy(nova->pista, pista);
    strcpy(nova->suspeito, suspeito);
    nova->prox = tabelaHash[idx];
    tabelaHash[idx] = nova;
}

// -------------------------------------------------------------
// encontrarSuspeito()
// -------------------------------------------------------------
const char* encontrarSuspeito(const char* pista) {
    int idx = hashFunction(pista);

    EntradaHash* atual = tabelaHash[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// -------------------------------------------------------------
// exibirPistas() – mostra BST in-order
// -------------------------------------------------------------
void exibirPistas(NodePista* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// -------------------------------------------------------------
// contarPistasDoSuspeito() – percorre BST e conta quantas pistas
// apontam para determinado suspeito
// -------------------------------------------------------------
int contarPistasDoSuspeito(NodePista* raiz, const char* suspeito) {
    if (!raiz) return 0;

    int total = 0;
    const char* s = encontrarSuspeito(raiz->pista);
    if (s && strcmp(s, suspeito) == 0)
        total++;

    total += contarPistasDoSuspeito(raiz->esquerda, suspeito);
    total += contarPistasDoSuspeito(raiz->direita, suspeito);

    return total;
}

// -------------------------------------------------------------
// explorarSalas() – navegação e coleta de pistas
// -------------------------------------------------------------
void explorarSalas(Sala* atual, NodePista** arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda) printf(" (e) Esquerda\n");
        if (atual->direita)  printf(" (d) Direita\n");
        printf(" (s) Sair\n> ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita) atual = atual->direita;
        else if (opcao == 's') {
            printf("Exploração finalizada.\n");
            return;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

// -------------------------------------------------------------
// verificarSuspeitoFinal()
// -------------------------------------------------------------
void verificarSuspeitoFinal(NodePista* arvorePistas) {
    char suspeitoFinal[50];
    printf("\nDigite o nome do suspeito acusado:\n> ");
    scanf(" %[^\n]", suspeitoFinal);

    int qtd = contarPistasDoSuspeito(arvorePistas, suspeitoFinal);

    if (qtd >= 2)
        printf("\nAcusação consistente. %s é o(a) culpado(a).\n", suspeitoFinal);
    else
        printf("\nAcusação insuficiente. %s não pode ser considerado culpado(a).\n", suspeitoFinal);
}

// -------------------------------------------------------------
// main()
// -------------------------------------------------------------
int main() {
    // inicializa tabela hash
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    // monta mansão fixa
    Sala* hall = criarSala("Hall de Entrada", "Pegadas");
    hall->esquerda = criarSala("Sala de Estar", "Foto Rasgada");
    hall->direita  = criarSala("Cozinha", "Copo Quebrado");
    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro Fora do Lugar");
    hall->esquerda->direita  = criarSala("Jardim Interno", "");
    hall->direita->esquerda  = criarSala("Despensa", "Luva Suja");
    hall->direita->direita   = criarSala("Porão", "Cofre Aberto");

    // associa pistas a suspeitos na tabela hash
    inserirNaHash("Pegadas", "Carlos");
    inserirNaHash("Foto Rasgada", "Marina");
    inserirNaHash("Copo Quebrado", "Carlos");
    inserirNaHash("Livro Fora do Lugar", "Marina");
    inserirNaHash("Luva Suja", "Carlos");
    inserirNaHash("Cofre Aberto", "Rogério");

    NodePista* arvorePistas = NULL;

    explorarSalas(hall, &arvorePistas);

    printf("\nPistas coletadas:\n");
    exibirPistas(arvorePistas);

    verificarSuspeitoFinal(arvorePistas);

    return 0;
}
