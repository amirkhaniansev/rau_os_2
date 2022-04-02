#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    const char* path = "/tmp/fifo_005";
    const int   fd   = open(path, O_RDONLY);
    if (fd == -1) {
        perror("fifo open");
        return 1;
    }
    
    write(STDOUT_FILENO, "Message from writer: ", 22);

    char buf;
    while (read(fd, &buf, 1) > 0)
        write(STDOUT_FILENO, &buf, 1);
    
    write(STDOUT_FILENO, "\n", 1);
    
    close(fd);
    unlink(path);

    return 0;
}