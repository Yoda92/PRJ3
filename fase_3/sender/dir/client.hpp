#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdint.h>
#include <sys/time.h>

#define PORT 8080

class client {
  public: 
    static int init(void);
    static int sendMessage(uint8_t);
    static char buffer[100];
    static int socketfd;
  private:
    static struct timeval timeout;  
    static struct sockaddr_in serveraddr;
};