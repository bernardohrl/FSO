#include <sys/msg.h>
#include <string.h>

#define ERROR -1
#define MSG_SIZE 100


struct message {
  long message_type;
  char message_text[200];
} buffer;
typedef struct message Message;



int get_queue_id(char *key_src, char key_letter) {
    key_t key = ftok(key_src, key_letter);                                    // Gera key para acessar fila
    int queueId = msgget(key, 0666 | IPC_CREAT);                              // Gela fila com acesso apenas de leitura (0444)
    if(queueId == ERROR) {
        printf("\n\n\t\tERROR: QUEUE NOT CREATED\n\n");
        exit(1);
    }
    return queueId;
}


int send_message(char *message, int queueId) {

    char messagePart[MSG_SIZE];

    int msgQt = (strlen(message) + (MSG_SIZE - 1))/MSG_SIZE;                  // -1 por causa do '\0'
    int position = 0;

    for(int i=0; i<msgQt; i++) {                                              // Manda quantas mensagens forem necessárias de acordo com o tamanho
        Message message_send = {2};                                           // Numero abitrario
        memcpy( messagePart, &message[position],  (MSG_SIZE-1));

        messagePart[strlen(messagePart)] = '\0';

        strncpy(message_send.message_text, messagePart, strlen(messagePart));

        int result = msgsnd(queueId, &message_send, sizeof(message_send), 0);
        if(result == ERROR) printf("\n\n\t\tERROR: MESSAGEM NOT SENT\n\n");


        position += (MSG_SIZE-1);
    }

    return 0;
}



char* recive_message(int queueId) {
    Message message_recived;
    char *str_to_ret = NULL; // = malloc (sizeof (char) * strlen(message_recived.message_text));
    int size = 0;

    while (msgrcv(queueId, &message_recived, sizeof(message_recived), 2, IPC_NOWAIT) != -1) {
        size += strlen(message_recived.message_text);

        str_to_ret = realloc(str_to_ret, size);
        strcat(str_to_ret, message_recived.message_text);

    }

    printf("nova string: %s\n", str_to_ret);
    return str_to_ret;
}



void delete_queue(int queueId) {
    msgctl(queueId, IPC_RMID, NULL);
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
