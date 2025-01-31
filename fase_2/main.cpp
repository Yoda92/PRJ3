#include "controller.hpp"
#include "inputHandler.hpp"
#include "outputHandler.hpp"

int main(void)
{
    inputHandler::init();
    outputHandler::init();
    // Program loop
    while (true)
    {
        inputHandler::updateInput();
        printf("Input: %d, %d, %d, %d, %d\n", inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch, inputHandler::adc0, inputHandler::adc1);
        controller::createCommand(
            inputHandler::throttleUp, 
            inputHandler::throttleDown, 
            inputHandler::toggleswitch,
            inputHandler::adc0,
            inputHandler::adc1
            );
        printf("Output: %d\n", controller::commandByte);
        outputHandler::sendOutput(controller::commandByte);
    }
    return 0;
}