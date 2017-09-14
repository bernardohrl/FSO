#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "queue.h"

#define MSG_SIZE 100
#define FALSE 0
#define TRUE 1

void application_header();
void transport_header();
char* readMessage();


int main() {

  pid_t process_son;              // Processo filho

  // Cria um processo filho exatamente igual ao processo pai
  process_son = fork();

  if(process_son == FALSE) {
    application_header();
    printf("Insira a mensagem: ");
    char *message = readMessage();

    int msgQt = (strlen(message) + (MSG_SIZE - 1))/MSG_SIZE;                  // -1 por causa do '\0'
    int position = 0;
    char messagePart[MSG_SIZE];

    for(int i=0; i<msgQt; i++) {                                              // Manda quantas mensagens forem necessárias de acordo com o tamanho
        memcpy( messagePart, &message[position],  (MSG_SIZE-1));
        messagePart[MSG_SIZE-1] = '\0';
        position += (MSG_SIZE-1);
        printf("\n%s\n", messagePart);

        send_message(message);
        // printf("\nDebug: Erro aconteceu depois do retoro na função\n");    // Quando tem 208 caracteres não chega a passar aqui!!!
    }

    free(message);

  } else {
    // Espera o processo filho terminar (sincronização)
    waitpid(process_son, NULL, 0);
    transport_header();

    recive_message();

  }
  return 0;
}


// Cabeçalho da camada de aplicação do host A.
void application_header() {
  printf("\n*******************\n");
  printf("Camada de aplicação\n");
  printf("*******************\n");
}

// Cabeçalho da camada de transporte do host A.
void transport_header() {
  printf("\n********************\n");
  printf("Camada de transporte\n");
  printf("********************\n");
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
