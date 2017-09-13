#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>

// Define a fila de mensagens
#define QUEUE "/my_queue"
#define MSG_SIZE 100

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

// Abre a fila de mensagem para enviar ou receber mensagens dependendo da flag
mqd_t open_queue(int flag) {
  struct mq_attr attribute;   // Atributos da fila de mensagem
  mqd_t queue;                // Descritor da fila

  // Define os atributos da fila de mensagem
  attribute.mq_msgsize = MSG_SIZE; // Tamanho de cada mensagem

  // Abre ou cria a fila para receber mensagem, caso ela não existir
  if((queue = mq_open(QUEUE, flag, &attribute)) < 0) {
    perror("mq_open");
    exit(1);
  }

  return queue;

}

int main() {

  pid_t process_son;              // Processo filho
  mqd_t queue;                    // Descritor da fila
  char message[MSG_SIZE];         // Mensagem a enviar
  char msg_buffer[MSG_SIZE + 1];  // Buffer para receber a mensagem

  // Cria um processo filho exatamente igual ao processo pai
  process_son = fork();

  if(process_son == 0) {

    application_header();
    printf("Insira a mensagem: ");
    fgets(message, MSG_SIZE, stdin);
    queue = open_queue(O_WRONLY);

    // Enviar mensagem
    if(mq_send(queue, message, MSG_SIZE, 0) < 0) {
      perror("mq_send");
      exit(1);
    }

    printf("Mensagem enviada: %s\n", message);

  } else {
    // Espera o processo filho terminar (sincronização)
    waitpid(process_son, NULL, 0);

    transport_header();
    queue = open_queue(O_RDONLY|O_CREAT);

    // Receber cada mensagem e imprime seu conteúdo
    if((mq_receive(queue, msg_buffer, MSG_SIZE, 0)) < 0) {
      perror("mq_receive");
      exit(1);
    }

    printf("Mensagem recebida: %s\n", msg_buffer);
  }

  return 0;

}
