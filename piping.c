#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pid1, pid2;
const int WRITE = 1;
const int READ = 0;
void sigchld_handler(int signo) {
    pid_t pid;
    int status;

    // Check which child process has exited
    if ((pid = waitpid(pid1, &status, WNOHANG)) > 0) {
        fprintf(stderr, "Child 1 exited\n");
        kill(pid2, SIGTERM);  // Send SIGTERM to the other child process
    } else if ((pid = waitpid(pid2, &status, WNOHANG)) > 0) {
        fprintf(stderr, "Child 2 exited\n");
        kill(pid1, SIGTERM);  // Send SIGTERM to the other child process
    }
}
int main(){
    int fd[2];
    // Install the SIGCHLD handler
    struct sigaction sa = { 0 };
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        perror(0);
        exit(1);
    }

    pipe(fd);
    pid1 = fork();
    if (pid1 == 0){
        close(fd[WRITE]);
        dup2(fd[READ], 0); // Redirect stdin to the read end of the pipe
        close(fd[READ]);
        FILE *file;
        char filename[] = "./test.txt";
        char input[1000];

        // Open the file for writing
        file = fopen(filename, "w");
        if (file == NULL) {
            printf("Failed to open the file for writing.\n");
            return 1;
        }
        printf("Hello World\n");
        while (fgets(input, sizeof(input), stdin) != NULL) {
            if (strcmp(input, "14") == 0) exit(1);
            fprintf(file, "%s", input); // Write the input to the file
            fflush(file); // Flush the file buffer
        }
        // Close the file
        fclose(file);

        printf("Data has been written to %s.\n", filename);
        return 0;
    }
    else{
        pid2 = fork();
        if (pid2 == 0){
            close(fd[READ]);
            dup2(fd[WRITE], 1); // Redirect stdout to the write end of the pipe
            close(fd[WRITE]);
            for (int i = 0; i < 15; i++){
                printf("%d\n", i);
                fflush(stdout); // Flush the stdout buffer to ensure the output is seen by the read end of the pipe
                sleep(1);
            }
        }
        else{
            close(fd[READ]);
            dup2(fd[WRITE], 1); // Redirect stdout to the write end of the pipe
            close(fd[WRITE]);
            char input[100];
            while (1) {
                fgets(input, sizeof(input), stdin); // Read a line of input

                if (strcmp(input, "stop\n") == 0) { // Include newline character in the comparison
                    break;
                }

                // You can process the user input here as needed
                printf("You entered: %s", input);
                fflush(stdout); // Flush the stdout buffer to ensure the output is seen by the read end of the pipe
            }
        }
    }
}