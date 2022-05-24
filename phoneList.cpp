#include <phoneList.h>
#include <stdlib.h>

#include <fstream>
#include <regex>

std::set<std::string> phoneList::authorizedUsers;

phoneList::phoneList() {
    system("/bin/bash getFile.sh");
    load(listFile);
    for (auto elem : authorizedUsers) {
        printf("Phone num : %s\n", elem.c_str());
    }
}

bool phoneList::checkStr(const std::string& inStr) {
    static const std::regex phone_regex("\\+\\d{12}");
    std::smatch match;

    if (std::regex_search(inStr, match, phone_regex)) {
        return true;
    }

    return false;
}

bool phoneList::load(const std::string& path) {
    bool result = false;
    std::ifstream file(path, std::ios_base::in);

    ready = false;
    if (file.is_open() && file.good()) {
        authorizedUsers.clear();
        for (std::string line; std::getline(file, line);) {
            if (checkStr(line)) {
                authorizedUsers.insert(line);
            }
        }
        result = true;
        ready = true;
    }

    file.close();
    return result;
}

bool phoneList::isReady() {
    return ready;
}
bool phoneList::findPhoneNumber(const std::string& phoneNum) {
    return (authorizedUsers.find(phoneNum) != authorizedUsers.end());
}
