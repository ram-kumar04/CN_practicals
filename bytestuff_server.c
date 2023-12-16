#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define MAX_MESSAGE_SIZE 1024

void byteUnstuff(char* input, char* output) {
    int inputLength = strlen(input);
    int outputIndex = 0;
    int escapeFlag = 0;

    for (int i = 0; i < inputLength; i++) {
        if (input[i] == 0x7D) {
            // Check for escape byte
            escapeFlag = 1;
        } else if (escapeFlag == 1) {
            // Handle escape sequence
            output[outputIndex++] = input[i] ^ 0x20; // XOR with 0x20 to unescape
            escapeFlag = 0;
        } else {
            output[outputIndex++] = input[i];
        }
    }

    output[outputIndex] = '\0'; // Null-terminate the output string
}

int main() {
    int serversocket, clientsocket, bindstatus;
    char servermessage[MAX_MESSAGE_SIZE];
    char unstuffedMessage[MAX_MESSAGE_SIZE];

    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        printf("Socket failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0) {
        printf("Binding failed\n");
        return -1;
    } else {
        printf("Binding is successful\n");
    }

    listen(serversocket, 3);
    printf("Waiting for client connection...\n");

    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0) {
        printf("Connection is rejected by server\n");
        return -1;
    } else {
        printf("Connection is accepted\n");
    }

    while (1) {
        // Receive a message from the client
        memset(servermessage, 0, sizeof(servermessage));
        ssize_t bytes_received = recv(clientsocket, servermessage, sizeof(servermessage), 0);

        // Perform byte unstuffing to handle flag bytes (0x7E)
        byteUnstuff(servermessage, unstuffedMessage);

        printf("Client: %s\n", unstuffedMessage);

        if (strcmp(unstuffedMessage, "bye") == 0) {
            printf("Client said goodbye. Exiting...\n");
            break;
        }
        break;
    }

    close(clientsocket);
    close(serversocket);

    return 0;
}