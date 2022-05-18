#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#define bufsize 255
#define path "/dev/ttyUSB2"
void init(int fd);
int main(){
    int fd = -1;
    int ret = 0, wrote = 0;
    char buf[bufsize + 1];
    const char command[] = "at+clip=1\r\n";
    fd = open(path, O_RDWR);
    if (fd < 0){
        printf("Error open\n");
        return 1;
    }
    init(fd);
    // printf(command);
    // printf ("sizeof(command): %zu\n", sizeof(command));
    wrote = write(fd, command, sizeof(command));
    // printf ("wrote: %d\n", wrote);

    while ((ret = read(fd, buf, bufsize)) > 0 ){
        buf[ret] = '\0';
        printf("ret: %d string: %s", ret, buf);
    }    

    return 0;
}

void init(int fd){
    struct termios tp, save;

    // tcflush(fd,TCIOFLUSH);
    if (tcgetattr(fd, &tp) < 0){
        printf("Error tcgetattr\n");
        _exit;
    }
    printf("tcgetattr SUCCESS\n");
    save = tp;
    tp.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
    if (tcsetattr(fd, TCSAFLUSH, &tp) < 0){
        printf("Error tcsetattr\n");
        _exit;
    }
    printf("tcsetattr SUCCESS\n");

    sleep(1);
}
