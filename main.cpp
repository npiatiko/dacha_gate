#include <callHandler.h>
#include <gateController.h>
#include <phoneList.h>
#include <stdio.h>
#include <ttyHandler.h>
#include <unistd.h>

void errExit(const char* errStr);

int main() {
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
    ttyHandler& modem = ttyHandler::getModem();
    gateController& gate = gateController::getController();
    callHandler call;
    phoneList list;
    std::string inStr;

    while (modem.readData(inStr)) {
        if (call.found(inStr)) {
            modem.hangUp();
            std::string callerId = call.getCallerId();
            if (list.findPhoneNumber(callerId)) {
                printf("AUTHORIZED: %s\n", callerId.c_str());
                gate.openGate();
                sleep(5);
                gate.closeGate();
                modem.flush();
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
