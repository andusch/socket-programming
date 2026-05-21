#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

const short PORT = 8080;

int main() {

  int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

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

  if (bind(serverSocket, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) < 0) {
    perror("bind failed");
    close(serverSocket);
    return 1;
  }

  std::cout << "Server listening on port " << PORT << "...\n";

  char buffer[1024] = {0};
  sockaddr_in clientAddr{};
  socklen_t clientLen = sizeof(clientAddr);

  ssize_t bytes = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                           (struct sockaddr *)&clientAddr, &clientLen);

  if (bytes > 0) {
    buffer[bytes] = '\0';
    std::cout << "Message from client: " << buffer << '\n';
  }

  close(serverSocket);

  return 0;
}
