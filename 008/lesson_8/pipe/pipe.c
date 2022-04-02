#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t cpid = fork();
    if (cpid == -1) {
        perror("fork");
        return 1;
    }

    char buf;
    if (cpid == 0) {    
        close(pipefd[1]);

        write(STDOUT_FILENO, "Printing message from parent: ", 31);
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
    }

    close(pipefd[0]);          
    write(pipefd[1], argv[1], strlen(argv[1]));
    close(pipefd[1]);          
    wait(NULL);        

    return 0;
}