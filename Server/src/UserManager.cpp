#include "UserManager.h"
#include "UsersDataBase.h"
#include "Security.h"

UserManager::UserManager(const std::string& host, const std::string& login, const std::string& password) {
	UsersDataBase::openConnection(host, login, password);
}

void UserManager::addUser(const std::string& input_username, SOCKET socket) {
	std::tuple<std::string, std::string, std::string, bool, std::string> user_info = UsersDataBase::getDataBase()->getUser(input_username);
	if (std::get<0>(user_info).empty())
		return;
	User user(std::get<0>(user_info), std::get<1>(user_info), std::get<2>(user_info), std::get<3>(user_info), std::get<4>(user_info), socket);
	users.push_back(user);
}

void UserManager::addUser(int id, SOCKET socket) {
	std::tuple<std::string, std::string, std::string, bool, std::string> user_info = UsersDataBase::getDataBase()->getUser(id);
	if (std::get<0>(user_info).empty())
		return;
	User user(std::get<0>(user_info), std::get<1>(user_info), std::get<2>(user_info), std::get<3>(user_info), std::get<4>(user_info), socket);
	users.push_back(user);
}

void UserManager::removeUser(SOCKET socket) {
	for (User user : users) {
		if (user.getSocket() == socket) {
			users.erase(std::remove(users.begin(), users.end(), user), users.end());
		}
	}
}

User UserManager::getUser(SOCKET socket) {
	for (User user : users) {
		if (user.getSocket() == socket)
			return user;
	}
}

User UserManager::getUser(const std::string& username) {
	for (User user : users) {
		if (user.getUsername() == username)
			return user;
	}
}

bool UserManager::haveSocket(SOCKET socket) {
	for (User user : users) {
		if (user.getSocket() == socket)
			return true;
	}
	return false;
}

bool UserManager::isConnect(const std::string& username) {
	for (User user : users) {
		if (user.getUsername() == username)
			return true;
	}
	return false;
}

bool UserManager::authenticate(const std::string& username, const std::string& input_password) {
	int user_id = UsersDataBase::getDataBase()->getUserId(username);
	if (user_id == -1)
		return false;

	std::pair<std::string, std::string> hash_password = UsersDataBase::getDataBase()->getPassword(username);
	std::string db_password = std::get<0>(hash_password);
	std::string salt = std::get<1>(hash_password);

	std::string hash_input_password = Security::hashPassword(input_password, salt);

	return hash_input_password == db_password;
}

bool UserManager::checkUsername(const std::string& username) {
	int user_id = UsersDataBase::getDataBase()->getUserId(username);
	return user_id == -1;
}

void UserManager::regustration(const std::string& username, const std::string& password) {
	std::pair<std::string, std::string> hash_password = Security::hashPassword(password);
	UsersDataBase::getDataBase()->addUser(username, std::get<0>(hash_password), std::get<1>(hash_password), true, "user");
}