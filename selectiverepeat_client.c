#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N 100 // Assuming N is the window size for selective repeat

int main() {
    struct sockaddr_in serveraddress;

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket < 0) {
        printf("socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (connection_status < 0) {
        printf("connection failed\n");
        return -1;
    } else
        printf("connection established\n");

    int count = 0;
    char data[20];
    int k;

    recv(clientsocket, &k, sizeof(k), 0);

    int window_start = 0;
    int window_end = N - 1;

    while (window_start < k) {
        for (int i = window_start; i <= window_end && i < k; i++) {
            recv(clientsocket, &data[i], sizeof(data[i]), 0);
            printf("Received bit %d from server\n", i + 1);
        }

        for (int i = window_start; i <= window_end && i < k; i++) {
            int x;
            do {
                printf("+ve ack - 1/-ve ack - 2: ");
                scanf("%d", &x);

                if (x == 1) {
                    count = i + 1; // Positive acknowledgment received
                    send(clientsocket, &count, sizeof(count), 0);
                    window_start++;
                    break; // Exit the loop after sending acknowledgment

                } else if (x == 2) {
                    // Negative acknowledgment received or invalid input
                    send(clientsocket, &count, sizeof(count), 0);
                    // Continue to the next bit without waiting for acknowledgment
                    break;

                } else {
                    printf("Invalid input. Please enter 1 or 2.\n");
                }
            } while (1); // Keep asking for acknowledgment until valid input is received
        }
    }

    printf("\nData received from the server is %s \n", data);

    close(clientsocket);
    return 0;
}