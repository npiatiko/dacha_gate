#include <ttyHandler.h>

ttyHandler::ttyHandler(/* args */) {
    fd = open(devPath.c_str(), O_RDWR);
    if (fd < 0) {
        printf("Error open\n");
    } else {
        configTTY();
    }
}

ttyHandler::~ttyHandler() {
    close(fd);
}

void ttyHandler::configTTY() {
    struct termios tp;

    if (tcgetattr(fd, &tp) == -1) {
        printf("Error tcgetattr\n");
        return;
    }
    printf("tcgetattr SUCCESS\n");

    tp.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE);
    tp.c_oflag &= ~(ONLCR | OPOST);
    tp.c_iflag &= ~(ICRNL | IXON);
    tp.c_iflag |= IGNBRK;

    if (tcsetattr(fd, TCSAFLUSH, &tp) == -1) {
        printf("Error tcsetattr\n");
        return;
    }

    printf("tcsetattr SUCCESS\n");

    if (!sendCommandAck(configTty_cmd)){
        printf("Config modem failure...\n");
        return;
    }

    ready = true;
    sleep(1);
}

bool ttyHandler::readData(std::string& outStr) {
    // printf("func: %s Line: %d\n", __func__, __LINE__);

    int ret = 0;

    ret = read(fd, buf, bufsize);
    if (ret <= 0) {
        return false;
    }

    buf[ret] = '\0';
    outStr = std::string(buf);
    printf("ret: %d string: %s", ret, buf);

    return true;
}

ttyHandler& ttyHandler::getModem() {
    static ttyHandler modem;

    return modem;
}

bool ttyHandler::sendCommandAck(const std::string& command) {
    bool result = false;
    int retries = 5;

    while (--retries) {
        printf("func: %s Line: %d\n", __func__, __LINE__);
        int wrote = 0;

        wrote = write(fd, command.c_str(), command.length());
        sleep(1);
        if (wrote == command.length()) {
            std::string ackMsg;

            if (readData(ackMsg) && ackMsg.find("OK") != std::string::npos) {
                result = true;
                break;
            }
        }
    }

    return result;
}

bool ttyHandler::sendCommand(const std::string& command) {
    bool result = false;
    int retries = 5;

    while (--retries) {
        int wrote = 0;

        wrote = write(fd, command.c_str(), command.length());
        if (wrote == command.length()) {
            result = true;
            break;
        }
    }

    return result;
}
