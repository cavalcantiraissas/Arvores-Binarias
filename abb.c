#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct No {
    int dado;
    struct No* esquerda;
    struct No* direita;
} No;

No* criarNo(int dado) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->dado = dado;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

No* inserir(No* raiz, int dado) {
    if (raiz == NULL) {
        return criarNo(dado);
    }
    if (dado < raiz->dado) {
        raiz->esquerda = inserir(raiz->esquerda, dado);
    } else if (dado > raiz->dado) {
        raiz->direita = inserir(raiz->direita, dado);
    }
    return raiz;
}

No* buscar(No* raiz, int dado, int* comparacoes) {
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

void percorrerEmOrdem(No* raiz) {
    if (raiz != NULL) {
        percorrerEmOrdem(raiz->esquerda);
        printf("%d ", raiz->dado);
        percorrerEmOrdem(raiz->direita);
    }
}

int altura(No* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    int alturaEsquerda = altura(raiz->esquerda);
    int alturaDireita = altura(raiz->direita);
    return (alturaEsquerda > alturaDireita ? alturaEsquerda : alturaDireita) + 1;
}

int main() {
    No* raiz = NULL;
    int valores[] = {50, 30, 20, 40, 70, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    printf("Árvore Binária de Busca em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    int valorParaBuscar = 42;
    int comparacoes = 0;
    clock_t inicio = clock();
    No* resultado = buscar(raiz, valorParaBuscar, &comparacoes);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (resultado != NULL) {
        printf("Valor %d encontrado na árvore.\n", valorParaBuscar);
    } else {
        printf("Valor %d não encontrado na árvore.\n", valorParaBuscar);
    }
    printf("Tempo de busca: %f segundos\n", tempo);
    printf("Número de comparações: %d\n", comparacoes);
    printf("Altura da árvore: %d\n", altura(raiz));

    return 0;
}
