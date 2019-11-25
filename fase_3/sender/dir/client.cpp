// Inspired by server-client implementation at:
// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
#include "client.hpp"

int client::socketfd;
struct sockaddr_in client::serveraddr;
char client::buffer[100];
struct timeval client::timeout;      

int client::init(void)
{
    // Create socket file descriptor
    // AF_INET : Domain : IPv4 Protokol (32 bit address)
    // SOCK_STREAM : Type : TCP (Connection protocol - More reliable transmission)
    // 0 : Protocol : Zero for IP
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        printf("Cannot create socket.\n"); 
        return -1;
    }
    printf("Socket created.\n");
    // Set timeout for socket
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    if (setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) != 0) {
        printf("Cannot set socket timeout.\n"); 
        return -1;
    }
    printf("Socket timeout set.\n"); 

    // memset(a, b, c) copies char b to the c first charachters in a
    // Ensures server address is empty  
    memset(&serveraddr, 0, sizeof(serveraddr)); 

    // Filling server address
    // AF_INET : Protocol : IPv4
    // inet_addr : Address : Server IP
    // htons(PORT) : Port : htons() ensures that port address is stored correct
    serveraddr.sin_family = AF_INET; 
	serveraddr.sin_addr.s_addr = inet_addr("192.168.0.2"); 
	serveraddr.sin_port = htons(PORT); 

    // Connect client socket to server socket
    if (connect(socketfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0) { 
        printf("Connection failed.\n"); 
        return -1;
    }    
    printf("Connection established.\n"); 
    return 0;
}

int client::sendMessage(uint8_t byte)
{
    int error = 0;
    // memset(a, b, c) copies char b to the c first charachters in a
    // Ensures server address is empty  
    memset(&buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%d", byte);
    // Send message
    if (send(socketfd, buffer, sizeof(buffer), 0) < 0)
    {
        printf("Message error!\n"); 
        memset(&buffer, 0, sizeof(buffer));
        return -1;
    }
    memset(&buffer, 0, sizeof(buffer));
    return 0;
}