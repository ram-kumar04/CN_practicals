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

    int n, f;
    printf("Enter the size: ");
    scanf("%d", &n);

    printf("\nEnter the frame size: ");
    scanf("%d", &f);

    send(clientsocket, &n, sizeof(n), 0);
    send(clientsocket, &f, sizeof(f), 0);

    int j=0,ack=0,pre=-1,count=1,c=1;
     
      for(int i=0;i<n+f;i++)
      {
         if(i<f)
         {
            send(clientsocket,&i,sizeof(j), 0);
            printf("\nbit sent:%d",i);
         }
         else
         {
       
            recv(clientsocket,&ack,sizeof(j), 0);
           c=1;
            if(ack!=pre+1&&count!=2)
             {
              
                 i=pre+1;
                 count=2;
                 c=2;

                 printf("\n ack not received for the bit  =%d",ack+1);
             }
             if(c!=2&&ack<n)
            printf("\n ack received:%d",ack);
            if(i<n){
            send(clientsocket,&i,sizeof(ack), 0);
            printf("\nbit sent:%d",i);
            }
            pre++;
         }
      }



    close(clientsocket);

    return 0;
}