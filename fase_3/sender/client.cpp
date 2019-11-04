// Inspired by server-client implementation at:
// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
#include "client.hpp"

int client::socketfd;
struct sockaddr_in client::serveraddr;
char client::buffer[100];

void client::init(void)
{
    // Create socket file descriptor
    // AF_INET : Domain : IPv4 Protokol (32 bit address)
    // SOCK_STREAM : Type : TCP (Connection protocol - More reliable transmission)
    // 0 : Protocol : Zero for IP
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) { 
		printf("Cannot create socket.\n"); 
		return;
	} 
    else printf("Socket created.\n");

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
		return;
	} 
	else
		printf("Connection established.\n"); 
}

void client::sendMessage(uint8_t byte)
{
    // memset(a, b, c) copies char b to the c first charachters in a
    // Ensures server address is empty  
    memset(&buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%d", byte);
    write(socketfd, buffer, sizeof(buffer)); 
}