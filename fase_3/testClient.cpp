#include "client.hpp"

int main(void)
{
    client::init();
    uint8_t byte = 0b10000000;
    client::sendMessage(byte);
    return 0;
}