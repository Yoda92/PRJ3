#include "server.hpp"

int main(void)
{
    server::init();
    server::receiveMessage();
    printf("Message: %s\n",server::buffer);
}