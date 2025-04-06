#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define WINDOW 4
#define TOTAL_PACKET 10

int main(void)
{
  int server_sock,client_sock;
  struct sockaddr_in server_addr,client_addr;
  int seq,expected_seq=0,ack;
  struct timeval timeout={3,0};
  socklen_t client_size=sizeof(client_addr);
  
  server_sock=socket(AF_INET,SOCK_STREAM,0);
  if(server_sock<0)
  {
    perror("Cant created\n");
    exit(1);
  }
  printf("Created succesfully!\n");
  
  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(2000);
  server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  
  if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
  {
    perror("Cant bind\n");
    exit(1);
  }
  printf("BInd the scoket\n");
  
  if(listen(server_sock,5)<0)
  {
    perror("Listening failed\n");
    exit(1);
  }
  printf("Listening .....\n");
  
  client_sock=accept(server_sock,(struct sockaddr*)&client_addr,&client_size);
  if(client_sock<0)
  {
    perror("Accept f ailed\n");
    exit(1);
  }
  printf("Client connectd\n");
  
  
  setsockopt(client_sock,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
  
  while(expected_seq<TOTAL_PACKET)
  {
    if(recv(client_sock,&seq,sizeof(seq),0)>0)
    {
      printf("sent seq:%d\n",seq);
      if(seq==expected_seq)
      {
        expected_seq++;
      }
      ack=expected_seq;
      send(client_sock,&ack,sizeof(ack),0);
      printf("Sent ack:%d\n",ack);
    }
    else
    {
      printf("Timeout!!!\n");
    }
    
    
  }
  close(client_sock);
    close(server_sock);
    
    return 0;
}
