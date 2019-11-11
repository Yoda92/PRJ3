#include "server.hpp"

int server::socketfd;
int server::conf;
char server::buffer[100];
struct sockaddr_in server::serveraddr;
struct sockaddr_in server::clientaddr;

void server::init(void)
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
    // INADDR_ANY : Address : Any avaible address. htonl() parses the address information.
    // htons(PORT) : Port : htons() ensures that port address is stored correct
    serveraddr.sin_family = AF_INET; 
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serveraddr.sin_port = htons(PORT);  

	// Bind socket to IP
	if ((bind(socketfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) != 0) { 
		printf("Cannot bind socket.\n"); 
		return;
	} 
	else printf("Socket bound to IP.\n"); 

    // Server listen
    // 5 --> Max number of pending connections
    if ((listen(socketfd, 5)) != 0) {     
		printf("Cannot listen.\n"); 
		return;
	} 
	else printf("Server listening.\n");
    socklen_t length = sizeof(clientaddr); 

	// Accept data packet from client
	conf = accept(socketfd, (struct sockaddr *)&clientaddr, &length); 
	if (conf < 0) { 
		printf("Server did not accept client.\n"); 
		return;
	} 
	else printf("Server accepted client\n"); 
}

void server::receiveMessage(void)
{
    // memset(a, b, c) copies char b to the c first charachters in a
    // Ensures buffer is empty
    // memset(buffer, 0, sizeof(buffer));

    // Read message from client
    read(conf, buffer, sizeof(buffer)); 
}