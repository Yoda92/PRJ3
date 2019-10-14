#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 8080 
#define MAXLINE 1024 

int sockfd;
struct sockaddr_in clientaddr, servaddr; 

void initClient(void);
void sendMessage(char *);