#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
void upload_file(int socket, const char *filename) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File not found");
        return;
    }
    send(socket, "UPLOAD", 6, 0);
    send(socket, filename, strlen(filename) + 1, 0);
    while (fread(buffer, sizeof(char), BUFFER_SIZE, file) > 0) {
        send(socket, buffer, BUFFER_SIZE, 0);
    }
    send(socket, "EOF", 3, 0); 
    printf("File %s uploaded successfully.\n", filename);
}
void download_file(int socket, const char *filename) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(filename, "wb");
    int n;
    if (!file) {
        perror("File creation failed");
        return;
    }
    send(socket, "DOWNLOAD", 8, 0);
    send(socket, filename, strlen(filename) + 1, 0);
    while ((n = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        if (strncmp(buffer, "EOF", 3) == 0) break;
        fwrite(buffer, sizeof(char), n, file);
    }
    fclose(file);
    printf("File %s downloaded successfully.\n", filename);
}
int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char command[BUFFER_SIZE];
    char filename[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    while (1) {
        printf("Enter command (UPLOAD/DOWNLOAD/QUIT): ");
        scanf("%s", command);
        if (strncmp(command, "UPLOAD", 6) == 0) {
            printf("Enter filename to upload: ");
            scanf("%s", filename);
            upload_file(client_socket, filename);
        } else if (strncmp(command, "DOWNLOAD", 8) == 0) {
            printf("Enter filename to download: ");
            scanf("%s", filename);
            download_file(client_socket, filename);
        } else if (strncmp(command, "QUIT", 4) == 0) {
            send(client_socket, "QUIT", 4, 0);
            break;
        }
    }
    close(client_socket);
    return 0;
}

