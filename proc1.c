#include <stdio.h>
#include <unistd.h>

int main() {
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
    return 0;
}
