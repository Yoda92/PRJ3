#include "server.hpp"

int main(void)
{
    server::init();
    while (true)
    {
        server::receiveMessage();
        printf("Message: %s\n",server::buffer);
    }
}