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

    char *END = "exit\n";
    int continues = 1;

    int queueId = get_queue_id(KEY_SRC, KEY_LETTER);
    int shmId = get_shm_id();
    char *shm = attach_shm(shmId);


    application_header();
    do {

        printf("Insira a mensagem: ");
        char *message = readMessage();

        if(strcmp(message, END) != 0) {
            send_message(message, queueId);

            // Cria um processo filho para colocar mensagem em memoria
            pid_t process_son = fork();
            if(process_son == 0) {

                char *recived = recive_message(queueId);
                // printf("\nmensagem retornada: %s\n", recived);

                if(strcmp(recived, END) == 0)
                    continues = 0;
                else
                    add_message_shm(shm, recived);

            }

            waitpid(process_son, NULL, 0);

        }
        else continues = 0;

        printf("\n\nshm: %s\n", shm);

    } while(continues);

    // shmdt(shmId);
    delete_queue(queueId);

    return 0;

}


// Lê mensagem até o '\n' não se importando com o tamanho
char* readMessage() {
    char* input = NULL;
    char tempString[MSG_SIZE];
    size_t inputlen = 0, templen = 0;
    do {//
        fgets(tempString, MSG_SIZE, stdin);                                   // Lê parte do input
        templen = strlen(tempString);
        inputlen += templen;                                                  // Tamanho do input total soma com o que acabou de ser lido
        input = realloc(input, inputlen+1);                                   // Realoca o input total com total + lido agora (+1 por causa do '\0')
        strcat(input, tempString);
    } while (templen==MSG_SIZE-1 && tempString[MSG_SIZE-2]!='\n');            // Continua apenas se: a ultima parte lida for do tamanho determinado (-1 do '\0')
    // printf("\n\n\n\n\tMensagem retornada: \n%s\n", input);                 // e se o penultimo caracter não seja '\n' (o ultimo é \0).
    return input;
}
