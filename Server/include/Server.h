#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <mutex>
#include "UserManager.h"

#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server(int port);
    ~Server();
    void start();

private:
    int port;
    SOCKET server_socket;
    std::vector<SOCKET> clients;
    std::mutex clients_mutex;
    UserManager* user_manager;

    void handleClient(SOCKET client_socket);
    void broadcastMessage(const std::string& message, SOCKET sender_socket);
    void handleLogin(const std::string& message, SOCKET client_socket);
    void handleRegistration(const std::string& message, SOCKET client_socket);
    void handleBan(const std::string& message, SOCKET sender_socket);
    void handleUnban(const std::string& message, SOCKET sender_socket);
    void handleKick(const std::string& message, SOCKET sender_socket);
    void handleDelete(const std::string& message, SOCKET sender_socket);
    void handleAddRole(const std::string& message, SOCKET sender_socket);
};

#endif