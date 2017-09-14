#include <sys/msg.h>
#include <string.h>

#define MSG_SIZE 100
#define KEYSRC "./Makefile"
#define FAILURE -1

struct my_msgbuf {
  long message_type;
  char message_text[200];
} buffer;


int send_message(char *message) {

    key_t key = ftok(KEYSRC, 'j');                        // Gera key para acessar fila
    int queueId = msgget(key, 0666 | IPC_CREAT);          // Gela fila com acesso apenas de leitura (0444)
    if(queueId == FAILURE) {
        printf("\n\n\t\tERROR: QUEUE NOT CREATED\n\n");
        return 0;
    }


    struct my_msgbuf message_send = {2};                                     //Numero abitrario
    strncpy(message_send.message_text, message, strlen(message));

    int result = msgsnd(queueId, &message_send, sizeof(message_send), 0);
    if(result == FAILURE)
        printf("\n\n\t\tERROR: MESSAGEM NOT SENT\n\n");

}

int recive_message() {

    key_t key = ftok(KEYSRC, 'j');
    int queueId = msgget(key, 0666 | IPC_CREAT);
    struct my_msgbuf message_recived;

    msgrcv(queueId, &message_recived, sizeof(message_recived), 2, 0);

    printf("mensagem: %s\n", message_recived.message_text);

}
