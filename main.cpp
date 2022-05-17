#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    int fd = -1;
    int ret = 0;
    char buf[255];
    const char comand[] = "AT+CLIP=1\n";
    fd = open("/dev/ttyUSB3", O_RDWR);
    if (fd < 0){
        printf("Error open\n");
        return 1;
    }
    printf ("sizeof(comand): %zu", sizeof(comand));
    // write(fd, comand, sizeof(comand));
    do
    {
        ret = read(fd, buf, 254);
        buf[ret] = '\0';
        printf("ret : %d string: %s", ret, buf);
    } while (ret > 0);
    

    return 0;
}