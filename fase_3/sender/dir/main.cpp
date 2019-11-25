#include "controller.hpp"
#include "inputHandler.hpp"
#include "client.hpp"

int main(void)
{
    if(inputHandler::init() < 0) return 0;
    client::init();
    int error = 0;
    // Program loop
    while (error == 0)
    {
        inputHandler::updateInput();
        // printf("Input: %d, %d, %d, %d, %d\n", inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch, inputHandler::adc0, inputHandler::adc1);
        controller::createCommand(
            inputHandler::throttleUp, 
            inputHandler::throttleDown, 
            inputHandler::toggleswitch,
            inputHandler::adc0,
            inputHandler::adc1
            );
        // printf("Output: %d\n", controller::commandByte);
        if (client::sendMessage(controller::commandByte) < 0)
        {
            printf("Closing socket.\n"); 
            close(client::socketfd);
            error = -1;
        }
    }
    return 0;
}