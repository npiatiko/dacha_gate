#include <fcntl.h>
#include <termios.h>
#include <ttyHandler.h>
#include <unistd.h>

ttyHandler::ttyHandler() {
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

    tp.c_lflag &= static_cast<tcflag_t>(~(ICANON | ISIG | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE));
    tp.c_oflag &= static_cast<tcflag_t>(~(ONLCR | OPOST));
    tp.c_iflag &= static_cast<tcflag_t>(~(ICRNL | IXON));
    tp.c_iflag |= static_cast<tcflag_t>(IGNBRK);

    if (tcsetattr(fd, TCSAFLUSH, &tp) == -1) {
        printf("Error tcsetattr\n");
        return;
    }

    sleep(1);
    printf("tcsetattr SUCCESS\n");

    flush();

    if (!sendCommandAck(configTty_cmd)) {
        printf("Config modem failure...\n");
        return;
    }

    ready = true;
    printf("Config SUCCESS\n");
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
        // printf("func: %s Line: %d\n", __func__, __LINE__);
        ssize_t wrote = 0;

        wrote = write(fd, command.c_str(), command.length());
        sleep(1);
        if (wrote == static_cast<ssize_t>(command.length())) {
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
        ssize_t wrote = 0;

        wrote = write(fd, command.c_str(), command.length());
        if (wrote == static_cast<ssize_t>(command.length())) {
            result = true;
            break;
        }
    }

    return result;
}

void ttyHandler::hangUp() {
    sendCommandAck(hangup_cmd);
}

void ttyHandler::flush() {
    tcflush(fd, TCIOFLUSH);
}
