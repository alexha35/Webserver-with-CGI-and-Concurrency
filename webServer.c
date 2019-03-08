// names: Alex Ha, Kennaa Yared

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[]){

  //Opening HTML file
  FILE *html_file;
  html_file = fopen("index.html","r");

  char response_buffer[1024];
  fgets(response_buffer,1024,html_file);

  //http header
  char headerHTTP[2048] = "HTTP/1.0 200 ok\r\n\n";
  strcat(headerHTTP,response_buffer);

  //Create socket

  int serverSocket;
  serverSocket = socket(AF_INET,SOCK_STREAM,0);
  if(serverSocket < 0){
    perror("Failed: ");
    exit(1);
  }

  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(8001);


  //bind to socket

  int bindSocket;
  bindSocket = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
  if(bindSocket < 0){
    perror("Failed: ");
    exit(1);
  }

  //listen to socket

  listen(serverSocket,5);

  //accept socket

  int clientSocket;

  while(1){
    clientSocket = accept(serverSocket,NULL,NULL);
    send(clientSocket,headerHTTP,sizeof(headerHTTP),0);
    //close(clientSocket);
  }
  return 0;
}
