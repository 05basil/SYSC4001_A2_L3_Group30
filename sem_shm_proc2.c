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
    int shmid = shmget(key, sizeof(struct shm_data), 0666);
    if (shmid < 0) { 
        perror("shmget"); 
        return 1; 
    }
    struct shm_data *sh = (struct shm_data*) shmat(shmid, NULL, 0);
    if (sh == (void*)-1) { 
        perror("shmat"); 
        return 1; 
    }

    int semid = semget(key, 1, 0666);
    if (semid < 0) { 
        perror("semget"); 
        return 1; 
    }

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    printf("[sem_proc2 PID %d] attached to shm+sem. Waiting for counter1 > 100\n", getpid());
    fflush(stdout);

    while (1) {
        semop(semid, &lock, 1);
        int c = sh->counter1;
        semop(semid, &unlock, 1);
        if (c > 100) break;
        usleep(100000);
    }

    while (1) {
        semop(semid, &lock, 1);
        int c = sh->counter1;
        int m = sh->multiple;
        semop(semid, &unlock, 1);

        printf("[sem_proc2] sees counter1=%d multiple=%d\n", c, m);
        fflush(stdout);

        if (c > 500) {
            printf("[sem_proc2] counter1 > 500, exiting\n");
            break;
        }
        usleep(300000);
    }

    shmdt(sh);
    return 0;
}
