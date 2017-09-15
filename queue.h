#include <sys/msg.h>
#include <string.h>

#define FAILURE -1
#define MSG_SIZE 100


struct my_msgbuf {
  long message_type;
  char message_text[200];
} buffer;


int send_message(char *message, int queueId) {

    char messagePart[MSG_SIZE];

    int msgQt = (strlen(message) + (MSG_SIZE - 1))/MSG_SIZE;                  // -1 por causa do '\0'
    int position = 0;

    for(int i=0; i<msgQt; i++) {                                              // Manda quantas mensagens forem necessárias de acordo com o tamanho
        struct my_msgbuf message_send = {2};                                  // Numero abitrario
        memcpy( messagePart, &message[position],  (MSG_SIZE-1));

        messagePart[strlen(messagePart)] = '\0';

        strncpy(message_send.message_text, messagePart, strlen(messagePart));

        int result = msgsnd(queueId, &message_send, sizeof(message_send), 0);
        if(result == FAILURE) printf("\n\n\t\tERROR: MESSAGEM NOT SENT\n\n");


        position += (MSG_SIZE-1);
    }

    return 0;
}

int recive_message(int queueId) {

    struct my_msgbuf message_recived, *buf;
    buf = &message_recived;
    int result = 0;


    while(msgrcv(queueId, &message_recived, sizeof(message_recived), 2, IPC_NOWAIT) != FAILURE) {
        printf("\nmessage: %s\n", message_recived.message_text);
    }

    return 0;
}
