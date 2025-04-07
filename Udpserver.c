#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>  // for close()

#define MAX 80
#define PORT 43454
#define SA struct sockaddr

void func(int sockfd) {
    char buff[MAX];
    int n, clen;
    struct sockaddr_in cli;
    clen = sizeof(cli);
    
    for (;;) {
        bzero(buff, MAX);  // Clear the buffer

        // Receive message from the client
        recvfrom(sockfd, buff, sizeof(buff), 0, (SA *)&cli, &clen);
        printf("From client: %s\tTo client: ", buff);

        bzero(buff, MAX);  // Clear the buffer before taking new input
        n = 0;
        
        // Input response from the server
        while ((buff[n++] = getchar()) != '\n');
        
        // Send message back to the client
        sendto(sockfd, buff, n, 0, (SA *)&cli, clen);
        
        // Check for "exit" message to break the loop
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit\n");
            break;
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed ....\n");
        exit(0);
    } else {
        printf("Socket successfully created....\n");
    }

    // Prepare server address structure
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed\n");
        exit(0);
    } else {
        printf("Socket successfully binded\n");
    }

    // Start communication with clients
    func(sockfd);

    // Close the socket
    close(sockfd);
}

