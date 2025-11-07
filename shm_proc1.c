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

    sh->multiple = 3;
    sh->counter1 = 0;

    printf("shm_proc1 PID %d: initialized multiple=%d counter1=%d\n", getpid(), sh->multiple, sh->counter1);
    fflush(stdout);

    while (1) {
        sh->counter1++;
        int c = sh->counter1;
        if (c % sh->multiple == 0) {
            printf("[shm_proc1 PID %d] counter1=%d is multiple of %d\n", getpid(), c, sh->multiple);
        } else {
            printf("[shm_proc1 PID %d] counter1=%d\n", getpid(), c);
        }
        fflush(stdout);

        if (c > 500) {
            printf("shm_proc1: counter1 > 500; exiting and cleaning up\n");
            shmdt(sh);
            shmctl(shmid, IPC_RMID, NULL);
            break;
        }
        usleep(200000); 
    }

    return 0;
}
