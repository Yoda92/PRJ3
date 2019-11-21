#include "server.hpp"
#include "outputHandler.hpp"

int main(void)
{
    server::init();
    outputHandler::init();
    uint8_t receiveByte;
    int error = 0;
    // Program loop
    while (error == 0)
    {
        if(server::receiveMessage() > 0) {
            // printf("Input: %d, %d, %d, %d, %d\n", inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch, inputHandler::adc0, inputHandler::adc1);
            receiveByte = atoi(server::buffer);
            printf("Output: %d\n", receiveByte);
            outputHandler::sendOutput(receiveByte);
        }
        else { 
            printf("Message error. Closing socket.\n");
            close(server::socketfd);
            error = -1;
        }
    }
    return 0;
}