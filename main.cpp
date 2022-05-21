#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ttyHandler.h>
#include <callHandler.h>

#define hangup_cmd "AT+CHUP\r\n"

void errExit(const char* errStr);

int main() {
    ttyHandler& modem = ttyHandler::getModem();
    callHandler call;
    std::string inStr;

    while (modem.readData(inStr)) {
        if (call.found(inStr)) {
            printf("getCallerId: %s\n", call.getCallerId().c_str());
            // write(fd, hangup_cmd, sizeof(hangup_cmd));
            // sleep(10);
            // printf("Resuming...\n");

            // tcflush(fd, TCIOFLUSH);
        }
    }
    printf("exit\n");

    return 0;
}

void errExit(const char* errStr) {
    printf("%s\n", errStr);
    exit(EXIT_FAILURE);
}
