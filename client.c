#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "queue.h"

#define FALSE 0
#define TRUE 1

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


int main() {

  pid_t process_son;              // Processo filho
  char message[MSG_SIZE];         // Mensagem a enviar

  // Cria um processo filho exatamente igual ao processo pai
  process_son = fork();

  if(process_son == FALSE) {
    application_header();
    printf("Insira a mensagem: ");
    fgets(message, 100, stdin);

    send_message(message);

  } else {
    // Espera o processo filho terminar (sincronização)
    waitpid(process_son, NULL, 0);
    transport_header();

    recive_message();

  }

  return 0;

}
