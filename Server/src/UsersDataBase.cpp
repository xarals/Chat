#include "UsersDataBase.h"

UsersDataBase* UsersDataBase::db = nullptr;

UsersDataBase::UsersDataBase(const std::string& host, const std::string& login, const std::string& password) {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect(host, login, password);
        conn->setSchema("Chat");
        createTable();
        std::cout << "Database connection established." << std::endl;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQLException: " << e.what() << std::endl;
        exit(1);
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        exit(1);
    }
}

UsersDataBase::~UsersDataBase() {
    if (conn)
        delete conn;
    if (driver)
        delete driver;
}

void UsersDataBase::openConnection(const std::string& host, const std::string& login, const std::string& password) {
    if (db == nullptr)
        db = new UsersDataBase(host, login, password);
}

UsersDataBase* UsersDataBase::getDataBase() {
    return db;
}

sql::ResultSet* UsersDataBase::executeQuery(const std::string& query) {
    if (conn) {
        try {
            sql::Statement* stmt = conn->createStatement();
            sql::ResultSet* res = stmt->executeQuery(query);
            delete stmt;
            return res;
        }
        catch (sql::SQLException& e) {
            std::cerr << "SQLException: " << e.what() << std::endl;
        }
    }
    return nullptr;
}

void UsersDataBase::createTable() {
    std::string query = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INT AUTO_INCREMENT PRIMARY KEY,
            username VARCHAR(255) NOT NULL,
            password VARCHAR(255) NOT NULL,
            salt VARCHAR(255) NOT NULL,
            active TINYINT(1) NOT NULL DEFAULT 1, 
            role VARCHAR(255) NOT NULL
        );
    )";

    try {
        sql::Statement* stmt = conn->createStatement();
        stmt->execute(query);
        delete stmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}

std::tuple<std::string, std::string, std::string, bool, std::string> UsersDataBase::getUser(int id) {
    sql::ResultSet* res = executeQuery("SELECT username, password, salt, active, role FROM users WHERE id = " + std::to_string(id) + ";");
    if (!res)
        return { nullptr, nullptr, nullptr, false, nullptr };
    
    std::string username, password, salt, role;
    bool active;
    
    if (res->next()) {
        username = res->getString(1);
        password = res->getString(2);
        salt = res->getString(3);
        active = res->getBoolean(4);
        role = res->getString(5);
    }

    delete res;

    return { username, password, salt, active, role };
}

std::tuple<std::string, std::string, std::string, bool, std::string> UsersDataBase::getUser(const std::string& input_username) {
    sql::ResultSet* res = executeQuery("SELECT username, password, salt, active, role FROM users WHERE username = '" + input_username + "';");
    if (!res)
        return { nullptr, nullptr, nullptr, false, nullptr };

    std::string username, password, salt, role;
    bool active;

    if (res->next()) {
        username = res->getString(1);
        password = res->getString(2);
        salt = res->getString(3);
        active = res->getBoolean(4);
        role = res->getString(5);
    }

    delete res;

    return { username, password, salt, active, role };
}

int UsersDataBase::getUserId(const std::string& username) {
    sql::ResultSet* res = executeQuery("SELECT id FROM users WHERE username = '" + username + "';");
    if (!res)
        return -1;

    int id = -1;
    if (res->next()) {
        id = res->getInt(1);
    }

    delete res;

    return id;
}

void UsersDataBase::addUser(const std::string& username, const std::string& password, const std::string& salt, bool isActive, const std::string& role) {
    std::string active = "TRUE";
    if (!isActive)
        active = "FALSE";
    sql::ResultSet* res = executeQuery("INSERT INTO users (username, password, salt, active, role) VALUES('" + username + "', '" + password + "', '" + salt + "', " + active + ", '" + role + "'); ");
    if (res)
        delete res;
}

void UsersDataBase::deleteUser(const std::string& username) {
    sql::ResultSet* res = executeQuery("DELETE FROM users WHERE username = '" + username + "'; ");
    if (res)
        delete res;
}

void UsersDataBase::setActive(const std::string& username, bool isActive) {
    std::string active = "FALSE";
    if (isActive)
        active = "TRUE";
    sql::ResultSet* res = executeQuery("UPDATE users SET active = " + active + " WHERE username = '" + username + "'; ");
    if (res)
        delete res;
}

void UsersDataBase::setRole(const std::string& username, const std::string& role) {
    sql::ResultSet* res = executeQuery("UPDATE users SET role = '" + role + "' WHERE username = '" + username + "'; ");
    if (res)
        delete res;
}

void UsersDataBase::setPassword(const std::string& username, const std::string& password, const std::string& salt) {
    sql::ResultSet* res = executeQuery("UPDATE users SET password = '" + password + "', salt = '" + salt + "' WHERE username = '" + username + "'; ");
    if (res)
        delete res;
}

std::pair<std::string, std::string> UsersDataBase::getPassword(const std::string& username) {
    sql::ResultSet* res = executeQuery("SELECT password, salt FROM users WHERE username = '" + username + "'; ");

    if (!res)
        return { nullptr, nullptr };

    std::string password, salt;

    if (res->next()) {
        password = res->getString(1);
        salt = res->getString(2);
    }

    delete res;

    return { password, salt };
}