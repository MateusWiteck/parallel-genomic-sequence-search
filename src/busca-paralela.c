#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"
#include <omp.h>

#define SIZE 100 
#define MAX_LINHAS_arquivo1 1000
#define NOME_ARQ_SIZE   50
char sequencia_busca[MAX_LINHAS_arquivo1][SIZE];
int main(int argc, char *argv[])
{
    char arq1[NOME_ARQ_SIZE];
    char arq2[NOME_ARQ_SIZE];
    long ocorrencias = 0;
    long arq2_size;
    long linha;
    long ctr_linha_arquivo2 = 0;
    char *conteudo_arquivo2;
    char **linhas_arquivo2;
    int *matches;
    long candidatos_ocorrencias;
    TIMER_CLEAR;

    FILE *f1;
    FILE *f2;

    // Abrir o arquivo da sequencia de cadeias buscada 
    if (argc == 3){
    	f1 = fopen(argv[1], "r");
    	f2 = fopen(argv[2], "r");
    }

    else{
    	printf("Digite o nome do arquivo 1(seqs a serem buscadas): ");
    	scanf("%s",&arq1);
    	printf("Digite o nome do arquivo 2 (em que as seqs serao buscadas):");
    	scanf("%s",&arq2);
    	f1 = fopen(arq1, "r");
    	f2 = fopen(arq2, "r");
    }

    // Verificacao do nome do arquivo
    if(f1 == NULL){
    	printf("Arquivo 1  nao encontrado\n");
    	exit(1);
    }
    if(f2 == NULL){
    	printf("Arquivo 2  nao encontrado\n");
    	exit(1);
    }


    // Leitura do arquivo 1 linha por linha e busca pela sequencia lida
    // Leitura do arquivo 2 linha por linha ao buscar a sequencia lida do arquivo 1
    TIMER_START;
printf("INICIO\n");
fflush(stdout);
    fseek(f2,0L,SEEK_END);
    arq2_size=ftell(f2);
    fseek(f2,0L,SEEK_SET);
  // Leitura de uma linha de arq1
    linha=0;
    while (linha < MAX_LINHAS_arquivo1 &&
           fgets(sequencia_busca[linha], SIZE, f1) != NULL) {
      // Substituir CRLF por \0
      sequencia_busca[linha][strcspn(sequencia_busca[linha], "\r\n")] = '\0';
      linha++;
    }

    // Lê o arq2 e divide as linhas em linhas_arquivo2
    conteudo_arquivo2 = malloc((size_t)arq2_size + 1);
    if (conteudo_arquivo2 == NULL) {
      fprintf(stderr, "Nao foi possivel alocar memoria para o arquivo 2\n");
      fclose(f1);
      fclose(f2);
      return 1;
    }
    size_t bytes_read = fread(conteudo_arquivo2, 1, (size_t)arq2_size, f2);
    conteudo_arquivo2[bytes_read] = '\0';
    if (ferror(f2)) {
      fprintf(stderr, "Nao foi possivel ler o arquivo2\n");
      free(conteudo_arquivo2);
      fclose(f1);
      fclose(f2);
      return 1;
    }
    size_t line_capacity = 1; // qtd de linhas
    for (size_t i = 0; i < bytes_read; i++)
      if (conteudo_arquivo2[i] == '\n')
        line_capacity++;
    linhas_arquivo2 = malloc(line_capacity * sizeof(*linhas_arquivo2));
    if (linhas_arquivo2 == NULL) {
      fprintf(stderr, "Nao foi possivel alocar memoria para os ponteiros das linhas\n");
      free(conteudo_arquivo2);
      fclose(f1);
      fclose(f2);
      return 1;
    }
    if (bytes_read > 0) {
      linhas_arquivo2[ctr_linha_arquivo2] = conteudo_arquivo2; // Primeiro ponteiro
      ctr_linha_arquivo2++;
      for (size_t i = 0; i < bytes_read; i++) {
        if (conteudo_arquivo2[i] == '\n') {
          conteudo_arquivo2[i] = '\0';
          if (i > 0 && conteudo_arquivo2[i - 1] == '\r') // Substituir CRLF (windows) por \0
            conteudo_arquivo2[i - 1] = '\0';
          if (i + 1 < bytes_read) {
            linhas_arquivo2[ctr_linha_arquivo2] = &conteudo_arquivo2[i + 1]; // Ponteiro da proxima linha
            ctr_linha_arquivo2++;
          }
        }
      }
      if (conteudo_arquivo2[bytes_read - 1] == '\0' && linhas_arquivo2[ctr_linha_arquivo2 - 1][0] == '\0')
        ctr_linha_arquivo2--; // linha vazia no final
    }

    // Cria vetor de ocorrencias para marcar TRUE/FALSE para cada potencial match
    candidatos_ocorrencias = ctr_linha_arquivo2 - linha + 1;
    if (candidatos_ocorrencias > 0) {
      matches = calloc((size_t)candidatos_ocorrencias, sizeof(*matches));
      if (matches == NULL) {
        fprintf(stderr, "Nao foi possivel alocar as flags de match\n");
        free(linhas_arquivo2);
        free(conteudo_arquivo2);
        fclose(f1);
        fclose(f2);
        return 1;
      }

      // Cada iteração verifica uma possível linha inicial
      #pragma omp parallel for
      for (long start = 0; start < candidatos_ocorrencias; start++) {
        int match = 1;
        for (long j = 0; j < linha; j++) {
          if (strcmp(sequencia_busca[j], linhas_arquivo2[start + j]) != 0) {
            match = 0;
            break;
          }
        }
        matches[start] = match;
      }

      // Conta ocorrencias em order para tirar as sobreposições
      long start = 0;
      while (start < candidatos_ocorrencias) {
        if (matches[start]) {
          ocorrencias++;
          start += linha;
        } else {
          start++;
        }
      }
      free(matches);
    }
    free(linhas_arquivo2);
    free(conteudo_arquivo2);
    // Fecha os arquivos
    fclose(f1);
    fclose(f2);

    TIMER_STOP;
    printf("Tempo: %f \n", TIMER_ELAPSED);
    printf("=======================================\n");
    printf("Total de ocorrencias = %ld\n",ocorrencias);
    printf("=======================================\n");
    return 0;
    
}
