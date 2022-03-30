#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("%s", "Usage error.\n");
        return 1;
    }

    const char* path = "/tmp/fifo_005";
    if (mkfifo(path, 0666) == -1) {
        perror("fifo make");
        return 1;
    }

    const int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("fifo open");
        return 1;
    }

    write(fd, argv[1], strlen(argv[1]));
    close(fd);
    
    return 0;
}