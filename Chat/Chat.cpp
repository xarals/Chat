#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define MAX_CLIENTS 10

int main()
{
    std::cout << "Hello World!\n";
}

