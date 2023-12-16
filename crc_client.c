#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N strlen(crc_key)

void XOR();
int i, j;
char data[20];
char crc_key[20];
char checksum[20];

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


    printf("Enter the data to be transmitted: ");
    scanf("%s", data);

    printf("Enter the CRC key: ");
    scanf("%s", crc_key);

    int data_length = strlen(data);

    for(i = data_length; i<data_length+N-1; i++)
        data[i] = '0';
    printf("new data = %s\n", data);

    for(i = 0; i<N; i++)
        checksum[i] = data[i];
    
    do
    {
        if(checksum[0] == '1')
            XOR();
        for(j = 0; j<N-1; j++)
            checksum[j] = checksum[j+1];
        checksum[j] = data[i++];
    } while (i <= data_length+N-1);

    printf("The check value is: %s\n", checksum);
    
    for(i=data_length; i<data_length+N-1; i++)
        data[i] = checksum[i-data_length];
    
    printf("The final data to be sent is: %s\n", data);

    send(clientsocket, data, sizeof(data), 0);
    send(clientsocket, crc_key, sizeof(crc_key), 0);
    send(clientsocket, checksum, sizeof(checksum), 0);

    /*char error_msg[30];
    recv(clientsocket, error_msg, sizeof(error_msg), 0);
    printf("Error status: %s\n", error_msg);*/


    close(clientsocket);

    return 0;
}

void XOR()
{
    for(j=1; j<N; j++)
        checksum[j] = ((checksum[j] == crc_key[j]) ? '0' : '1');
}