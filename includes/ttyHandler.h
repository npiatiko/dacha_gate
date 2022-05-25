#ifndef TTY_HANDLER
#define TTY_HANDLER

#include <string>

class ttyHandler {
   private:
    const std::string devPath = "/dev/ttyUSB2";
    const std::string configTty_cmd = "AT+CLIP=1\r\n";
    const std::string hangup_cmd = "AT+CHUP\r\n";

    static constexpr int bufsize = 255;
    bool ready = false;
    int fd = -1;
    void configTTY();
    char buf[bufsize + 1];
    ttyHandler();
    ttyHandler(const ttyHandler&) = delete;
    ttyHandler& operator=(ttyHandler&) = delete;

   public:
    ~ttyHandler();
    static ttyHandler& getModem();
    bool readData(std::string& outStr);
    bool sendCommandAck(const std::string& data);
    bool sendCommand(const std::string& data);
    void hangUp();
    void flush();
};

#endif
