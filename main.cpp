#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

#define bufsize 255
#define path "/dev/ttyUSB2"
#define command "at+clip=1\r\n"

int init();
void errExit (const char* errStr);

int main(){
    int fd = -1;
    int ret = 0, wrote = 0;
    char buf[bufsize + 1];

    fd = init();
    // printf(command);
    // printf ("sizeof(command): %zu\n", sizeof(command));
    wrote = write(fd, command, sizeof(command));
    // printf ("wrote: %d\n", wrote);

    while (ret = read(fd, buf, bufsize)){
        buf[ret] = '\0';
        printf("ret: %d string: %s", ret, buf);
    }    
    printf ("exit\n");

    return 0;
}

void errExit (const char* errStr){
    printf("%s\n", errStr);
    exit(EXIT_FAILURE);
}

int init(){
    int fd = -1;
    struct termios tp, save;

    fd = open(path, O_RDWR);
    if (fd < 0){
        errExit("Error open");
    }

    tp.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
    if (tcsetattr(fd, TCSAFLUSH, &tp) == -1){
        errExit("Error tcsetattr");
    }

    printf("tcsetattr SUCCESS\n");

    sleep(1);
    return fd;
}
