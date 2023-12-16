#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        perror("Acceptance failed");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    int read_bytes;

    while (1) {
        read_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (read_bytes <= 0) {
            break;
        }

        buffer[read_bytes] = '\0';
        printf("Received: %s", buffer);

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
