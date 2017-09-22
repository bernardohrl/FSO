#include <sys/shm.h>
#define ERROR -1


int get_shm_id() {
    key_t key = ftok("./Makefile", 'A');                                      // Deve ser a mesma para o server e o cliente
    int shmId = shmget(key, 3001, 0644 | IPC_CREAT);                        // SHM criada
    // printf("\n\nshmId: %d --- key: %d\n\n", shmId, key);
    if (shmId == ERROR) {
        printf("\n\n\t\tERROR: SHARED MEMORY NOT CREATED\n\n");
        exit(1) ;
    }

    return shmId;
}


char *attach_shm(int shmId) {
  char *shm;

  shm = shmat(shmId, 0, 0);
  if(shm == (char*) ERROR) {
    printf("\n\n\t\tERROR: SHARED MEMORY NOT ATTACHED\n\n");
    exit(1);
  }

  return shm;
}


void add_message_shm(char *shm, char *message) {
    if(strcmp(message, "\n") == 0) {
        printf("\n\tJUST LINEBREAKS ARE NOT ALLOWED\n\n\n");
        return;
    }

    strcat(shm, message);
    printf("\n\tMESSAGE ADDED TO SHM\n\n\n");
    return;
}

char *get_message_shm(char *shm) {

    //Pega indice que aparece primeiro '\n'
    char *substring;
    int index;
    substring = strchr(shm, '\n');
    index = (int)(substring - shm);

    // Pega mensagem antes do '\n' primeira mensagem e adiciona um '\n\0' no final
    char *message = (char*) malloc((index+1) * sizeof(char));
    strcpy(message, shm);
    strtok(message, "\n");
    message[strlen(message)] = '\0';


    // Copia shm retirando tudo antes do '\n' e depois retira do primeiro caracter ('\n')
    char *newShm = strstr(shm, "\n");
    memmove(newShm, newShm+1, strlen(newShm));
    strcpy(shm, newShm);

    return message;

}

void delete_shm(int shmId) {
    msgctl(shmId, IPC_RMID, NULL);
}
