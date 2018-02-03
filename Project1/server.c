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
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

const int port_num = 2010;
const char* Filename;
char* ver;
char* conn;
int status=200;
char* stm;
int newfd;

void parse(char* );

int main(int argc, char** argv){

  int sockfd, realsocketfd;
  struct sockaddr_in server_addr;
  
  int pID,new;
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


  //Binding to socket and port:

  int rtc = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(rtc<0){
    fprintf(stderr, "\nError in binding socket and port number");
    exit(1);
  }
  
  //Waiting for client requests:

  listen(sockfd,10);

  //Accepting connection requests:
   while(1){

        struct sockaddr_in client_addr;
	socklen_t client = sizeof(client_addr);

	new = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&client);	
	newfd=new;
	if(new<0){
	    fprintf(stderr, "\nError in accepting connection");
	    exit(1);
	}

	//creating seperate process to keep connection going
	pID = fork();

	if(pID<0)
	  fprintf(stderr,"\nError in creating process");
	if(pID==0){    //child process

	  close(sockfd);
	  memset(message, 0, 1024);
	  rtc = read(new, message, 1024);
	  if(rtc<0)
	    fprintf(stderr,"\nError reading from socket");

	  fprintf(stdout,"\n%s", message);       //TO DO: close socket!!!

	  parse(message);

	  exit(0);
	}
	else
	  close(new);
	  
       
   }

  
   /*
  //Read clients message:
  rtc = read(new, message, 1024);
  if(rtc<0)
    fprintf(stderr,"\nError reading from socket");

  //Dumping message onto console:
  fprintf(stdout, "\nMessage: %s", message);

   */
	return 0;
}

void parse(char* buffer){

  //Use Tokens to get File name
  char* file;
  file = strtok(buffer," ");      //Fine name by looking at entry after first space
  file = strtok(NULL," ");

  file++;                         // get rid of the "/" character
  Filename=file;

  if(*Filename == '\0')
    status=404;

  char* out;
  file = strtok(NULL,"\r\n");
  ver=file;
  out=ver;
  
  int i=0;
  while(i<6){
    file = strtok(NULL,"\r\n");
    i++;
  }

  //Get connection type from request message
  
  conn=file;
  
  int ffd; 
  ffd = open(Filename, O_RDONLY);

  if(ffd<0){
    status=404;
    return;
  }

  //Getting file information
  struct stat buf;
  int filestatus;
  filestatus = fstat(ffd, &buf);

  if (filestatus < 0){
    fprintf(stderr, "Error getting stats\n");
    status=404;
  }


  //Status line

  if(status = 200)
    stm = "OK";
  else
    stm = "NOT FOUND";

  char t1[21];
  sprintf(t1, " %d ", status);
  
  strcat(out,t1);
  strcat(out,stm);
  strcat(out,"\n");

  //HEADER lines
  strcat(out,conn);
  strcat(out,"\n");

  //Date and Time
  time_t rtime;
  struct tm * info;
  time(&rtime);
  info = localtime(&rtime);

  char* date = "Date: ";
  strcat(out,date);
  strcat(out,asctime(info));
  
  //server line
  char* serv = "Server: Charora/2.0\n";
  strcat(out,serv);

  char t2[21];
  char* lastm = "Last-Modified: ";
  strcat(out,lastm);
  time_t mod_time = buf.st_mtime;
  sprintf(t2, "%li", buf.st_mtime);
  strcat(out,asctime(gmtime(&mod_time)));
  
  char t3[20];
  char* conl="Content-Length: ";
  strcat(out,conl);
  sprintf(t3, "%li", buf.st_size);
  strcat(out,t3);
  strcat(out,"\n");    //CHECK!!!!!

  //GETTING FILE EXTENSION
  int typeoffile;
  const char *ext = strrchr(Filename, '.');
  if (!ext || ext == Filename)
    fprintf(stderr, "Some error\n"); //CHANGGEEEEEEE

  if (strcmp(ext+1, "jpeg") == 0 || strcmp(ext+1, "jpg") == 0)
    typeoffile = 0;
  else if (strcmp(ext+1, "gif") == 0)
    typeoffile = 1;
  else if (strcmp(ext+1, "html") == 0 || strcmp(ext+1, "htm") == 0)
    typeoffile = 2;
  else
    fprintf(stderr, "Some random file extension\n");

  //CONCATENATING CONTENT TYPE
  strcat(out,"Content-Type: ");
  if (typeoffile == 0)
    strcat(out,"image/jpeg\n\n");
  else if (typeoffile == 1)
    strcat(out,"image/gif\n\n");
  else if (typeoffile == 2)
    strcat(out,"text/html\n\n");
  else
    fprintf(stderr, "Some random type\n");
  
  fprintf(stdout,"\n%s",out);
 
  int size;
  char *br = malloc(sizeof(char) * (buf.st_size));

  /*
    Need to send three lines of header first
    CODE
    CONTENT TYPE
    \r\n
   */
  write(newfd, out, strlen(out));
  size=read(ffd, br,buf.st_size);
  send(newfd,br,buf.st_size+1,0);
  close(newfd);
  return ;
}
