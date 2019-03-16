// names: Alex Ha, Kennaa Yared
// x500: haxxx185, yared006

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
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
  char buf[2048];
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
    if(clientSocket < 0){
      perror("Failed: ");
      continue;
    }
    printf("Got client connection \n");

    if(!fork()){//child process
      close(serverSocket);
      memset(buf,0,2048);
      read(clientSocket,buf,2047);

      if(!strncmp(buf, "GET /a.jpg",16)){
        img = open("a.jpg", O_RDONLY);
        write(clientSocket,headerHTTP, sizeof(headerHTTP) - 1);
        sendfile(clientSocket,img, NULL, 51835);
        close(img);
      }
      else{
        write(clientSocket,headerHTTP, sizeof(headerHTTP) - 1);
      }
      close(clientSocket);

    }
    //parent process
    close(clientSocket);
    exit(0);
  }
  return 0;
}
*/

int main(int argc, char *argv[]){
  //webpage
  char webpage[10000] =
    "HTTP/1.1 200 OK\r\n"
    "Content_Type: text/html; charset=UTF8\r\n\r\n";

  char imgheader[]=
    "HTTP/1.1 200 OK\r\n"
    "Content_Type: image/jpeg\r\n\r\n";

  char errorheader[]=
    "HTTP/1.1 200 Ok\r\n"
    "Content-Type: text/html\r\n\r\n"
    "<h1>404 Not Found!</h1>";
/*
  //read index.html file
  char webAddOn[8000];
  FILE *file;
  file = fopen("index.html", "r");
  int count = 0;
  if(file == NULL){
    fprintf(stderr, "%s\n","There was an error opening file" );
  }
  char c = (char)fgetc(file);
  while(c != EOF){
    strcat(webAddOn, &c);
    printf("%c\n",c );
  }
  fclose(file);
  strcat(webpage,webAddOn);
  */
char str[10000];
FILE * file;
char c;
file = fopen("index.html", "r");
if (file) {
  while((c = getc(file)) != EOF) {
    strcat(str, &c);
    printf("%c",c);
  }
  fclose(file);
}

strcat(webpage, str);

  //All necessary variables (sockets, sockaddr_in, buffer)
  int serverSocket;
  int clientSocket;
  int bindsuccess;
  int listenSuccess;
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  socklen_t sinLen = sizeof(clientAddress);
  char buffer[4096];
  int fdimg;
  int status = 1;

  //socket
  serverSocket = socket(AF_INET,SOCK_STREAM, 0);
  if(serverSocket < 0){
    perror("Socket:");
    exit(1);
  }

  //initiate sockaddr_in
  setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR, &status, sizeof(int));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(8080);

  //bindSocket
  bindsuccess = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
  if(bindsuccess < 0){
    perror("Bind: ");
    exit(1);
  }

  //Listen
  listenSuccess = listen(serverSocket, 10);
  if(listenSuccess < 0){
    perror("Listen: ");
    exit(1);
  }

  //Accept
  while(1){
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &sinLen);
    if(clientSocket < 0){
      perror("Accept: ");
      continue;
    }
    //debug
    printf("%s\n","Connected");

    //child process
    if(fork() == 0){
      close(serverSocket);
      memset(buffer,0,4096);
      read(clientSocket,buffer,4095);

      if(strncmp(buffer, "GET /images/", 12) == 0){
        write(clientSocket,imgheader, sizeof(imgheader) - 1);
        fdimg = open("/images/ApexLegends.jpeg", O_RDONLY);
        //sendfile(clientSocket,fdimg, NULL, 55120);
        close(fdimg);
      }
      else{
        write(clientSocket, errorheader, sizeof(errorheader)-1);
      }
      close(clientSocket);
      exit(1);
    }
    close(clientSocket);
  }

  return 0;
}
