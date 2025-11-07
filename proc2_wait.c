#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int counter = 0;
    while (1) {
        counter--;
        printf("[proc2_wait PID %d] counter = %d\n", getpid(), counter);
        fflush(stdout);
        if (counter < -500) {
            printf("[proc2_wait PID %d] reached %d, exiting.\n", getpid(), counter);
            fflush(stdout);
            break;
        }
        usleep(20000); 
    }
    return 0;
}
