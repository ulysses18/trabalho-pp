#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int ret, rank, size, i, tag;
    int a = 0;
    double tempo;
    MPI_Status status;

    char *texto_para_o_buffer;

    ret = MPI_Init(&argc, &argv);
    ret = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ret = MPI_Comm_size(MPI_COMM_WORLD, &size);
    // tempo = MPI_Wtime();

    char *filename = "entrada.txt";
    char *seq = "CARECA";
    char *str;

    // rank = 0;
    if (rank == 0) {
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            exit(EXIT_FAILURE);
        }

        fseek(fp, 0L, SEEK_END);
        int fileSize = ftell(fp);
        rewind(fp);

        str = (char*) malloc(fileSize + 1);
        if (fgets(str, fileSize + 1, fp) == NULL) {
            printf("Erro ao ler o arquivo.\n");
            exit(EXIT_FAILURE);
        }
        if (str[strlen(str) - 1] == '\n') {
            str[strlen(str) - 1] = '\0';
        }

        int divisao = strlen(str) / 3;
        int resto = strlen(str) % 3;
        texto_para_o_buffer = (char*) malloc(divisao + resto);

        rewind(fp);
        int i = 2;
    
        fgets(texto_para_o_buffer, divisao+1, fp);
        printf(">>destino %d = %s\n", i, texto_para_o_buffer);
        ret = MPI_Send(&texto_para_o_buffer, 14, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        
        // printf(">>3 = %s\n", texto_para_o_buffer);

        // count = countOccurrences(str, seq);

        // ret = MPI_Send(&texto_para_o_buffer, strlen(texto_para_o_buffer), MPI_INT, 1, tag, MPI_COMM_WORLD);
        // ret = MPI_Recv(&a, 1, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);

        fclose(fp);
    } else if (rank == 2) {
        printf("\nrank: %d", rank);
        // texto_para_o_buffer = (char*) malloc(14);
        // ret = MPI_Recv(&texto_para_o_buffer, 14, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

        // printf("escravo: %d -> tam: 14 -> conteudo: %s\n", rank, texto_para_o_buffer);
    }

    // printf("O número de ocorrências de '%s' em '%s' é: %d\n", seq, str, count);
    free(str);

    return 0;
}
