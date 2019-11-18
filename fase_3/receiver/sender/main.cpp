#include "controller.hpp"
#include "inputHandler.hpp"
#include "client.hpp"

int main(void)
{
    inputHandler::init();
    client::init();
    // Program loop
    while (true)
    {
        inputHandler::updateInput();
        printf("Input: %d, %d, %d, %d, %d\n", inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch, inputHandler::adc0, inputHandler::adc1);
        int x;
        controller::createCommand(
            inputHandler::throttleUp, 
            inputHandler::throttleDown, 
            inputHandler::toggleswitch,
            inputHandler::adc0,
            inputHandler::adc1
            );
        printf("Output: %d\n", controller::commandByte);
        client::sendMessage(controller::commandByte);
    }
    return 0;
}