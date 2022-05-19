#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <regex>

#define bufsize 255
#define path "/dev/ttyUSB2"
#define command "at+clip=1\r\n"

int init();
void errExit (const char* errStr);

int main(){
    int fd = -1;
    int ret = 0, wrote = 0;
    char buf[bufsize + 1];
    static const std::regex phone_regex("\\+CLIP: \"(\\+\\d{12})\",145,,,,0", std::regex::icase);
    std::smatch match;

    fd = init();
    // printf(command);
    // printf ("sizeof(command): %zu\n", sizeof(command));
    wrote = write(fd, command, sizeof(command));
    // printf ("wrote: %d\n", wrote);

    while (ret = read(fd, buf, bufsize)){
        buf[ret] = '\0';
        std::string inStr(buf);
        if (std::regex_search(inStr, match, phone_regex)) {
            printf("FOUND!!! match.size(): %d: %s\n", match.size(), match[0].str().c_str());
        }
        printf("ret: %d string: %s", ret, inStr.c_str());
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

    if (tcgetattr(fd, &tp) == -1){
        errExit("Error tcgetattr");
    }
    printf("tcgetattr SUCCESS\n");

    tp.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE);
    tp.c_oflag &= ~(ONLCR | OPOST);
    tp.c_iflag &= ~(ICRNL | IXON);
    tp.c_iflag |= IGNBRK;

    if (tcsetattr(fd, TCSAFLUSH, &tp) == -1){
        errExit("Error tcsetattr");
    }

    printf("tcsetattr SUCCESS\n");
    sleep(1);
    return fd;
}
