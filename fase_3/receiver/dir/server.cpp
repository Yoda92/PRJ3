#include "server.hpp"

int server::socketfd;
int server::conf;
char server::buffer[100];
struct sockaddr_in server::serveraddr;
struct sockaddr_in server::clientaddr;
struct timeval server::timeout;

int server::init(void)
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
    if(setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) != 0) {
        printf("Cannot set socket timout.\n");
        return -1;
    }
  
    printf("Socket timeout set.\n");
    
    // memset(a, b, c) copies char b to the c first characters in a
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
		return -1;
	} 
	printf("Socket bound to IP.\n"); 

    // Server listen
    // 5 --> Max number of pending connections
    if ((listen(socketfd, 5)) != 0) {     
		printf("Cannot listen.\n"); 
		return -1;
	} 
	printf("Server listening.\n");
    socklen_t length = sizeof(clientaddr); 

	// Accept data packet from client 
    conf = accept(socketfd, (struct sockaddr *)&clientaddr, &length);
	if (conf < 0) { 
		printf("Server did not accept client.\n"); 
		return -1;
	} 
	printf("Server accepted client\n"); 

    return 0;
}

int server::receiveMessage(void)
{
    // memset(a, b, c) copies char b to the c first charachters in a
    // Ensures buffer is empty
    memset(buffer, 0, sizeof(buffer));

    // Read message from client
    return read(conf, buffer, sizeof(buffer));
}