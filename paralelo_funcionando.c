#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int countOccurrences(char *str, char *seq, int size) {
    int count = 0;
    int seqLength = strlen(seq);
    int strLength = strlen(str);
    int i = 0;

    for (i; i <= strLength - seqLength; i++) {
        int j;
        for (j = 0; j < seqLength; j++) {
            if (str[i + j] != seq[j]) {
                break;
            }
        }
        if (j == seqLength) {
            count++;
            j = 0;
        }
    }

    int totalCount = 0;
    MPI_Reduce(&count, &totalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return totalCount;
}

int main(int argc, char **argv) {
    int rank, size;
    char *filename = "entrada.txt";
    char *seq = "GCC";
    char *str;
    int count;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Abre o arquivo em modo de leitura
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    // Obtém o tamanho do arquivo
    fseek(fp, 0L, SEEK_END);
    int fileSize = ftell(fp);
    rewind(fp);

    // Aloca memória para armazenar a parte do arquivo correspondente a este processo
    int blockSize = fileSize / size; // pega qual vai ser o tamanho do bloco
    if (rank == size - 1) {
        blockSize += fileSize % size; // se for o mestre é o tamanho do bloco somado ao resto caso a divisao nao for exata
    }
    str = (char*) malloc(blockSize + 1); // aloca memoria de acordo com o tamanho bloco que o nó recebe

    // Lê a parte do arquivo correspondente a este processo
    fseek(fp, rank * blockSize, SEEK_SET);
    if (fgets(str, blockSize + 1, fp) == NULL) {
        printf("Erro ao ler o arquivo.\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    // Fecha o arquivo
    fclose(fp);

    // Remove o caractere de quebra de linha da string, se existir
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }

    // Chama a função countOccurrences para contar as ocorrências da sequência
    count = countOccurrences(str, seq, size);

    // Imprime o resultado no processo principal
    if (rank == 0) {
        printf("O número de ocorrências de '%s' é: %d\n", seq, count);
    }

    // Libera a memória alocada
    free(str);

    MPI_Finalize();
    return 0;
}