#include <stdio.h>
#include <unistd.h>

int main() {
    int counter = 0;
    while (1) {
        counter--;
        if (counter % 3 == 0) {
            printf("[proc2 PID %d] Cycle %d - %d is a multiple of 3\n", getpid(), -counter, counter);
        } else {
            printf("[proc2 PID %d] Cycle %d\n", getpid(), -counter);
        }
        fflush(stdout);
        usleep(400000); 
    }
    return 0;
}
