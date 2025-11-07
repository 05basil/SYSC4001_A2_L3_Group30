#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

struct shm_data { 
    int multiple; 
    int counter1; 
};

int main() {
    key_t key = 0x1234;
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

    printf("[shm_proc2 PID %d] attached to shm. Waiting for counter1 > 100 to start.\n", getpid());
    fflush(stdout);

    while (sh->counter1 <= 100) {
        usleep(100000);
    }
    printf("[shm_proc2 PID %d] starting because counter1=%d\n", getpid(), sh->counter1);
    fflush(stdout);

    while (1) {
        int c = sh->counter1;
        int m = sh->multiple;
        printf("[shm_proc2 PID %d] sees counter1=%d multiple=%d\n", getpid(), c, m);
        fflush(stdout);
        if (c > 500) {
            printf("[shm_proc2 PID %d] counter1 > 500; exiting\n", getpid());
            break;
        }
        usleep(300000);
    }

    shmdt(sh);
    return 0;
}
