#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int sock = 0, valread;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Reading and writing to the server
    while (1) {
        printf("Enter message to send to server: ");
        fgets(buffer, sizeof(buffer), stdin);

        send(sock, buffer, strlen(buffer), 0);

        // Read the server's response
        valread = read(sock, buffer, sizeof(buffer) - 1);
        if (valread == 0) {
            printf("Server closed the connection\n");
            break;
        }

        buffer[valread] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(sock);
    return 0;
}

