#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    char filename[BUFFER_SIZE];
    FILE *file;
    int n;
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        n = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (n <= 0) break;
        if (strncmp(buffer, "UPLOAD", 6) == 0) {  
            recv(client_socket, filename, BUFFER_SIZE, 0);
            file = fopen(filename, "wb");
            if (!file) {
                perror("File open failed");
                continue;
            }
            while ((n = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
                if (strncmp(buffer, "EOF", 3) == 0) break;
                fwrite(buffer, sizeof(char), n, file);
            }
            fclose(file);
            printf("File %s uploaded successfully.\n", filename);
        } else if (strncmp(buffer, "DOWNLOAD", 8) == 0) {
           
            recv(client_socket, filename, BUFFER_SIZE, 0);
            file = fopen(filename, "rb");
            if (!file) {
                perror("File not found");
                continue;
            }
            while ((n = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
                send(client_socket, buffer, n, 0);
            }
            send(client_socket, "EOF", 3, 0); 
            fclose(file);
            printf("File %s downloaded successfully.\n", filename);
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            break;
        }
    }
    close(client_socket);
}
int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) >= 0) {
        printf("Client connected.\n");
        handle_client(client_socket);
    }
    close(server_socket);
    return 0;
}

