// names: Alex Ha, Kennaa Yared
// x500: haxxx185, yared006

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


int main(){

  //http header
  char headerHTTP[10000] =
    "HTTP/1.1 200 ok\r\n"
    "Content_Type: text/html; charset=UTF8\r\n\r\n";

  //Open HTML file
  //TODO

  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sin_len = sizeof(clientAddress);
  int serverSocket, clientSocket;
  char buf[20000];
  int img;
  int active = 1;

  //Create socket

  serverSocket = socket(AF_INET,SOCK_STREAM,0);
  if(serverSocket < 0){
    perror("Failed: ");
    exit(1);
  }

  setsockopt(serverSocket,SOL_SOCKET, SO_REUSEADDR, &active, sizeof(active));

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(8080);




  //bind to socket

  int bindSocket;
  bindSocket = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
  if(bindSocket < 0){
    perror("Failed: ");
    close(serverSocket);
    exit(1);
  }

  //listen to socket

  int servListen;
  servListen = listen(serverSocket,5);
  if(servListen < 0){
    perror("Failed: ");
    close(serverSocket);
    exit(1);
  }

  //accept socket

  while(1){
    clientSocket = accept(serverSocket,(struct sockaddr *) &clientAddress,&sin_len);
    printf("Got client connection \n");

    if(!fork()){
      close(serverSocket);
      memset(buf,0,20000);
      read(clientSocket,buf,2047);

      write(clientSocket,headerHTTP, sizeof(headerHTTP) - 1);
      close(clientSocket);
    }
    close(clientSocket);
    //close(clientSocket);
  }
}
