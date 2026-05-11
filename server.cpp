#include <iostream>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

const short PORT = 8080;

int main()
{

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
	perror("socket failed");
	return 1;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
	perror("bind failed");
	close(serverSocket);
	return 1;
    }

    if (listen(serverSocket, 5) < 0) {
	perror("listen failed");
	return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
	perror("accept failed");
	close(serverSocket);
	return 1;
    }

    char buffer[1024] = { 0 };
    ssize_t bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
	std::cout << "Message from client: " << buffer << '\n';
    } else if (bytes == 0) {
	std::cout << "Client disconnected\n";
    } else {
	perror("recv failed");
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
