#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "queue.h"
#include "shm.h"

#define KEY_SRC "./Makefile"
#define KEY_LETTER 'A'

void application_header();
void transport_header();
char* readMessage();

int main() {
  char *message;

  pid_t process_son;              // Processo filho
  int queueId = get_queue_id(KEY_SRC, KEY_LETTER);
  int shmId = get_shm_id();

  // Cria um processo filho exatamente igual ao processo pai
  process_son = fork();

  if(process_son == 0) {
    application_header();

    return 0;

  } else {
    // Espera o processo filho terminar (sincronização)
    waitpid(process_son, NULL, 0);

  }

  free(message);
  return 0;
}


// Lê mensagem até o '\n' não se importando com o tamanho
char* readMessage()
{
   char* input = NULL;
   char tempString[MSG_SIZE];
   size_t inputlen = 0, templen = 0;
   do {//
       fgets(tempString, MSG_SIZE, stdin);                                  // Lê parte do input
       templen = strlen(tempString);
       inputlen += templen;                                                  // Tamanho do input total soma com o que acabou de ser lido
       input = realloc(input, inputlen+1);                                   // Realoca o input total com total + lido agora (+1 por causa do '\0')
       strcat(input, tempString);
   } while (templen==MSG_SIZE-1 && tempString[MSG_SIZE-2]!='\n');          // Continua apenas se: a ultima parte lida for do tamanho determinado (-1 do '\0')
   // printf("\n\n\n\n\tMensagem retornada: \n%s\n", input);                 // e se o penultimo caracter não seja '\n' (o ultimo é \0).
   return input;
}
