#include <callHandler.h>

const std::regex callHandler::phone_regex("\\+CLIP: \"(\\+\\d{12})\",145,,,,0", std::regex::icase);
callHandler::callHandler() : match(std::smatch()) {
}

bool callHandler::found(const std::string& inStr) {
    if (std::regex_search(inStr, match, phone_regex)) {
        // printf("FOUND!!! match.size(): %d: %s\n", match.size(), match[0].str().c_str());
        return true;
    }
    return false;
}

std::string callHandler::getCallerId() {
    std::string CallerId;

    if (match.ready()) {
        CallerId = match[1].str();
        // printf("match.ready(): true\n");
    }

    return CallerId;
}
