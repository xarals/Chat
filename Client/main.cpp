#include "Client.h"
#include <iostream>

int main() {
	Client client("127.0.0.1", 8085);
	client.connectToServer();

	std::string message;
	while (true) {
		std::getline(std::cin, message);
		client.sendMessage(message);
	}
	
	return 0;
}