#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MSG_SIZE 100
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

struct my_msgbuf {
  long message_type;
  char message_text[200];
} buffer;

int main() {

  pid_t process_son;              // Processo filho
  char message[MSG_SIZE];         // Mensagem a enviar

  // Cria um processo filho exatamente igual ao processo pai
  process_son = fork();

  if(process_son == FALSE) {

    application_header();
    printf("Insira a mensagem: ");
    fgets(message, MSG_SIZE, stdin);

    printf("%s", message);

  } else {
    // Espera o processo filho terminar (sincronização)
    waitpid(process_son, NULL, 0);

    transport_header();

  }

  return 0;

}
