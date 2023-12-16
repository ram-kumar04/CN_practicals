#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N strlen(crc_key)

int i, j;
int count = 0;
char data[20];
char crc_key[20];
char checksum[20];

void XOR()
{
    for(j=1; j<N; j++)
        checksum[j] = ((checksum[j] == crc_key[j]) ? '0' : '1');
}

int main()
{
    int serversocket, clientsocket, bindstatus;
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0)
    {
        printf("socket failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0)
    {
        printf("binding failed\n");
        return -1;
    }
    else
    {
        printf("binding is successful\n");
    }

    listen(serversocket, 3);
    printf("Waiting for client connection...\n");

    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0)
    {
        printf("connection is rejected by server\n");
        return -1;
    }
    else
    {
        printf("connection is accepted\n");
    }


    recv(clientsocket, data, sizeof(data), 0);
    recv(clientsocket, crc_key, sizeof(crc_key), 0);
    recv(clientsocket, checksum, sizeof(checksum), 0);


    printf("Received data is: %s\n", data);

    int data_length = strlen(data);

    for(i=0; i<N; i++)
        checksum[i] = data[i];

    do{
        if(checksum[0] == '1')
            XOR();
        for(j=0; j<N-1; j++)
            checksum[j] = checksum[j+1];
        checksum[j] = data[i++];
    }while(i <= data_length+N-1);

    for(i = 0; i<N; i++)
    {
        if(checksum[i] == '1')
            count++;
        else
            count = 0;
    }

    if(count < 0)
        printf("error detected\n");
    else
        printf("no error detected\n");


    close(serversocket);
    return 0;
}