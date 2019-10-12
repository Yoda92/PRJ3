#include "server.h"

void initServer(void)
{
        // Create socket file descriptor
    // AF_INET : Domain : IPv4 Protokol (32 bit address)
    // SOCK_DGRAM : Type : Datagram (UDP) (No receive checks and no need for connection) (SOCK_STREAM for TCP)
    // 0 : Protocol : Zero for IP
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        printf("Creating socket failed"); 
        exit(1);
    } 

    // memset(a, b, c) copies char b to the c first charachters in a
    // Ensures server address is empty  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server address
    // AF_INET : Protocol : IPv4
    // INADDR_ANY : Address : All address
    // htons(PORT) : Port : htons() ensures that port address is stored correct
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with server
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
    { 
        printf("Socket bind failed"); 
        exit(1); 
    }  
}

char* receiveMessage(void)
{  
    int length = sizeof(remoteaddr);
    int recvlen = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&remoteaddr, &length);
    printf("Client: %s\n", buffer);
    return buffer;
}