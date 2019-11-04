#include "client.hpp"

int main(void)
{
    client::init();
    uint8_t byte = 0;
    while(true)
    {
        client::sendMessage(byte);
        byte++;
        sleep(1);
    }
    return 0;
}