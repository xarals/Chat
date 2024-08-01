#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <vector>

class UserManager {
public:
    UserManager(const std::string& host, const std::string& login, const std::string& password);
    void addUser(const std::string& username, SOCKET socket);
    void addUser(int id, SOCKET socket);
    void removeUser(SOCKET socket);
    User getUser(SOCKET socket);
    bool authenticate(const std::string& username, const std::string& password);
    void regustration(const std::string& username, const std::string& password);
    bool checkUsername(const std::string& username);

private:
    std::vector<User> users;
};

#endif