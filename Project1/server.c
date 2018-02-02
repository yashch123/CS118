//Setting up server:
#include<stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include<sys/socket.h>

const int port_num =  80;

int main(int argc, char** argv){

  int sockfd, realsocketfd, port_num;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client;
  char message[1024];


  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
    fprintf(stderr, "\nError in opening socket");

  // memset((char*)&server_addr, 0, sizeof(server_addr)); //Clearing buffer for storing server address

  //Getting in all server information:
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Connect to local host
  server_addr.sin_port = htons(port_num);

  //Binding to socket and port:
  int rtc = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(client));
  if(rtc<0)
    fprintf(stderr, "\nError in binding socket and port number");

  //Waiting for client requests:
  listen(sockfd,6);

  //Accepting connection requests:
  rtc = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&client);
  if(rtc<0)
    fprintf(stderr, "\nError in accepting connection");

  //Read clients message:
  rtc = read(realsocketfd, message, 1024);
  if(rtc<0)
    fprintf(stderr,"\nError reading from socket");

  //Dumping message onto console:
  fprintf(stdout, "\nMessage: %s", message);

  return 0;
}

