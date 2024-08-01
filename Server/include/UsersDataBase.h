#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <string>
#include "jdbc/mysql_driver.h"
#include "jdbc/mysql_connection.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/resultset.h"

class UsersDataBase {
public:
	static void openConnection(const std::string& host, const std::string& login, const std::string& password);
	static UsersDataBase* getDataBase();
	~UsersDataBase();
	std::tuple<std::string, std::string, std::string, bool, std::string> getUser(int id);
	std::tuple<std::string, std::string, std::string, bool, std::string> getUser(const std::string& username);
	int getUserId(const std::string& username);
	void addUser(const std::string& username, const std::string& password, const std::string& salt, bool isActive, const std::string& role);
	void deleteUser(const std::string& username);
	void setActive(const std::string& username, bool isActive);
	void setRole(const std::string& username, const std::string& role);
	void setPassword(const std::string& username, const std::string& password, const std::string& salt);
	std::pair<std::string, std::string> getPassword(const std::string& username);

private:
	static UsersDataBase* db;
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* conn;

	UsersDataBase(const std::string& host, const std::string& login, const std::string& password);
	sql::ResultSet* executeQuery(const std::string& query);
	void createTable();
};

#endif
