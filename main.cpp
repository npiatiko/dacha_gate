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
            std::string callerId = call.getCallerId();

            printf("Incoming call: %s...\n", callerId.c_str());
            modem.hangUp();
            if (list.findPhoneNumber(callerId)) {
                printf("%s: AUTHORIZED, opening gate...\n", callerId.c_str());
                gate.openGate();
                sleep(5);
                gate.closeGate();
                modem.flush();
            }
        }
    }

    printf("Ret 0, exit\n");
    return 0;
}

void errExit(const char* errStr) {
    printf("%s\n", errStr);
    exit(EXIT_FAILURE);
}
