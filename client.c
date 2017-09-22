#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "queue.h"
#include "shm.h"

#define KEY_SRC "./Makefile"
#define KEY_LETTER 'A'

char* readMessage();

int main() {

    char *END = "exit\n";
    int continues = 1;

    int queueId = get_queue_id(KEY_SRC, KEY_LETTER);
    int shmId = get_shm_id();
    char *shm = attach_shm(shmId);


    while(check_continue()) {

        printf("\n\tINSERT MESSAGE: ");
        char *message = readMessage();

        send_message_client(message, queueId);

        // Cria um processo filho para colocar mensagem em memoria
        pid_t process_son = fork();
        if(process_son == 0) {

            char *recived = recive_message_client(queueId);
            // printf("\nmensagem retornada: %s\n", recived);
            add_message_shm(shm, recived);

            exit(127);                                                    // Mata processo filho depois que a mensagem é adicionada
        }

        waitpid(process_son, NULL, 0);                                    // Espera finalização do processo filho para continuar
        getchar();
    }


    printf("\n\n\tDESCONNECTED\n\n\n");
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

int check_continue() {
    char option;

    printf("\033c");
    printf("IF YOU WANT TO SEND MESSAGES TYPE 1");
    printf("\nIF YOU WANT TO LEAVE TYPE 0\n");
    printf("\n\tOPTION:\t");
    option = getchar();
    getchar();

    if(option == '0') return 0;
    if(option == '1') return 1;
    else return check_continue();

}
