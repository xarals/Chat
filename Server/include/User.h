#ifndef USER_H
#define USER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class User {
public:
    User(const std::string& username, const std::string& password, const std::string& salt, bool isActive, const std::string& role, SOCKET socket);
    SOCKET getSocket();
    std::string getUsername();
    bool operator==(const User& other) const;

private:
    SOCKET socket;
    std::string username;
    std::string password;
    std::string salt;
    bool isActive;
    std::string role;
};

#endif