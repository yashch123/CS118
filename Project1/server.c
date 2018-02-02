//Setting up server:
#include<stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

const int port_num = 2050;

int main(int argc, char** argv){

  int sockfd, realsocketfd;
  struct sockaddr_in server_addr;
  
  
  char message[1024];


  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
   fprintf(stderr, "\nError in opening socket");
   exit(1);
  }
  //Getting in all server information:
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY; //Connect to local host
  server_addr.sin_port = htons(port_num);

  fprintf(stderr, "Port address: %d", server_addr.sin_port);
  //Binding to socket and port:

  int rtc = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(rtc<0){
    fprintf(stderr, "\nError in binding socket and port number");
    exit(1);
  }
  
  //Waiting for client requests:
  fprintf(stderr, "%d\n", sockfd);
  listen(sockfd,5);
  //fprintf(stdout, "\nFirst");
  //Accepting connection requests:
  while(1){
	struct sockaddr_in client_addr;
	socklen_t client = sizeof(client_addr);
	rtc = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&client);	
	//fprintf(stdout, "\nHERE");
	if(rtc<0){
	    fprintf(stderr, "\nError in accepting connection");
	    exit(1);
	}else
	    fprintf(stdout, "\nConnected");
  }
/*
  //Read clients message:
  rtc = read(realsocketfd, message, 1024);
  if(rtc<0)
    fprintf(stderr,"\nError reading from socket");

  //Dumping message onto console:
  fprintf(stdout, "\nMessage: %s", message);
*/
   
	return 0;
}

