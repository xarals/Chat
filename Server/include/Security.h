#ifndef SECURITY_H
#define SECURITY_H

#include <string>

class Security {
public:
	static std::pair<std::string, std::string> hashPassword(const std::string& password);
	static std::string hashPassword(const std::string& password, const std::string& salt);
private:
	static std::string generateSalt(size_t length);
	static std::string hashString(const std::string& str);
};

#endif
