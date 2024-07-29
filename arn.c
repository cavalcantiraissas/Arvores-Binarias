#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VERMELHO 0
#define PRETO 1

typedef struct NoRubroNegro {
    int dado;
    int cor;
    struct NoRubroNegro* esquerda;
    struct NoRubroNegro* direita;
    struct NoRubroNegro* pai;
} NoRubroNegro;

NoRubroNegro* criarNoRubroNegro(int dado) {
    NoRubroNegro* novoNo = (NoRubroNegro*)malloc(sizeof(NoRubroNegro));
    novoNo->dado = dado;
    novoNo->cor = VERMELHO; 
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->pai = NULL;
    return novoNo;
}

NoRubroNegro* rotacionarEsquerda(NoRubroNegro* x) {
    NoRubroNegro* y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL) y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL) {
        // x é a raiz
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
    return y;
}

NoRubroNegro* rotacionarDireita(NoRubroNegro* y) {
    NoRubroNegro* x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != NULL) x->direita->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL) {
        // y é a raiz
    } else if (y == y->pai->esquerda) {
        y->pai->esquerda = x;
    } else {
        y->pai->direita = x;
    }
    x->direita = y;
    y->pai = x;
    return x;
}

void ajustarNoRubroNegro(NoRubroNegro* no) {
    while (no != NULL && no->pai != NULL && no->pai->cor == VERMELHO) {
        if (no->pai == no->pai->pai->esquerda) {
            NoRubroNegro* tio = no->pai->pai->direita;
            if (tio != NULL && tio->cor == VERMELHO) {
                no->pai->cor = PRETO;
                tio->cor = PRETO;
                no->pai->pai->cor = VERMELHO;
                no = no->pai->pai;
            } else {
                if (no == no->pai->direita) {
                    no = no->pai;
                    rotacionarEsquerda(no);
                }
                no->pai->cor = PRETO;
                no->pai->pai->cor = VERMELHO;
                rotacionarDireita(no->pai->pai);
            }
        } else {
            NoRubroNegro* tio = no->pai->pai->esquerda;
            if (tio != NULL && tio->cor == VERMELHO) {
                no->pai->cor = PRETO;
                tio->cor = PRETO;
                no->pai->pai->cor = VERMELHO;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    rotacionarDireita(no);
                }
                no->pai->cor = PRETO;
                no->pai->pai->cor = VERMELHO;
                rotacionarEsquerda(no->pai->pai);
            }
        }
    }
}

NoRubroNegro* inserir(NoRubroNegro* raiz, int dado) {
    NoRubroNegro* novoNo = criarNoRubroNegro(dado);
    NoRubroNegro* y = NULL;
    NoRubroNegro* x = raiz;
    while (x != NULL) {
        y = x;
        if (novoNo->dado < x->dado) {
            x = x->esquerda;
        } else {
            x = x->direita;
        }
    }
    novoNo->pai = y;
    if (y == NULL) {
        raiz = novoNo;
    } else if (novoNo->dado < y->dado) {
        y->esquerda = novoNo;
    } else {
        y->direita = novoNo;
    }
    ajustarNoRubroNegro(novoNo);
    raiz->cor = PRETO;
    return raiz;
}

NoRubroNegro* buscar(NoRubroNegro* raiz, int dado, int* comparacoes) {
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

void percorrerEmOrdem(NoRubroNegro* raiz) {
    if (raiz != NULL) {
        percorrerEmOrdem(raiz->esquerda);
        printf("%d ", raiz->dado);
        percorrerEmOrdem(raiz->direita);
    }
}

int main() {
    NoRubroNegro* raiz = NULL;
    int valores[] = {50, 30, 20, 40, 70, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    printf("Árvore Rubro-Negra em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    int valorParaBuscar = 40;
    int comparacoes = 0;
    clock_t inicio = clock();
    NoRubroNegro* resultado = buscar(raiz, valorParaBuscar, &comparacoes);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (resultado != NULL) {
        printf("Valor %d encontrado na árvore.\n", valorParaBuscar);
    } else {
        printf("Valor %d não encontrado na árvore.\n", valorParaBuscar);
    }
    printf("Tempo de busca: %f segundos\n", tempo);
    printf("Número de comparações: %d\n", comparacoes);

    return 0;
}
