#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    char message[MAX_BUFFER_SIZE];

    while (1) {
        printf("Enter a message to send (or 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);

        if (strncmp(message, "exit", 4) == 0) {
            break;
        }

        send(client_socket, message, strlen(message), 0);

        char buffer[MAX_BUFFER_SIZE];
        int read_bytes;

        read_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (read_bytes <= 0) {
            break;
        }

        buffer[read_bytes] = '\0';
        printf("Server response: %s", buffer);
    }

    close(client_socket);

    return 0;
}
