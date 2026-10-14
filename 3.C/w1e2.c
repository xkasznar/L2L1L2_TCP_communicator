#include <fcntl.h>      // open()
#include <unistd.h>     // write(), read(), close()
#include <string.h>     // strlen()
#include <stdlib.h>     // srand(), rand()
#include <time.h>       // time()
#include <stdio.h>      // sprintf()

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int fd;
    char tmp[1];
    char buf[1024];
    if (argc != 3)
    {
        write(2, "Pouzitie: ./main filename msg\n", 30);
        return 1;
    }
    fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    
    while ((read(fd, tmp, sizeof(tmp))) > 0){}
    sprintf(buf, "%d%s", rand() % 10000, argv[2]);
    
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}
