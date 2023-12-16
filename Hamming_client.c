#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000

int i, j;
char data[20], final_data[20], rec_data[20];

void even_parity(int *parity_index, int parity_index_size, int x, char *final_data)
{
    int count = 0;
    for(i = 0; i < parity_index_size; i++)
    {
        int index = parity_index[i];
        if(final_data[index] == '1')
            count++;
    }

    if(count % 2 != 0)
        final_data[x] = '1';
    else
        final_data[x] = '0';

    printf("Final string: %s\n", final_data);
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

    
    printf("Enter the 7 bit data: ");
    scanf("%s", data);

    int data_length = strlen(data);

    strrev(data);

    int current_data_index = 0;
    int final_data_index = 0;

    for(i = 1; i<= data_length + 4; i++)
    {
        if((i & (i-1)) == 0)
        {
            final_data[final_data_index] = '_';
            final_data_index++;
        }
        else
        {
            final_data[final_data_index] = data[current_data_index];
            final_data_index++;
            current_data_index++;
        }
    }

    final_data[final_data_index] = '\0';
    printf("Final data string is: %s\n", final_data);

    int parity_index1[] = {0, 2, 4, 6, 8, 10};
    int parity_index2[] = {1, 2, 5, 6, 9, 10};
    int parity_index3[] = {3, 4, 5, 6};
    int parity_index4[] = {7, 8, 9, 10};

    int parity_index1_size = sizeof(parity_index1) / sizeof(parity_index1[0]);
    int parity_index2_size = sizeof(parity_index2) / sizeof(parity_index2[0]);
    int parity_index3_size = sizeof(parity_index3) / sizeof(parity_index3[0]);
    int parity_index4_size = sizeof(parity_index4) / sizeof(parity_index4[0]);

    even_parity(parity_index1, parity_index1_size, 0, final_data);
    even_parity(parity_index2, parity_index2_size, 1, final_data);
    even_parity(parity_index3, parity_index3_size, 3, final_data);
    even_parity(parity_index4, parity_index4_size, 7, final_data);

    strrev(final_data);

    printf("The string to be transmitted is: %s\n", final_data);

    send(clientsocket, final_data, sizeof(final_data), 0);

    close(clientsocket);

    return 0;
}