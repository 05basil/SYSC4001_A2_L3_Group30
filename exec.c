#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) { 
        perror("fork"); 
        exit(1); 
    }
    if (pid == 0) {
        // Child: replace image with proc2
        char *args[] = {"./proc2", NULL};
        execv(args[0], args);
        perror("execv failed");
        exit(1);
    } else {
        // Parent: run proc1 code inline 
        int counter = 0;
        while (1) {
            if (counter % 3 == 0) {
                printf("[proc1 PID %d] Cycle %d - %d is a multiple of 3\n", getpid(), counter, counter);
            } else {
                printf("[proc1 PID %d] Cycle %d\n", getpid(), counter);
            }
            fflush(stdout);
            counter++;
            usleep(400000);
        }
    }
    return 0;
}
