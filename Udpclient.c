#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>  // For inet_addr
#include <unistd.h>     // For close()

#define MAX 80
#define PORT 43454
#define SA struct sockaddr

int main() {
    char buff[MAX];
    int sockfd, len, n;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created...\n");
    }

    bzero(&servaddr, sizeof(servaddr));  // Corrected this line
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    len = sizeof(servaddr);

    for (;;) {
        printf("\nEnter string: ");
        n = 0;
        // Safe input handling using fgets
        fgets(buff, sizeof(buff), stdin);
        buff[strcspn(buff, "\n")] = '\0';  // Remove the newline character

        // Send message to the server
        sendto(sockfd, buff, strlen(buff), 0, (SA *)&servaddr, len);

        bzero(buff, sizeof(buff));  // Clear the buffer before receiving

        // Receive server's response
        recvfrom(sockfd, buff, sizeof(buff), 0, (SA *)&servaddr, &len);
        printf("From server: %s\n", buff);

        if (strncmp("exit", buff, 4) == 0) {
            printf("Client exit...\n");
            break;
        }
    }

    close(sockfd);  // Close the socket
}

