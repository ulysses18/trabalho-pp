#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countOccurrences(char *str, char *seq) {
    int count = 0;
    int seqLength = strlen(seq);
    int strLength = strlen(str);

    for (int i = 0; i <= strLength - seqLength; i++) {
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

    return count;
}

int main() {
    char *filename = "entrada.txt";
    char *seq = "TACCGCTACGTCGTAGCTAGCTAGCTACGAGCGCTAGCGACGAGC";
    char *str;
    int count;

    // Abre o arquivo em modo de leitura
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Obtém o tamanho do arquivo
    fseek(fp, 0L, SEEK_END);
    int fileSize = ftell(fp);
    rewind(fp);

    // Aloca memória para armazenar o conteúdo do arquivo
    str = (char*) malloc(fileSize + 1);

    // Lê o conteúdo do arquivo e armazena na variável str
    if (fgets(str, fileSize + 1, fp) == NULL) {
        printf("Erro ao ler o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Fecha o arquivo
    fclose(fp);

    // Remove o caractere de quebra de linha da string, se existir
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }

    // Chama a função countOccurrences para contar as ocorrências da sequência
    count = countOccurrences(str, seq);

    // Imprime o resultado
    printf("O número de ocorrências de '%s' é: %d\n", seq, count);

    // Libera a memória alocada
    free(str);

    return 0;
}
