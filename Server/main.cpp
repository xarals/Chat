#include "Server.h"

int main() {
	Server server(8085);
	server.start();
	return 0;
}