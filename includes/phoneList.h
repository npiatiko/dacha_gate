#ifndef PHONE_LIST
#define PHONE_LIST
#include <set>
#include <string>

class phoneList {
   private:
    const std::string listFile = "./accessList.txt";
    static std::set<std::string> authorizedUsers;
    bool ready = false;

    bool checkStr(const std::string& inStr);

   public:
    bool findPhoneNumber(const std::string& phoneNum);
    bool load(const std::string& path);
    bool isReady();
    phoneList();
    ~phoneList() = default;
};

#endif
