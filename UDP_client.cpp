#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

const short PORT = 8080;

int main() {

  int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (clientSocket < 0) {
    perror("socket failed");
    return 1;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

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
