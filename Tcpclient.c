#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>  
#include <arpa/inet.h>  

int main() {
    char buf[100];
    int k;
    int sock_desc;
    struct sockaddr_in server;

    
    memset(&server, 0, sizeof(server));
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        perror("Error in socket creation");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server.sin_port = 3002;

    
    k = connect(sock_desc, (struct sockaddr*)&server, sizeof(server));
    if (k == -1) {
        perror("Error in connecting to server");
        exit(1);
    }
    printf("Connected to the server\n");

    while (1) {
        printf("\nEnter data to be sent to the server: ");
        fgets(buf, 100, stdin);

        buf[strcspn(buf, "\n")] = 0;

        
        if (strncmp(buf, "end", 3) == 0)
            break;

        
        k = send(sock_desc, buf, strlen(buf) + 1, 0); 
        if (k == -1) {
            perror("Error in sending");
            exit(1);
        }

        
        k = recv(sock_desc, buf, 100, 0);
        if (k == -1) {
            perror("Error in receiving");
            exit(1);
        }
        buf[k] = '\0'; 

        
        printf("Message from server: %s\n", buf);
    }

   
    close(sock_desc);
    printf("Connection closed\n");
    return 0;
}

