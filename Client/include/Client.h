#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

class Client {
public:
    Client(const std::string& server_ip, int port);
    ~Client();
    void connectToServer();
    void sendMessage(const std::string& message);
    void startReceiving();
    std::string getUsername();

private:
    SOCKET client_socket;
    std::string server_ip;
    int port;
    std::string username;

    void receiveMessages();
    void handleLogin();
    void handleRegistration();
};

#endif