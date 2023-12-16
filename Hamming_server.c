#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



#define PORT 8000

int i, j;
char data[20], final_data[20], rec_data[20];

int parity_check(int *parity_index, int parity_index_size, char *rec_data)
{
    int x;
    int count = 0;
    for(i = 0; i < parity_index_size; i++)
    {
        int index = parity_index[i];
        if(rec_data[index]== '1')
            count++;
    }

    if(count % 2 != 0)
        x = 1;
    else
        x = 0;

    return x;
}

void strrev(char *rec_data)
{
    int len;
    len = strlen(rec_data);
    int start = 0, end = len - 1;
    while(start < end)
    {
        char temp = rec_data[start];
        rec_data[start] = rec_data[end];
        rec_data[end] = temp;
        start++;
        end--;
    }
}

int main()
{
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    int serversocket;
    serversocket = socket(AF_INET, SOCK_STREAM, 0); 
    if(serversocket < 0)
    {
        printf("socket failed");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int bindstatus;
    bindstatus = bind(serversocket, (const struct sockaddr*)&serveraddress, sizeof(serveraddress));
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

    int clientsocket;
    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if(clientsocket<0)
    {
        printf("connection is rejected by server\n");
        return -1;
    } 
    else
        printf("connection is accepted\n");

    recv(clientsocket, final_data, sizeof(final_data), 0);
    printf("Received data: %s\n", final_data);

    printf("Enter the data received: ");
    scanf("%s", rec_data);

    strrev(rec_data);

    int parity_index1[] = {0, 2, 4, 6, 8, 10};
    int parity_index2[] = {1, 2, 5, 6, 9, 10};
    int parity_index3[] = {3, 4, 5, 6};
    int parity_index4[] = {7, 8, 9, 10};

    int parity_index1_size = sizeof(parity_index1) / sizeof(parity_index1[0]);
    int parity_index2_size = sizeof(parity_index2) / sizeof(parity_index2[0]);
    int parity_index3_size = sizeof(parity_index3) / sizeof(parity_index3[0]);
    int parity_index4_size = sizeof(parity_index4) / sizeof(parity_index4[0]);
    
    char parity_results[4];

    parity_results[3] = parity_check(parity_index1, parity_index1_size, rec_data);
    parity_results[2] = parity_check(parity_index2, parity_index2_size, rec_data);
    parity_results[1] = parity_check(parity_index3, parity_index3_size, rec_data);
    parity_results[0] = parity_check(parity_index4, parity_index4_size, rec_data);

    printf("Parity check results: P8: %d, P4: %d, P2: %d, P1: %d\n", parity_results[0], parity_results[1], parity_results[2], parity_results[3]);

    int sum = 0;
    for(i = 0; i < 4; i++)
    {
        int y = sizeof(parity_results) - (i+1);
        sum = sum + parity_results[i]*pow(2, y);
    }

    if(sum == 0)
        printf("\nNo error\n");
    else
        printf("\nError in bit %d\n", sum);

    close(serversocket);

    return 0;
}