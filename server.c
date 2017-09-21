#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "queue.h"
#include "shm.h"

#define KEY_SRC "./Makefile"
#define KEY_LETTER 'B'

void application_header();
void transport_header();

int main() {

    int continues = 1;

    int queueId = get_queue_id(KEY_SRC, KEY_LETTER);
    int shmId = get_shm_id();
    char *shm = attach_shm(shmId);


    // application_header();
    do {

        int process_son = fork();
        if(process_son == 0) {

            get_message_shm(shm);
            getchar();
            exit(127);                                                    // Mata processo filho depois que a mensagem é adicionada
        }

        waitpid(process_son, NULL, 0);                                    // Espera finalização do processo filho para continuar



    } while(continues);

    printf("\n\n\t\tDESCONNECTED\n\n\n");
    delete_queue(queueId);

    // printf("%s\n", shm);
    // printf("%d\n", shmId);


    return 0;

}
