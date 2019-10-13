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
        printf("Input: %d, %d, %d\n", inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch);
        controller::createCommand(inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch);
        printf("Output: %d, %d, %d\n", controller::commandArray[0], controller::commandArray[1], controller::commandArray[2]);
        outputHandler::sendOutput(controller::commandArray);
    }
    return 0;
}