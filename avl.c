#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct NoAVL {
    int dado;
    struct NoAVL* esquerda;
    struct NoAVL* direita;
    int altura;
} NoAVL;

int altura(NoAVL* no) {
    return (no == NULL) ? 0 : no->altura;
}

NoAVL* criarNoAVL(int dado) {
    NoAVL* novoNo = (NoAVL*)malloc(sizeof(NoAVL));
    novoNo->dado = dado;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->altura = 1;
    return novoNo;
}

NoAVL* rotacionarDireita(NoAVL* y) {
    NoAVL* x = y->esquerda;
    NoAVL* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = 1 + (altura(y->esquerda) > altura(y->direita) ? altura(y->esquerda) : altura(y->direita));
    x->altura = 1 + (altura(x->esquerda) > altura(x->direita) ? altura(x->esquerda) : altura(x->direita));

    return x;
}

    NoAVL* rotacionarEsquerda(NoAVL* x) {
    NoAVL* y = x->direita;
    NoAVL* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = 1 + (altura(x->esquerda) > altura(x->direita) ? altura(x->esquerda) : altura(x->direita));
    y->altura = 1 + (altura(y->esquerda) > altura(y->direita) ? altura(y->esquerda) : altura(y->direita));

    return y;
}

int obterBalanceamento(NoAVL* no) {
    return (no == NULL) ? 0 : altura(no->esquerda) - altura(no->direita);
}

NoAVL* balancear(NoAVL* no) {
    int balanceamento = obterBalanceamento(no);

    if (balanceamento > 1 && no->dado > no->esquerda->dado) {
        return rotacionarDireita(no);
    }

    if (balanceamento < -1 && no->dado < no->direita->dado) {
        return rotacionarEsquerda(no);
    }

    if (balanceamento > 1 && no->dado < no->esquerda->dado) {
        no->esquerda = rotacionarEsquerda(no->esquerda);
        return rotacionarDireita(no);
    }

    if (balanceamento < -1 && no->dado > no->direita->dado) {
        no->direita = rotacionarDireita(no->direita);
        return rotacionarEsquerda(no);
    }

    return no;
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

    raiz->altura = 1 + (altura(raiz->esquerda) > altura(raiz->direita) ? altura(raiz->esquerda) : altura(raiz->direita));

    return balancear(raiz);
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

int main() {
    NoAVL* raiz = NULL;
    int valores[] = {50, 30, 20, 40, 70, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    printf("Árvore AVL em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    int valorParaBuscar = 40;
    int comparacoes = 0;
    clock_t inicio = clock();
    NoAVL* resultado = buscar(raiz, valorParaBuscar, &comparacoes);
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
