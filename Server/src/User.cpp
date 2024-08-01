#include "User.h"

User::User(const std::string& username, const std::string& password, const std::string& salt, bool isActive, const std::string& role, SOCKET socket) :
	username(username), password(password), salt(salt), isActive(isActive), role(role), socket(socket) {};

SOCKET User::getSocket() {
	return socket;
}

std::string User::getUsername() {
	return username;
}

bool User::operator==(const User& other) const {
	return (username == other.username) && (password == other.password) && (isActive == other.isActive) 
        && (socket == other.socket) && (salt == other.salt) && (role == other.role);

    SOCKET socket;
    std::string username;
    std::string password;
    std::string salt;
    bool isActive;
    std::string role;
}