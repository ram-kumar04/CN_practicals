#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N 

void ogdata(char *output,char *final){

    int outputLength = strlen(output);
    int finalIndex = 0;
    int consecutiveOnes = 0;
    int skipNextZero = 0;

    for (int i = 0; i < outputLength; i++) {
        if (skipNextZero) {
            // Skip the '0' that follows five consecutive '1s'
            skipNextZero = 0;
            continue;
        }

        if (output[i] == '1') {
            consecutiveOnes++;

            if (consecutiveOnes == 5) {
                skipNextZero = 1;         // Reset consecutiveOnes counter
            }
        } else {
            consecutiveOnes = 0;  // Reset consecutiveOnes counter if a '0' is encountered
        }
        final[finalIndex++] = output[i];
    }

    final[finalIndex] = '\0';  // Null-terminate the output string

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
    printf("waiting for client connection...\n");

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
    char output[256];
    char final[256];
    recv(clientsocket,output,sizeof(output),0);
    printf("Date recieved after bit stuffing: %s\n",output);
    ogdata(output,final);

    printf("Retrieved original data: %s\n",final);
    close(serversocket);
    return 0;
}