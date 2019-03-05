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

char webpage[] =
"HTTP/1.1 200 ok\r\n"
"Content-Type: text/html; charset=UTF-8\r\r\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>LAB 1</title>\r\n"
"<style>body {background-color: #999}</style></head></html>\r\n";


int main(int argc, char *argv[]){

  return 0;
}
