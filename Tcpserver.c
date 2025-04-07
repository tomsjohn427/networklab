#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h> 
#include <arpa/inet.h>  

int main() {
    char buf[100];
    int k;
    socklen_t len;
    int sock_desc, temp_sock_desc;
    struct sockaddr_in server, client;

    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        perror("Error in socket creation");
        exit(1);
    }
    printf("Socket created successfully\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  
    server.sin_port = 3002;

    k = bind(sock_desc, (struct sockaddr*)&server, sizeof(server));
    if (k == -1) {
        perror("Error in binding");
        exit(1);
    }
    printf("Socket bound successfully\n");

    k = listen(sock_desc, 20);
    if (k == -1) {
        perror("Error in listening");
        exit(1);
    }
    printf("Server listening on port 3002\n");

    len = sizeof(client);
    temp_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &len);
    if (temp_sock_desc == -1) {
        perror("Error in accepting connection");
        exit(1);
    }
    printf("Connection accepted from client\n");

    while (1) {
        k = recv(temp_sock_desc, buf, 100, 0);
        if (k == -1) {
            perror("recv");
            exit(1);
        }a

        buf[k] = '\0'; 
        printf("Message from client: %s\n", buf);

        printf("Enter data to send to client: ");
        fgets(buf, 100, stdin);
        buf[strcspn(buf, "\n")] = 0;  

        if (strncmp(buf, "end", 3) == 0) {
            break;
        }

        k = send(temp_sock_desc, buf, 100, 0);
        if (k == -1) {
            perror("send");
            exit(1);
        }
    }

    close(temp_sock_desc);
    close(sock_desc); 
    printf("Server terminated\n");
    return 0;
}

