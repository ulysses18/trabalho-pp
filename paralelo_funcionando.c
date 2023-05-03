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
    char *filename = argv[1];
    char *seq = "TACCGCTACGTCGTAGCTAGCTAGCTACGAGCGCTAGCGACGAGC";
    char *str;
    int count;
    double tempo = 0;
    tempo = MPI_Wtime();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    int fileSize = ftell(fp);
    rewind(fp);
    
    int blockSize = fileSize / size; 
    if (rank == size - 1) {
        blockSize += fileSize % size; 
    }
    str = (char*) malloc(blockSize + 1); 
    
    fseek(fp, rank * blockSize, SEEK_SET);
    if (fgets(str, blockSize + 1, fp) == NULL) {
        printf("Erro ao ler o arquivo.\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
    
    fclose(fp);
    
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
    count = countOccurrences(str, seq, size);
    tempo = MPI_Wtime() - tempo;
    printf("\ntempo do no %d: %f\n", rank, tempo);

    if (rank == 0) {
        printf("\nO número de ocorrências de '%s' é: %d\n", seq, count);
    }
    free(str);
    MPI_Finalize();
    return 0;
}