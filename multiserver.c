#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

void *handle_client(void *client_socket);

int main() {
    int server_fd, new_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int client_count = 1;  // Keep track of connected clients

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    
    addr_len = sizeof(client_addr);
    
    while (1) {
        // Accepting incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Create a new thread to handle the client
        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));  // Allocate memory to pass socket to the thread
        *new_sock = new_socket;

        // Send "Client X connected" message
        printf("Client %d connected!\n", client_count);
        
        // Create a thread for the client
        pthread_create(&thread_id, NULL, handle_client, (void *)new_sock);
        
        // Increment client count for next client
        client_count++;
    }

    return 0;
}

void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    char buffer[1024] = {0};
    int valread;

    while (1) {
        valread = read(sock, buffer, sizeof(buffer));
        if (valread == 0) {
            printf("Client disconnected.\n");
            break;
        }
        
        buffer[valread] = '\0';
        printf("Received: %s\n", buffer);
        
        // Echo the message back to the client
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    free(client_socket);  // Free the memory allocated for the client socket
    return NULL;
}

