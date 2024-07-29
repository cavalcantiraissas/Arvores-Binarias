# Projeto de Estruturas de Dados

## Descrição

Este projeto visa implementar e analisar diferentes tipos de árvores binárias, incluindo Árvore Binária de Busca (Clássica), Árvore AVL e Árvore Rubro-Negra. A análise foca na eficiência de inserção e busca, medindo altura das árvores, número de rotações e tempo de busca.

## Etapas do Projeto

### Etapa 1: Geração de Arquivo com Valores Aleatórios

**Objetivo:** Criar um arquivo contendo números inteiros aleatórios, sem repetição, no intervalo de 1 a 100.000.
- O algoritmo deve receber dois parâmetros de entrada: o número de valores a serem gerados e o nome do arquivo de saída.
- Os números gerados devem ser salvos no arquivo de saída, com um valor por linha.

### Etapa 2: Implementação de Árvores de Busca

**Objetivo:**  Implementar diferentes tipos de árvores de busca e funções associadas.
### Árvores a serem implementadas:
- Árvore Binária de Busca (Clássica)
- Árvore AVL (Árvore Balanceada)
- Árvore Rubro-Negra

### Funções a serem implementadas:
- Inserção de valores
- Busca de valores
- Função para calcular a altura da árvore
- Contagem do número de comparações durante a busca
- Contagem do número de rotações durante a inserção (para árvores balanceadas)

### Etapa 3: Análise de Desempenho das Árvores

**Objetivo:** Comparar o desempenho das diferentes árvores implementadas usando conjuntos de dados específicos.
- Gerar 3 arquivos de entrada com 5.000 números e 3 arquivos com 20.000 números.
- Carregar cada arquivo em cada uma das árvores (Binária Clássica, AVL, Rubro-Negra) e registrar:
- A altura da árvore resultante após todas as inserções.
- O número total de rotações realizadas (para árvores AVL e Rubro-Negra).
- Comparar as diferenças de altura e o número de rotações entre os diferentes tipos de árvores.

### Etapa 4: Avaliação de Busca

**Objetivo:** Avaliar a eficiência das árvores na busca de valores.
- Selecionar aleatoriamente 20% dos valores gerados nos arquivos da Etapa 3.
- Para cada valor selecionado, realizar a busca em cada tipo de árvore.
- Calcular o tempo de busca e o número de comparações realizadas para encontrar cada valor.
