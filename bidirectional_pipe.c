#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 100

// Function to reverse the case of characters in a string
void reverse_case(char *str) {
    for (int i = 0; str[i]; i++) {
        if (islower(str[i]))
            str[i] = toupper(str[i]);
        else if (isupper(str[i]))
            str[i] = tolower(str[i]);
    }
}

int main() {
    int pipe1[2], pipe2[2];  // pipe1 for parent to child, pipe2 for child to parent
    pid_t pid;
    char original_msg[BUFFER_SIZE] = "Hi There";
    char modified_msg[BUFFER_SIZE];

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid > 0) {  // Parent process
        close(pipe1[0]);  // Close reading end of pipe1
        close(pipe2[1]);  // Close writing end of pipe2

        // Send the original message to the child process
        write(pipe1[1], original_msg, strlen(original_msg) + 1);
        close(pipe1[1]);  // Close writing end after sending

        // Read the modified message from the child
        read(pipe2[0], modified_msg, BUFFER_SIZE);
        close(pipe2[0]);  // Close reading end after reading

        printf("Parent received modified message: %s\n", modified_msg);
    } else {  // Child process
        close(pipe1[1]);  // Close writing end of pipe1
        close(pipe2[0]);  // Close reading end of pipe2

        // Read the original message from the parent
        read(pipe1[0], modified_msg, BUFFER_SIZE);
        close(pipe1[0]);  // Close reading end after reading

        // Reverse the case of the message
        reverse_case(modified_msg);

        // Send the modified message back to the parent
        write(pipe2[1], modified_msg, strlen(modified_msg) + 1);
        close(pipe2[1]);  // Close writing end after sending
    }

    return 0;
}