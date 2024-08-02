#ifndef USER_H
#define USER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

class User {
public:
    User(const std::string& username, const std::string& password, const std::string& salt, bool isActive, const std::string& role, SOCKET socket);
    SOCKET getSocket();
    std::string getUsername();
    std::string getHeighestRole();
    bool canChange(const std::string& role);
    bool operator==(const User& other) const;

    static std::string getHeighestRole(const std::string& roles);

private:
    SOCKET socket;
    std::string username;
    std::string password;
    std::string salt;
    bool isActive;
    std::string role;
};

#endif