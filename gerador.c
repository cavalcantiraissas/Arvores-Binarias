#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para gerar números inteiros únicos aleatórios
void gerarNumerosUnicos(int* array, int tamanho) {
    int i, j, numero, unico;

    for (i = 0; i < tamanho; i++) {
        do {
            unico = 1;
            numero = rand() % 100000 + 1; 
            
            for (j = 0; j < i; j++) {
                if (array[j] == numero) {
                    unico = 0;
                    break;
                }
            }
        } while (!unico);
        array[i] = numero;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <número de valores> <nome do arquivo>\n", argv[0]);
        return 1;
    }

    int numeroDeValores = atoi(argv[1]);
    const char* nomeDoArquivo = argv[2];

    if (numeroDeValores <= 0) {
        printf("O número de valores deve ser maior que zero.\n");
        return 1;
    }

    int* numeros = (int*)malloc(numeroDeValores * sizeof(int));
    if (numeros == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    gerarNumerosUnicos(numeros, numeroDeValores);

    FILE* arquivo = fopen(nomeDoArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        free(numeros);
        return 1;
    }

    for (int i = 0; i < numeroDeValores; i++) {
        fprintf(arquivo, "%d\n", numeros[i]);
    }

    fclose(arquivo);
    free(numeros);

    printf("Arquivo '%s' gerado com %d números.\n", nomeDoArquivo, numeroDeValores);

    return 0;
}
