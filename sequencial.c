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
    fclose(fp);

    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }

    count = countOccurrences(str, seq);
    printf("O número de ocorrências de '%s' é: %d\n", seq, count);

    free(str);
    return 0;
}
