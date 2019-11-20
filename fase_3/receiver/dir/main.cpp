#include "server.hpp"
#include "outputHandler.hpp"

int main(void)
{
    server::init();
    outputHandler::init();
    uint8_t receiveByte;
    // Program loop
    while (true)
    {
        server::receiveMessage();
        // printf("Input: %d, %d, %d, %d, %d\n", inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch, inputHandler::adc0, inputHandler::adc1);
        // printf("Output: %d\n", controller::commandByte);
        receiveByte = atoi(server::buffer);
        outputHandler::sendOutput(receiveByte);
    }
    return 0;
}