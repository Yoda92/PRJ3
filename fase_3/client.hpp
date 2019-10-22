#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define PORT 8080

class client {
  public: 
    static void init(void);
    static void sendMessage();
    static char buffer[100];
  private:
    static int socketfd;
    static struct sockaddr_in serveraddr;
};