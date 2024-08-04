#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct NoABB {
    int dado;
    struct NoABB* esquerda;
    struct NoABB* direita;
} NoABB;

NoABB* criarNoABB(int dado) {
    NoABB* novoNo = (NoABB*)malloc(sizeof(NoABB));
    novoNo->dado = dado;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

NoABB* inserir(NoABB* raiz, int dado) {
    if (raiz == NULL) {
        return criarNoABB(dado);
    }
    if (dado < raiz->dado) {
        raiz->esquerda = inserir(raiz->esquerda, dado);
    } else if (dado > raiz->dado) {
        raiz->direita = inserir(raiz->direita, dado);
    }
    return raiz;
}

NoABB* buscar(NoABB* raiz, int dado, int* comparacoes) {
    (*comparacoes)++;
    if (raiz == NULL || raiz->dado == dado) {
        return raiz;
    }
    if (dado < raiz->dado) {
        return buscar(raiz->esquerda, dado, comparacoes);
    } else {
        return buscar(raiz->direita, dado, comparacoes);
    }
}

void percorrerEmOrdem(NoABB* raiz) {
    if (raiz != NULL) {
        percorrerEmOrdem(raiz->esquerda);
        printf("%d ", raiz->dado);
        percorrerEmOrdem(raiz->direita);
    }
}

void buscar20PorCento(NoABB* raiz, int* valores, int numValores) {
    int n = numValores * 0.20;
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int index = rand() % numValores;
        int valorParaBuscar = valores[index];
        int comparacoes = 0;
        clock_t inicio = clock();
        NoABB* resultado = buscar(raiz, valorParaBuscar, &comparacoes);
        clock_t fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (resultado != NULL) {
            printf("Valor %d encontrado na árvore.\n", valorParaBuscar);
        } else {
            printf("Valor %d não encontrado na árvore.\n", valorParaBuscar);
        }
        printf("Tempo de busca: %f segundos\n", tempo);
        printf("Número de comparações: %d\n", comparacoes);
    }
}

int main() {
    NoABB* raiz = NULL;
    int numValores = 5000;
    int* valores = (int*)malloc(numValores * sizeof(int));
    
    // Carregar os valores do arquivo
    FILE* arquivo = fopen("entrada_5000.txt", "r");
    for (int i = 0; i < numValores; i++) {
        fscanf(arquivo, "%d", &valores[i]);
        raiz = inserir(raiz, valores[i]);
    }
    fclose(arquivo);
    
    printf("Árvore Binária de Busca em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    buscar20PorCento(raiz, valores, numValores);

    free(valores);
    return 0;
}

