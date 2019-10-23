#include "outputHandler.hpp"

void outputHandler::init(void)
{
    spiHandler::init(spi_dev_fd_1, "/dev/spidev0.1");
}

void outputHandler::sendOutput(uint8_t &byte)
{
    spiHandler::transmit(spi_dev_fd_1, &byte, NULL);
}

