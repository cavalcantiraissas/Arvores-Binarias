#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct NoAVL {
    int dado;
    struct NoAVL* esquerda;
    struct NoAVL* direita;
    int altura;
} NoAVL;

int rotacoes = 0;  // Variável global para contar rotações
int comparacoes = 0;  // Variável global para contar comparações

NoAVL* criarNoAVL(int dado) {
    NoAVL* novoNo = (NoAVL*)malloc(sizeof(NoAVL));
    novoNo->dado = dado;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->altura = 1;
    return novoNo;
}

int altura(NoAVL* no) {
    return no ? no->altura : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

NoAVL* rotacionarEsquerda(NoAVL* raiz) {
    rotacoes++;  // Incrementa o contador de rotações
    NoAVL* novoRaiz = raiz->direita;
    NoAVL* temp = novoRaiz->esquerda;
    novoRaiz->esquerda = raiz;
    raiz->direita = temp;
    raiz->altura = max(altura(raiz->esquerda), altura(raiz->direita)) + 1;
    novoRaiz->altura = max(altura(novoRaiz->esquerda), altura(novoRaiz->direita)) + 1;
    return novoRaiz;
}

NoAVL* rotacionarDireita(NoAVL* raiz) {
    rotacoes++;  // Incrementa o contador de rotações
    NoAVL* novoRaiz = raiz->esquerda;
    NoAVL* temp = novoRaiz->direita;
    novoRaiz->direita = raiz;
    raiz->esquerda = temp;
    raiz->altura = max(altura(raiz->esquerda), altura(raiz->direita)) + 1;
    novoRaiz->altura = max(altura(novoRaiz->esquerda), altura(novoRaiz->direita)) + 1;
    return novoRaiz;
}

int obterBalanceamento(NoAVL* no) {
    return no ? altura(no->esquerda) - altura(no->direita) : 0;
}

NoAVL* inserir(NoAVL* raiz, int dado) {
    if (raiz == NULL) {
        return criarNoAVL(dado);
    }
    if (dado < raiz->dado) {
        raiz->esquerda = inserir(raiz->esquerda, dado);
    } else if (dado > raiz->dado) {
        raiz->direita = inserir(raiz->direita, dado);
    } else {
        return raiz;
    }

    raiz->altura = max(altura(raiz->esquerda), altura(raiz->direita)) + 1;
    int balanceamento = obterBalanceamento(raiz);

    if (balanceamento > 1 && dado < raiz->esquerda->dado) {
        return rotacionarDireita(raiz);
    }
    if (balanceamento < -1 && dado > raiz->direita->dado) {
        return rotacionarEsquerda(raiz);
    }
    if (balanceamento > 1 && dado > raiz->esquerda->dado) {
        raiz->esquerda = rotacionarEsquerda(raiz->esquerda);
        return rotacionarDireita(raiz);
    }
    if (balanceamento < -1 && dado < raiz->direita->dado) {
        raiz->direita = rotacionarDireita(raiz->direita);
        return rotacionarEsquerda(raiz);
    }

    return raiz;
}

NoAVL* buscar(NoAVL* raiz, int dado, int* comparacoes) {
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

void percorrerEmOrdem(NoAVL* raiz) {
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

void buscarAleatorios(NoAVL* raiz, int* valores, int n, double* tempoTotal, int* comparacoesTotal) {
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
    NoAVL* raiz = NULL;

    int* valores;
    int n;
    lerValoresDoArquivo(nomeArquivo, &valores, &n);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    double tempoTotal;
    int comparacoesTotal;
    buscarAleatorios(raiz, valores, n, &tempoTotal, &comparacoesTotal);

    printf("Árvore AVL em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    printf("Número total de rotações: %d\n", rotacoes);
    printf("Número total de comparações: %d\n", comparacoesTotal);
    printf("Tempo total de busca: %f segundos\n", tempoTotal);

    free(valores);
    return 0;
}
