#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>

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


    key_t key = ftok("./Makefile", 'j');         // Gera key para acessar fila
    int queueId = msgget(key, 0666 | IPC_CREAT);          // Gela fila com acesso apenas de leitura (0444)
    if(queueId == -1) {
        printf("\n\n\tERROR: QUEUE NOT CREATED\n\n");
        return 0;
    }

    struct my_msgbuf message_send = {2};                                     //Numero abitrario
    strncpy(message_send.message_text, message, sizeof(message)-1);

    int result = msgsnd(queueId, &message_send, sizeof(message_send), 0);
    printf("\n%d\n", result);
    printf("id: %d\n", queueId);
    // printf("id: %d\n", queueId);

    // printf("mensagem: %s\n %ld\n", message_struct.message_text, message_struct.message_type);

  } else {
    // Espera o processo filho terminar (sincronização)
    waitpid(process_son, NULL, 0);
    transport_header();

    key_t key = ftok("./Makefile", 'j');
    int queueId = msgget(key, 0666 | IPC_CREAT);
    struct my_msgbuf message_recived;

    msgrcv(queueId, &message_recived, sizeof(message_recived), 2, 0);

    printf("mensagem: %s\n", message_recived.message_text);
    
  }

  return 0;

}
