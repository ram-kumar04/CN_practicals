#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define MAX_MESSAGE_SIZE 1024

void byteStuff(char* input, char* output) {
    int inputLength = strlen(input);
    int outputIndex = 0;

    // Start flag byte
    output[outputIndex++] = 0x7E;

    for (int i = 0; i < inputLength; i++) {
        if (input[i] == 0x7E) {
            // If the byte is 0x7E, escape it by adding another 0x7E
            output[outputIndex++] = 0x7D;
            output[outputIndex++] = 0x5E; // Transformed 0x7E
        } else {
            output[outputIndex++] = input[i];
        }
    }

    // End flag byte
    output[outputIndex++] = 0x7E;

    output[outputIndex] = '\0'; // Null-terminate the output string
}

int main() {
    struct sockaddr_in serveraddress;
    char message[MAX_MESSAGE_SIZE];
    char stuffedMessage[MAX_MESSAGE_SIZE];

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket < 0) {
        printf("Socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (connection_status < 0) {
        printf("Connection failed\n");
        return -1;
    } else {
        printf("Connection established\n");
    }

    printf("Enter Data: ");
    scanf("%s", message);

    byteStuff(message, stuffedMessage);

    printf("After byte stuffing: %s\n", stuffedMessage);

    send(clientsocket, stuffedMessage, strlen(stuffedMessage), 0);

    close(clientsocket);

    return 0;
}