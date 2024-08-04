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

int rotacoes = 0;  
int comparacoes = 0;  

NoRubroNegro* criarNoRubroNegro(int dado) {
    NoRubroNegro* novoNo = (NoRubroNegro*)malloc(sizeof(NoRubroNegro));
    novoNo->dado = dado;
    novoNo->cor = VERMELHO;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->pai = NULL;
    return novoNo;
}

NoRubroNegro* rotacionarEsquerda(NoRubroNegro* raiz, NoRubroNegro* x) {
    rotacoes++; 
    NoRubroNegro* y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL) y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL) {
        raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
    return raiz;
}

NoRubroNegro* rotacionarDireita(NoRubroNegro* raiz, NoRubroNegro* y) {
    rotacoes++; 
    NoRubroNegro* x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != NULL) x->direita->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL) {
        raiz = x;
    } else if (y == y->pai->esquerda) {
        y->pai->esquerda = x;
    } else {
        y->pai->direita = x;
    }
    x->direita = y;
    y->pai = x;
    return raiz;
}

NoRubroNegro* ajustarNoRubroNegro(NoRubroNegro* raiz, NoRubroNegro* no) {
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
                    raiz = rotacionarEsquerda(raiz, no);
                }
                no->pai->cor = PRETO;
                no->pai->pai->cor = VERMELHO;
                raiz = rotacionarDireita(raiz, no->pai->pai);
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
                    raiz = rotacionarDireita(raiz, no);
                }
                no->pai->cor = PRETO;
                no->pai->pai->cor = VERMELHO;
                raiz = rotacionarEsquerda(raiz, no->pai->pai);
            }
        }
    }
    raiz->cor = PRETO;
    return raiz;
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
    raiz = ajustarNoRubroNegro(raiz, novoNo);
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
        printf("%d(%s) ", raiz->dado, raiz->cor == PRETO ? "PRETO" : "VERMELHO");
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

void buscarAleatorios(NoRubroNegro* raiz, int* valores, int n, double* tempoTotal, int* comparacoesTotal) {
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
    NoRubroNegro* raiz = NULL;

    int* valores;
    int n;
    lerValoresDoArquivo(nomeArquivo, &valores, &n);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    double tempoTotal;
    int comparacoesTotal;
    buscarAleatorios(raiz, valores, n, &tempoTotal, &comparacoesTotal);

    printf("Árvore Rubro-Negra em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    printf("Número total de rotações: %d\n", rotacoes);
    printf("Número total de comparações: %d\n", comparacoesTotal);
    printf("Tempo total de busca: %f segundos\n", tempoTotal);

    free(valores);
    return 0;
}
