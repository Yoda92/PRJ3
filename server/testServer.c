#include "server.h"

int main(void)
{
    initServer();
    while(1)
    {
        receiveMessage();
    }
    return 0;
}