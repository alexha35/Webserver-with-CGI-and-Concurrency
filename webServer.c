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

// method that helps write to accesslog.txt
void access_log(char* log) {
  char buffer[10000];

  FILE *f = fopen("logs/accesslog.txt", "r");
  if (f == NULL) {
    perror("file cannot be opened");
      fclose(f);
    exit(1);
  }
  else{
    fread(buffer, sizeof(buffer), 1, f);
    fclose(f);
  }

  strcat(buffer, log);
  f = fopen("logs/accesslog.txt", "w+");
  fputs(buffer,f);
  fclose(f);
}

// method that helps write to errorlog.txt
void error_log(char* log) {
  char buffer[10000];

  FILE *f = fopen("logs/errorslog.txt", "r");
  if (f == NULL) {
    perror("file cannot be opened");
    fclose(f);
    exit(1);
  }
  else{
    fread(buffer, sizeof(buffer), 1, f);
    fclose(f);
  }

  strcat(buffer, log);
  f = fopen("logs/errorslog.txt", "w+");
  fputs(buffer, f);
  fclose(f);
}

int main(int argc, char *argv[]){
  //webpage
  char webpage[20000] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF8\r\n\r\n";

  char htmlheader[]=
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n\r\n";

  char imgheader[]=
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/jpg\r\n\r\n";

  char errorheader[]=
    "HTTP/1.1 404 Ok\r\n"
    "Content-Type: text/html\r\n\r\n"
    "<h1>404 Not Found</h1>";

//Concatenate index.html to webpage[]
char str[100000];
FILE * file;
char c;
file = fopen("index.html", "r");
if (file) {
  while((c = getc(file)) != EOF) {
    strcat(str, &c);
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
  char buffer[10000];
  int fdimg;
  int fd;
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
      memset(buffer,0,10000);
      read(clientSocket,buffer,9999);

      //Debugging
      printf("%s\n",buffer );


      if(strncmp(buffer, "GET /index.html",15 ) == 0 ){
        access_log(buffer);
        write(clientSocket, webpage, sizeof(webpage) - 1);
      }

      else if (!strncmp(buffer, "GET / ", 6)) {
        access_log(buffer);
        write(clientSocket, webpage, sizeof(webpage) - 1);
      }

      else if(strncmp(buffer, "GET /apex.html",14 ) == 0 ){
        access_log(buffer);
        write(clientSocket, htmlheader, sizeof(htmlheader) - 1);
        fd = open("apex.html", O_RDONLY);
        sendfile(clientSocket, fd, NULL, 100000);
        close(fd);
      }

      else if(strncmp(buffer, "GET /fortnite.html",18 ) == 0){
        access_log(buffer);
        write(clientSocket, htmlheader, sizeof(htmlheader) - 1);
        fd = open("fortnite.html", O_RDONLY);
        sendfile(clientSocket, fd, NULL, 10000);
        close(fd);
      }

      else if(strncmp(buffer, "GET /favicon.ico", 16) == 0){
        access_log(buffer);
        write(clientSocket,imgheader, sizeof(imgheader) - 1);
        fdimg = open("/images/favicon.ico", O_RDONLY);
        sendfile(clientSocket,fdimg, NULL, 50000);
        close(fdimg);
      }

      else if(strncmp(buffer, "GET /images/", 12) == 0){
        access_log(buffer);
        write(clientSocket,imgheader, sizeof(imgheader) - 1);

        const char* imgptr = buffer;
        char line[20];
        char *img;
        strcpy(line, imgptr);
        img = strtok(line,"/");
        img=strtok(NULL," ");
        fdimg = open(img, O_RDONLY);
        sendfile(clientSocket,fdimg, NULL, 410035);
        close(fdimg);
      }

      //TODO
      //ADD CGI STUFF HERE (POST AND GET)
      else if(strncmp(buffer, "GET /cgi-bin/", 13) == 0){
        access_log(buffer);
        char *token;
        char game[20];
        token = strtok(buffer, "");
        sscanf(token,"GET /cgi-bin/info.cgi?game=%s", game);
        printf("Game of Choice: %s\n",game );
      }
      else{
        error_log(buffer);
        write(clientSocket, errorheader, sizeof(errorheader)-1);
      }
      close(clientSocket);
      exit(0);
    }
    close(clientSocket);
  }
  return 0;
}
