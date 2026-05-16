#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

const short PORT = 8080;

int main() {
  int clientSocket = socket(AF_INET6, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    perror("socket failed");
    return 1;
  }

  sockaddr_in6 serverAddress;
  serverAddress.sin6_family = AF_INET6;
  serverAddress.sin6_port = htons(PORT);

  if (inet_pton(AF_INET6, "::1", &serverAddress.sin6_addr) <= 0) {
    perror("Invalid address / Address not supported.");
    close(clientSocket);
    return 1;
  }

  if (connect(clientSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) < 0) {
    perror("connect failed");
    close(clientSocket);
    return 1;
  }

  std::cout << "Enter message for send: ";
  char message[1024];
  std::cin.getline(message, sizeof(message));

  if (send(clientSocket, message, strlen(message), 0) < 0) {
    perror("send failed");
    close(clientSocket);
    return 1;
  }

  close(clientSocket);
  return 0;
}
