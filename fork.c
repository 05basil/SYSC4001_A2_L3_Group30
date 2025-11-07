#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        // Child process: Process 2
        int c = 0;
        while (1) {
            printf("[Child PID %d] counter = %d\n", getpid(), c++);
            fflush(stdout);
            usleep(500000); 
        }
    } else {
        // Parent process: Process 1
        int c = 0;
        while (1) {
            printf("[Parent PID %d] counter = %d\n", getpid(), c++);
            fflush(stdout);
            usleep(500000); 
        }
    }
    return 0;
}
