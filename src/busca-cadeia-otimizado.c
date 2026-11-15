#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>
#include "mede_time.h"

#define SIZE 100 
#define MAX_LINHAS_arquivo1 1000
#define NOME_ARQ_SIZE   50
#define TRUE 1 
#define FALSE 0
    FILE *f1;
    FILE *f2;
char sequencia_busca[MAX_LINHAS_arquivo1][SIZE];
const char *arquivo2_mapeado;
long arq2_size;
int busca_linha(long *i_seq,long i_linha_seq_busca)
{
  char buff[SIZE];
  int achou_linha,i;
  int size_seq;
  int size_sequencia_busca;
  size_sequencia_busca=strlen(sequencia_busca[i_linha_seq_busca]);
  // Copia linha atual
  int bytes_lidos = 0;
  while (*i_seq + bytes_lidos < arq2_size && bytes_lidos < SIZE - 1) {
    char c = arquivo2_mapeado[*i_seq + bytes_lidos];
    buff[bytes_lidos++] = c;
    if (c == '\n')
      break;
  }
  buff[bytes_lidos] = '\0';
  // Comparacao da sequencia na linha em arq2 com a sequencia em arq1
  size_seq=strlen(buff);
  *i_seq=*i_seq+size_seq;
  achou_linha=FALSE;
  if (size_sequencia_busca == size_seq){
       achou_linha = TRUE;
       for(i = 0; i < (size_sequencia_busca-1); i++){
	    if(sequencia_busca[i_linha_seq_busca][i] != buff[i]){
	         i++;
                 achou_linha = FALSE;
	    	 break;
            }
        }
   }
   return(achou_linha);
} 
int main(int argc, char *argv[])
{
    char arq1[NOME_ARQ_SIZE];
    char arq2[NOME_ARQ_SIZE];
    long ocorrencias;
    long i_seq;
    int achou;
    int primeira_seq;
    long i_seq_segunda;
    long linha,linha_seq_busca;
    TIMER_CLEAR;

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
    // Obtem o tamanho e mapeia arquivo2 sem copiar o arquivo inteiro
    if (fseek(f2, 0L, SEEK_END) != 0 || (arq2_size = ftell(f2)) < 0) {
      perror("Tamanho do arquivo2");
      fclose(f1);
      fclose(f2);
      return 1;
    }
    arquivo2_mapeado = NULL;
    if (arq2_size > 0) {
      arquivo2_mapeado = mmap(NULL, (size_t)arq2_size, PROT_READ,
                             MAP_PRIVATE, fileno(f2), 0);
      if (arquivo2_mapeado == MAP_FAILED) {
        perror("mmap");
        fclose(f1);
        fclose(f2);
        return 1;
      }
    }

  // Leitura de uma linha de arq1
    linha=0;
    while (linha < MAX_LINHAS_arquivo1 &&
           fgets(sequencia_busca[linha], SIZE, f1) != NULL) {
      linha++;
    }
    if (linha == 0 || (linha == MAX_LINHAS_arquivo1 && fgetc(f1) != EOF)) {
      fprintf(stderr, "Padrao vazio ou acima do limite de linhas\n");
      if (arq2_size > 0)
        munmap((void *)arquivo2_mapeado, (size_t)arq2_size);
      fclose(f1);
      fclose(f2);
      return 1;
    }
    ocorrencias=0;
    i_seq=0;
    while(i_seq< arq2_size){
        achou=TRUE;
        linha_seq_busca=0;
        primeira_seq=FALSE;
        while((linha_seq_busca< linha)&& (i_seq<arq2_size)){
          
           if ((achou=busca_linha(&i_seq,linha_seq_busca))==FALSE){
         
               break;
}
           else if (primeira_seq==FALSE) {
               primeira_seq=TRUE;
               i_seq_segunda=i_seq;
           } 
           linha_seq_busca++;
        }
        if ((achou==TRUE)&& (linha_seq_busca>linha-1)) {
          ocorrencias++;
//        printf("achou %d   ocorrencias %d\n",achou,ocorrencias);
        }
        else if (primeira_seq==TRUE) {
               i_seq=i_seq_segunda;
              }
    }
    if (arq2_size > 0)
      munmap((void *)arquivo2_mapeado, (size_t)arq2_size);
    // Fecha os arquivos
    fclose(f1);
    fclose(f2);

    TIMER_STOP;
    printf("Tempo: %f \n", TIMER_ELAPSED);
    printf("=======================================\n");
    printf("Total de ocorrencias = %d\n",ocorrencias);
    printf("=======================================\n");
    return 0;
    
}
