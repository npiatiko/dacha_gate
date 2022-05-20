#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ttyHandler.h>

#include <regex>

#define hangup_cmd "AT+CHUP\r\n"

void errExit(const char* errStr);

int main() {
    static const std::regex phone_regex("\\+CLIP: \"(\\+\\d{12})\",145,,,,0", std::regex::icase);
    std::smatch match;
    ttyHandler& modem = ttyHandler::getModem();
    std::string inStr;

    while (modem.readData(inStr)) {
        if (std::regex_search(inStr, match, phone_regex)) {
            printf("FOUND!!! match.size(): %d: %s\nSleeping...\n", match.size(), match[0].str().c_str());
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

