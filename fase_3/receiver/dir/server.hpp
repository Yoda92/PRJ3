#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/time.h>

#define PORT 8080

class server {
  public: 
    static int init(void);
    static int receiveMessage(void);
    static char buffer[100];
    static int socketfd;
  private:
    static int conf;
    static struct sockaddr_in serveraddr, clientaddr;
    static struct timeval timeout;
};