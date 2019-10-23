#include "psocHandler.hpp"

void psocHandler::init(void)
{
    // Setup SPI-communication
    spiHandler::init(spiHandler::spi_dev_fd_1, "/dev/spidev0.1");
}

void psocHandler::sendByte(uint8_t &byte)
{
    spiHandler::transmit(byte, NULL);
}