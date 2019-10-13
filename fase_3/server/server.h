// Inspired by https://www.geeksforgeeks.org/udp-server-client-implementation-c/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

// Default port 8080  
#define PORT 8080 
#define MAXLINE 1024

// Socket filedescriptor
int sockfd;
// Server- and client address - Structs for connection handling
struct sockaddr_in servaddr, remoteaddr;

char buffer[MAXLINE];

void initServer(void);
char* receiveMessage(void);