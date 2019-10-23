#include "mcp3202Handler.hpp"

uint8_t mcp3202Handler::commandByte0 = 0b00011010;
uint8_t mcp3202Handler::commandByte1 = 0b00011110;

void mcp3202Handler::init(void)
{
    // Setup SPI-communication
    spiHandler::init(spi_dev_fd_0, "/dev/spidev0.0");
}

uint16_t mcp3202Handler::getValue(int channel)
{
    // Send start byte and set channel
    // Not interested in rx
    uint8_t tempByte0, tempByte1;
    tempByte0 = commandByte0;
    tempByte1 = commandByte1;
    if (channel == 0)
    {
        transmit(spi_dev_fd_0, &tempByte0, NULL);
    }
    else if (channel == 1)
    {
        transmit(spi_dev_fd_0, &tempByte1, NULL);
    }
    else
    {
        printf("Only channel 0 and 1 avaible.\n");
        return 0;
    }
    // Reset and Receive bytes
    tempByte0 = 0;
    tempByte1 = 0;
    transmit(spi_dev_fd_0, NULL, &tempByte0);
    transmit(spi_dev_fd_0, NULL, &tempByte1);
    // Reset CS
    spiHandler::transmit(spi_dev_fd_0, NULL, NULL, 0);
    // Combine bytes to actual value
    uint16_t value = combineBytes(tempByte0, tempByte1);
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