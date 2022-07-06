#ifndef CALL_HANDLER
#define CALL_HANDLER

#include <regex>
#include <string>

class callHandler {
   private:
    static const std::regex phone_regex;
    std::smatch match;

   public:
    bool found(const std::string& inStr);
    std::string getCallerId();
    callHandler();
    ~callHandler() = default;
    
};

#endif
