#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <random>
#include <functional>
#include "Security.h"

std::string Security::generateSalt(size_t length) {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);
    std::string salt;

    for (size_t i = 0; i < length; ++i) {
        salt += charset[dist(rng)];
    }

    return salt;
}

std::string Security::hashString(const std::string& str) {
    std::hash<std::string> hash_fn;
    size_t hash = hash_fn(str);

    std::stringstream ss;
    ss << std::hex << hash;

    return ss.str();
}

std::pair<std::string, std::string> Security::hashPassword(const std::string& password) {
    const size_t saltLength = 16;
    std::string salt = generateSalt(saltLength);
    std::string saltedPassword = password + salt;
    std::string hashedPassword = hashString(saltedPassword);

    return { hashedPassword, salt };
}

std::string Security::hashPassword(const std::string& password, const std::string& salt) {
    std::string saltedPassword = password + salt;
    std::string hashedPassword = hashString(saltedPassword);

    return hashedPassword;
}