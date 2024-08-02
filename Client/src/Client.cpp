#include "Client.h"
#include <iostream>
#include <cstring>

Client::Client(const std::string& server_ip, int port) : server_ip(server_ip), port(port), client_socket(INVALID_SOCKET) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        exit(1);
    }
}

Client::~Client() {
    closesocket(client_socket);
    WSACleanup();
}

void Client::connectToServer() {
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        closesocket(client_socket);
        WSACleanup();
        exit(1);
    }

    if (connect(client_socket, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(client_socket);
        WSACleanup();
        exit(1);
    }

    std::cout << "Connected to server" << std::endl;

    std::thread(&Client::startReceiving, this).detach();

    int choice;

    std::cout << "/login - login\n/registration - registration" << std::endl;
}

void Client::sendMessage(const std::string& message) {
    send(client_socket, message.c_str(), message.length(), 0);
}

void Client::startReceiving() {
    receiveMessages();
}

void Client::receiveMessages() {
    char buffer[1024];
    int valread;
    while ((valread = recv(client_socket, buffer, 1024, 0)) > 0) {
        buffer[valread] = '\0';
        std::cout << buffer << std::endl;
    }
}

void Client::handleLogin() {
    std::string password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    std::string login_message = "/login " + username + " " + password;
    sendMessage(login_message);
}

void Client::handleRegistration() {
    std::string password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (username.find(' ') != std::string::npos || username.find('\'') != std::string::npos || username.find('\"') != std::string::npos || username.find('\\') != std::string::npos || username.find('/') != std::string::npos) {
        std::cerr << "Invalid username";
        handleRegistration();
    }

    if (password.find(' ') != std::string::npos || password.find('\'') != std::string::npos || password.find('\"') != std::string::npos || password.find('\\') != std::string::npos || password.find('/') != std::string::npos) {
        std::cerr << "Invalid password";
        handleRegistration();
    }

    std::string registration_message = "/registration " + username + " " + password;
    sendMessage(registration_message);
}

std::string Client::getUsername() {
    return username;
}