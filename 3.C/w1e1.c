#include <fcntl.h>      // open()
#include <unistd.h>     // write()
#include <string.h>     // strlen()

int main(int argc, char *argv[])
{
    int fd;
    if (argc != 3)
    {
        write(2, "Pouzitie: ./main filename msg\n", 30);
        return 1;
    }
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, argv[2], strlen(argv[2]));
    close(fd);
    return 0;
}
