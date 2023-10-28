#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    int fd[2];
    const int WRITE = 1;
    const int READ = 0;
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0){
        close(fd[READ]);
        dup2(fd[WRITE], 1); // Redirect stdout to the write end of the pipe
        close(fd[WRITE]);
        // execlp("cat", "cat", "./catalog-info.yaml", NULL);
        char input[100];
        while (1) {
            fgets(input, sizeof(input), stdin); // Read a line of input

            if (strcmp(input, "stop\n") == 0) { // Include newline character in the comparison
                break;
            }

            // You can process the user input here as needed
            printf("You entered: %s", input);
        }
    }
    else{
        pid_t pid2 = fork();
        if (pid2 == 0){
            close(fd[READ]);
            dup2(fd[WRITE], 1); // Redirect stdout to the write end of the pipe
            close(fd[WRITE]);
            for (int i = 0; i < 15; i++){
                printf("%d\n", i);
                sleep(1);
            }
            // execlp("cat", "cat", "./.github/workflows/add-to-project.yaml", NULL);
        }
        else{
            close(fd[WRITE]);
            dup2(fd[READ], 0); // Redirect stdin to the read end of the pipe
            close(fd[READ]);
            // execlp("cat", "cat", NULL);
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
                fprintf(file, "%s", input); // Write the input to the file
            }
            // Close the file
            fclose(file);

            printf("Data has been written to %s.\n", filename);
            return 0;
        }
    }
}