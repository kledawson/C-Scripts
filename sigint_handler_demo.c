#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// Signal handler function
void handle_sigint(int sig) {
    signal(SIGINT, handle_sigint);  // Re-enable signal catching
    printf("\nJumped to interrupt handler\n");
}

int main() {
    // Set up the signal handler
    signal(SIGINT, handle_sigint);

    for (int i = 1; i <= 5; i++) {
        printf("Sleep #%d\n", i);
        sleep(1);
    }

    printf("Program exiting\n");
    return 0;
}
