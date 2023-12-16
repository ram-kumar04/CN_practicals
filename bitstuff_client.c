#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N 

void bitStuff(char* input, char* output) {
    int inputLength = strlen(input);
    int outputIndex = 0;
    int consecutiveOnes = 0;

    for (int i = 0; i < inputLength; i++) {
        output[outputIndex++] = input[i];

        if (input[i] == '1') {
            consecutiveOnes++;

            if (consecutiveOnes == 5) {
                output[outputIndex++] = '0';  // Add a '0' after 5 consecutive '1s'
                consecutiveOnes = 0;           // Reset consecutiveOnes counter
            }
        } else {
            consecutiveOnes = 0;  // Reset consecutiveOnes counter if a '0' is encountered
        }
    }

    output[outputIndex] = '\0';  // Null-terminate the output string
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

    char input[256];
    char output[256];
    printf("Enter Data: ");
    scanf("%s",input);

    bitStuff(input, output);

    printf("After bit stuffing %s\n", output);
   
    send(clientsocket,output,strlen(output),0);

    close(clientsocket);

    return 0;
}
