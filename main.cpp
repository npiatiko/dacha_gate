#include <callHandler.h>
#include <phoneList.h>
#include <stdio.h>
#include <ttyHandler.h>
#include <wiringPi.h>

#define hangup_cmd "AT+CHUP\r\n"

void errExit(const char* errStr);

int main() {
    ttyHandler& modem = ttyHandler::getModem();
    callHandler call;
    phoneList list;
    std::string inStr;
    wiringPiSetupGpio();

    while (modem.readData(inStr)) {
        if (call.found(inStr)) {
            std::string callerId = call.getCallerId().c_str();
            if (list.findPhoneNumber(callerId)) {
                printf("AUTHORIZED: %s\n", callerId.c_str());
            }

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
