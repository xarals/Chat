#include "Server.h"
#include <iostream>

Server::Server(int port) : port(port), server_socket(INVALID_SOCKET) {
    user_manager = new UserManager("tcp://127.127.126.50:3306/Chat", "root", "");

    WSADATA wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        exit(1);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }

    if (listen(server_socket, 10) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
}

Server::~Server() {
    closesocket(server_socket);
    WSACleanup();
    if (user_manager)
        delete user_manager;
}

void Server::start() {
    std::cout << "Server started, waiting for connections..." << std::endl;
    sockaddr_in client_address;
    int client_address_len = sizeof(client_address);

    fd_set master_set, read_fds;
    FD_ZERO(&master_set);
    FD_SET(server_socket, &master_set);

    while (true) {
        read_fds = master_set;

        if (select(0, &read_fds, nullptr, nullptr, nullptr) == SOCKET_ERROR) {
            std::cerr << "Select failed" << std::endl;
            break;
        }

        for (int i = 0; i < read_fds.fd_count; ++i) {
            SOCKET sock = read_fds.fd_array[i];
            if (sock == server_socket) {
                SOCKET client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_len);
                if (client_socket == INVALID_SOCKET) {
                    std::cerr << "Accept failed" << std::endl;
                }
                else {
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.push_back(client_socket);
                    FD_SET(client_socket, &master_set);
                    std::thread(&Server::handleClient, this, client_socket).detach();
                }
            }
            else {
                char buffer[1024];
                int valread = recv(sock, buffer, 1024, 0);
                if (valread == 0 || valread == SOCKET_ERROR) {
                    std::cerr << "Client disconnected or error occurred" << std::endl;
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.erase(std::remove(clients.begin(), clients.end(), sock), clients.end());
                    user_manager->removeUser(sock);
                    closesocket(sock);
                    FD_CLR(sock, &master_set);
                }
                else {
                    buffer[valread] = '\0';
                    std::string message = buffer;
                    broadcastMessage(message, sock);
                }
            }
        }
    }
}

void Server::handleClient(SOCKET client_socket) {
    char buffer[1024];
    int valread;
    while ((valread = recv(client_socket, buffer, 1024, 0)) > 0) {
        buffer[valread] = '\0';
        std::string message = buffer;
        broadcastMessage(message, client_socket);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    }
    closesocket(client_socket);
}

void Server::broadcastMessage(const std::string& message, SOCKET sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    std::cout << message << std::endl;

    if (message.find("/login") != std::string::npos)
        handleLogin(message, sender_socket);
    else if (message.find("/registration") != std::string::npos)
        handleRegistration(message, sender_socket);
    else {
        for (SOCKET client_socket : clients) {
            if (client_socket != sender_socket) {
                std::string clients_message = user_manager->getUser(sender_socket).getUsername() + ": " + message;
                send(client_socket, clients_message.c_str(), clients_message.length(), 0);
            }
        }
    }
}

void Server::handleLogin(std::string message, SOCKET client_socket) {
    std::string username = message.substr(7, message.find(' ', 7) - 7);
    std::string password = message.substr(message.find(' ', 7) + 1);

    if (user_manager->authenticate(username, password)) {
        std::string response = "LOGIN SUCCESS";
        user_manager->addUser(username, client_socket);
        clients.push_back(client_socket);
        std::thread(&Server::handleClient, this, client_socket).detach();
        send(client_socket, response.c_str(), response.length(), 0);

    }
    else {
        std::string response = "LOGIN FAILED";
        send(client_socket, response.c_str(), response.length(), 0);
        closesocket(client_socket);
    }
}

void Server::handleRegistration(std::string message, SOCKET client_socket) {
    std::string username = message.substr(14, message.find(' ', 14) - 14);
    std::string password = message.substr(message.find(' ', 14) + 1);

    if (username.find(' ') != std::string::npos || username.find('\'') != std::string::npos || username.find('\"') != std::string::npos || username.find('\\') != std::string::npos || username.find('/') != std::string::npos) {
        std::string response = "Invalid username";
        send(client_socket, response.c_str(), response.length(), 0);
        closesocket(client_socket);
    }

    if (password.find(' ') != std::string::npos || password.find('\'') != std::string::npos || password.find('\"') != std::string::npos || password.find('\\') != std::string::npos || password.find('/') != std::string::npos) {
        std::string response = "Invalid password";
        send(client_socket, response.c_str(), response.length(), 0);
        closesocket(client_socket);
    }

    if (user_manager->checkUsername(username)) {
        std::string response = "REGISTRATION SUCCESS";
        user_manager->regustration(username, password);
        send(client_socket, response.c_str(), response.length(), 0);

    }
    else {
        std::string response = "REGISTRATION FAILED";
        send(client_socket, response.c_str(), response.length(), 0);
        closesocket(client_socket);
    }
    //handleLogin(client_socket);
}