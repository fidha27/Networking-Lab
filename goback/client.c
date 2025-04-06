#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define WINDOW 4
#define TOTAL_PACKET 10

int main(void)
{
  int server_sock,client_sock;
  struct sockaddr_in server_addr,client_addr;
  int base=0,next_seq=0,ack,i;
  
  client_sock=socket(AF_INET,SOCK_STREAM,0);
  if(client_sock<0)
  {
    perror("Cant created\n");
    exit(1);
  }
  printf("Created succesfully!\n");
  
  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(2000);
  server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  
  if(connect(client_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
  {
    perror("Cant connect\n");
    exit(1);
  }
  printf("Connected to server\n");
  
  srand(time(NULL));
  
  while(base<TOTAL_PACKET)
  {
    for(i=0;i<WINDOW&&(base+i)<TOTAL_PACKET;i++)
    {
      if(rand()%4!=0)
      {
        next_seq=base+i;
        send(client_sock,&next_seq,sizeof(next_seq),0);
        
        printf("sent sequences:%d\n",next_seq);
        
      }
      else
      {
        printf("Timeout at %d",base+i);
      }
      
    }
    if(recv(client_sock,&ack,sizeof(ack),0)>0)
    {
      printf("recieves ack:%d\n",ack);
       base=ack;
    }
    else
    {
      printf("imeout:%d",base);
    }
  }
 close(client_sock);
 
 return 0;
}
