#include "User.h"

User::User(const std::string& username, const std::string& password, const std::string& salt, bool isActive, const std::string& role, SOCKET socket) :
	username(username), password(password), salt(salt), isActive(isActive), role(role), socket(socket) {};

SOCKET User::getSocket() {
	return socket;
}

std::string User::getUsername() {
	return username;
}

std::string User::getHeighestRole() {
    std::vector<std::string> roles;
    size_t start = 0;
    size_t end = role.find(", ");

    while (end != std::string::npos) {
        roles.push_back(role.substr(start, end - start));
        start = end + 2;
        end = role.find(", ", start);
    }
    roles.push_back(role.substr(start));

    int rank = 0;
    std::string heighest_role = "user";

    for (std::string user_role : roles) {
        if (user_role == "creator" && rank < 3) {
            rank = 3;
            heighest_role = "creator";
        }
        else if (user_role == "admin" && rank < 2) {
            rank = 2;
            heighest_role = "admin";
        }
        else if (user_role == "moderator" && rank < 1) {
            rank = 1;
            heighest_role = "moderator";
        }
    }
    
    return heighest_role;
}

std::string User::getHeighestRole(const std::string& role) {
    std::vector<std::string> roles;
    size_t start = 0;
    size_t end = role.find(", ");

    while (end != std::string::npos) {
        roles.push_back(role.substr(start, end - start));
        start = end + 2;
        end = role.find(", ", start);
    }
    roles.push_back(role.substr(start));

    int rank = 0;
    std::string heighest_role = "user";

    for (std::string user_role : roles) {
        if (user_role == "creator" && rank < 3) {
            rank = 3;
            heighest_role = "creator";
        }
        else if (user_role == "admin" && rank < 2) {
            rank = 2;
            heighest_role = "admin";
        }
        else if (user_role == "moderator" && rank < 1) {
            rank = 1;
            heighest_role = "moderator";
        }
    }
    
    return heighest_role;
}

bool User::canChange(const std::string& role) {
    std::string heighestRole = getHeighestRole();
    int input_score = 0;
    int user_score = 0;

    if (role == "creator")
        input_score = 3;
    else if (role == "admin")
        input_score = 2;
    else if (role == "moderator")
        input_score = 1;

    if (heighestRole == "creator")
        user_score = 3;
    else if (heighestRole == "admin")
        user_score = 2;
    else if (heighestRole == "moderator")
        user_score = 1;

    return user_score > input_score;
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