// sem_shm_proc1.c
// Compile: gcc -o sem_shm_proc1 sem_shm_proc1.c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

struct shm_data { 
    int multiple; 
    int counter1; 
};

int main() {
    key_t key = 0x4321;
    int shmid = shmget(key, sizeof(struct shm_data), IPC_CREAT | 0666);
    if (shmid < 0) { 
        perror("shmget"); 
        return 1; 
    }

    struct shm_data *sh = (struct shm_data*) shmat(shmid, NULL, 0);
    if (sh == (void*)-1) {
        perror("shmat"); 
        return 1; 
    }

    // create semaphore set with 1 semaphore
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget"); 
        return 1; 
    }
     
    // initialize semaphore to 1
    union semun { int val; } arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    // initialize shared values under lock
    semop(semid, &lock, 1);
    sh->multiple = 3;
    sh->counter1 = 0;
    semop(semid, &unlock, 1);

    printf("sem_shm_proc1 PID %d: initialized multiple=%d counter1=%d\n", getpid(), sh->multiple, sh->counter1);
    fflush(stdout);

    while (1) {
        semop(semid, &lock, 1);
        sh->counter1++;
        int c = sh->counter1;
        int m = sh->multiple;
        semop(semid, &unlock, 1);

        if (c % m == 0) {
            printf("[sem_proc1] counter1=%d is multiple of %d\n", c, m);
        } else {
            printf("[sem_proc1] counter1=%d\n", c);
        }
        fflush(stdout);

        if (c > 500) {
            semctl(semid, 0, IPC_RMID);
            shmdt(sh);
            shmctl(shmid, IPC_RMID, NULL);
            printf("sem_proc1: exiting and removed sem + shm\n");
            break;
        }
        usleep(200000);
    }
    return 0;
}
