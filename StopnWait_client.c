#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000

int main()
{
    struct sockaddr_in serveraddress;

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocket<0)
    {
        printf("socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if(connection_status < 0)
    {
        printf("connection failed\n");
        return -1;
    }
    else
        printf("connection established\n");

    
    int count = 0;
    char data[20];
    int k;

    recv(clientsocket, &k, sizeof(k), 0);

    for(int i = 0; i < k; i++)
    {
        recv(clientsocket, &data[i], sizeof(data[i]), 0);
        printf("Received bit %d from server\n", i+1);

        if(i == 3)
        {
            int x;
            printf("Press 1 if you want to send ack, else press 2: ");
            scanf("%d", &x);

            if(x == 1)
            {
                count++;
                send(clientsocket, &count, sizeof(count) ,0);
            }
            else
                count++;
        }
        else
        {
            count++;
            send(clientsocket, &count, sizeof(count), 0);
        }
    }

    printf("\nData received from the server is %s ", data);


    close(clientsocket);

    return 0;
}