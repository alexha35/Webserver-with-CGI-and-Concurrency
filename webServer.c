// names: Alex Ha, Kennaa Yared
// x500: haxxx185, yared006

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
  //webpage
  char webpage[20000] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF8\r\n\r\n";

  char imgheader[]=
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/jpeg\r\n\r\n";

  char errorheader[]=
    "HTTP/1.1 200 Ok\r\n"
    "Content-Type: text/html\r\n\r\n"
    "<h1>404 Not Found</h1>";

char str[100000];
FILE * file;
char c;
file = fopen("index.html", "r");
if (file) {
  while((c = getc(file)) != EOF) {
    strcat(str, &c);
    //printf("%c",c);
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
  char buffer[100000];
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
    close(serverSocket);
    exit(1);
  }

  //Listen
  listenSuccess = listen(serverSocket, 10);
  if(listenSuccess < 0){
    perror("Listen: ");
    close(serverSocket);
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
      memset(buffer,0,200000);
      read(clientSocket,buffer,199999);

      //Debugging
      printf("%s\n",buffer );

      if(strncmp(buffer, "GET /images/", 12) == 0){
        write(clientSocket,imgheader, sizeof(imgheader) - 1);
        char pic = buffer[12];
        char img[20];
        strcpy(img, "/images/");
        img[10] = pic;
        printf("%s\n", img);
        fdimg = open(img, O_RDONLY);
        sendfile(clientSocket,fdimg, NULL, 100000);
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
