#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "queue.h"
#include "shm.h"

#define KEY_SRC "./Makefile"
#define KEY_LETTER 'B'

int check_continue();

int main() {

    int queueId = get_queue_id(KEY_SRC, KEY_LETTER);
    int shmId = get_shm_id();
    char *shm = attach_shm(shmId);

    while(check_continue()) {
        int process_son = fork();
        if(process_son == 0) {

            char *message = get_message_shm(shm);
            send_message_server(message, queueId);

            exit(127);                                                    // Mata processo filho depois que a mensagem é adicionada
        }

        waitpid(process_son, NULL, 0);                                    // Espera finalização do processo filho para continuar

        char *recived = recive_message_server(queueId);
        if(strlen(recived) != 0)
            printf("\n\n\tMESSAGE RECIVED:   %s\n\n", recived);
        else
            printf("\n\n\n\tNO MESSAGE\n\n");

        getchar();
    }

    printf("\n\n\tDESCONNECTED\n\n\n");
    delete_queue(queueId);
    delete_shm(shmId);
    free(shm);

    return 0;
}

int check_continue() {
    char option;

    printf("\033c");
    printf("IF YOU WANT TO RECIVE MESSAGES TYPE 1");
    printf("\nIF YOU WANT TO LEAVE TYPE 0\n");
    printf("\n\tOPTION:\t");
    option = getchar();
    getchar();

    if(option == '0') return 0;
    if(option == '1') return 1;
    else return check_continue();

}
