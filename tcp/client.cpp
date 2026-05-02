#include <cstring>
#include <iostream>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080



int main() {

    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Sock creation error" << '\n';
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Adress not supported" << '\n';
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << '\n';
        return -1;
    }

    send(sock, hello, strlen(hello), 0);
    std::cout << "Message sent" << '\n';
    
    close(sock);

    return 0;
}