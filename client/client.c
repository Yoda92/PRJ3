#include "client.h"
// See server.c for description of parameters

void initClient(void)
{
	// Create socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        printf("Creating socket failed"); 
        exit(1);
	}     

	// Filling client information 
    memset((char*)&clientaddr, 0, sizeof(clientaddr)); 
	clientaddr.sin_family = AF_INET; 
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientaddr.sin_port = htons(0); 

    // Filling server information 
    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with client
    if ( bind(sockfd, (const struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0 ) 
    { 
        printf("Socket bind failed"); 
        exit(1); 
    }  
}

void sendMessage(char *message)
{
    if (sendto(sockfd, (const char *) message, strlen(message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("Message failed to send"); 
        exit(1);  
    }
}
