#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) { 
        perror("fork"); 
        exit(1); 
    }
    if (pid == 0) {
        // child: run proc2_wait
        execl("./proc2_wait", "./proc2_wait", (char*)NULL);
        perror("execl");
        exit(1);
    } else {
        // parent: run proc1-like printing but also wait on child 
        int counter = 0;
        int status;
        while (1) {
            if (counter % 3 == 0) {
                printf("[wait parent PID %d] Cycle %d - %d is multiple of 3\n", getpid(), counter, counter);
            } else {
                printf("[wait parent PID %d] Cycle %d\n", getpid(), counter);
            }
            fflush(stdout);
            counter++;
            usleep(400000);

            pid_t w = waitpid(pid, &status, WNOHANG);
            if (w == -1) {
                perror("waitpid");
                break;
            } else if (w > 0) {
                if (WIFEXITED(status)) {
                    printf("Child exited with status %d\n", WEXITSTATUS(status));
                } 
                else {
                    printf("Child terminated abnormally\n");
                }
                printf("Parent will exit too.\n");
                break;
            }
        }
    }
    return 0;
}
