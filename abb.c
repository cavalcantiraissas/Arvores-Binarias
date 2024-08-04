#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct NoABB {
    int dado;
    struct NoABB* esquerda;
    struct NoABB* direita;
} NoABB;

int comparacoes = 0;

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
    } else {
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

void lerValoresDoArquivo(const char* nomeArquivo, int** valores, int* n) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int capacidade = 1000;
    *valores = (int*)malloc(capacidade * sizeof(int));
    *n = 0;

    int valor;
    while (fscanf(arquivo, "%d", &valor) != EOF) {
        if (*n >= capacidade) {
            capacidade *= 2;
            *valores = (int*)realloc(*valores, capacidade * sizeof(int));
        }
        (*valores)[(*n)++] = valor;
    }

    fclose(arquivo);
}

void buscarAleatorios(NoABB* raiz, int* valores, int n, double* tempoTotal, int* comparacoesTotal) {
    int numBuscas = n / 5;  // 20% dos valores
    int* valoresSelecionados = (int*)malloc(numBuscas * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < numBuscas; i++) {
        int index = rand() % n;
        valoresSelecionados[i] = valores[index];
    }

    *tempoTotal = 0.0;
    *comparacoesTotal = 0;
    for (int i = 0; i < numBuscas; i++) {
        int comparacoes = 0;
        clock_t inicio = clock();
        buscar(raiz, valoresSelecionados[i], &comparacoes);
        clock_t fim = clock();
        *tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
        *comparacoesTotal += comparacoes;
    }

    free(valoresSelecionados);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nome do arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* nomeArquivo = argv[1];
    NoABB* raiz = NULL;

    int* valores;
    int n;
    lerValoresDoArquivo(nomeArquivo, &valores, &n);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    double tempoTotal;
    int comparacoesTotal;
    buscarAleatorios(raiz, valores, n, &tempoTotal, &comparacoesTotal);

    printf("Árvore ABB em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    printf("Número total de comparações: %d\n", comparacoesTotal);
    printf("Tempo total de busca: %f segundos\n", tempoTotal);

    free(valores);
    return 0;
}
