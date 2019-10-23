#include "mcp3202Handler.hpp"

uint8_t mcp3202Handler::commandByte0 = 0b00011010;
uint8_t mcp3202Handler::commandByte1 = 0b00011110;

void mcp3202Handler::init(void)
{
    // Setup SPI-communication
    spiHandler::init(spiHandler::spi_dev_fd_0, "/dev/spidev0.0");
}

uint16_t mcp3202Handler::getValue(int channel)
{
    // Send start byte and set channel
    // Not interested in rx
    if (channel == 0)
    {
        spiHandler::transmit(&commandByte0, NULL);
    }
    else if (channel == 1)
    {
        spiHandler::transmit(&commandByte1, NULL);
    }
    else
    {
        printf("Only channel 0 and 1 avaible.\n");
        return 0;
    }
    // Receive bytes
    uint8_t tempByte1, tempByte2;
    spiHandler::transmit(NULL, &tempByte1);
    spiHandler::transmit(NULL, &tempByte2);
    // Combine bytes to actual value
    uint16_t value = combineBytes(tempByte1, tempByte2);
    return value;
}

uint16_t mcp3202Handler::combineBytes(uint8_t byte1, uint8_t byte2)
{
    uint16_t tempByte1, tempByte2, tempByte3;
    tempByte1 = byte1;
    tempByte2= byte2;
    tempByte3 = 0;
    tempByte3 += (tempByte1 << 4);
    tempByte3 += (tempByte2 >> 4);
    return tempByte3;
}