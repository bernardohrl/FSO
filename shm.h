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
    strcat(shm, message);
    printf("\n\t\tMESSAGE ADDED TO SHM\n\n\n");
    return;
}

void delete_shm(int shmId) {
    msgctl(shmId, IPC_RMID, NULL);
}
