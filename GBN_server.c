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

    int j=0,f,n,ack=0,count=1,c=1;
        recv(clientsocket,&n,sizeof(n), 0);
        recv(clientsocket,&f,sizeof(f), 0);
      for(int i=0;i<n+f;i++)
      {
         if(i<f)
         {
            recv(clientsocket,&j,sizeof(j), 0);
            printf("\nbit recieved:%d",j);
         }
         else
         {
             c=1;
             if(i==f+2&&count!=2)
             {
                printf("\n do you want to send ack for bit=%d??y/n:",ack);
                char ans;
                scanf("%c",&ans);
                if(ans=='n'&&count!=2)
                {
                  i=ack;
                  ack-=1;
                 count=2;
                 c=2;
                }
             }
            send(clientsocket,&ack,sizeof(ack),0);
            if(c!=2&&ack<n)
            printf("\nsending ack for:%d",ack);
            if(i<n){
            recv(clientsocket,&j,sizeof(j), 0);
            printf("\n bit received:%d",j);
            }
           
            ack++;
         }
      }
    

    close(serversocket);

    return 0;
}