#include "mcp3202Handler.hpp"

void mcp3202Handler::init(void)
{
    // Setup SPI-communication
    spiHandler::init();
}

int mcp3202Handler::getValue(int channel)
{
    // Send start byte and set channel
    // Not interested in rx
    if (channel == 0)
    {
        spiHandler::transmit(&commandByte0, NULL);
    }
    else if (channel == 1)
    {
        spiHandler::transmit(commandByte1, NULL);
    }
    else
    {
        printf("Only channel 0 and 1 avaible.\n");
        return 0;
    }
    // Receive bytes
    uint8_t tempByte1, tempByte2;
    transmit(NULL, &tempByte1);
    transmit(NULL, &tempByte2);
    // Combine bytes to actual value
    uint16_t value;
    combineBytes(&tempByte1, &tempByte2, &value);
    return value;
}

int mcp3202Handler::combineBytes(uint8_t &byte1, uint8_t &byte2, uint16_t &value)
{
    value = 0;
    value += ((uint16_t) byte1 << 4);
    value += ((uint16_t) byte2 >> 4);
}