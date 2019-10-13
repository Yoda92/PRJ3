#include "controller.hpp"
#include "inputHandler.hpp"
#include "outputHandler.hpp"

int main(void)
{
    intputHandler::init();
    outputHandler::init();
    // Program loop
    while (true)
    {
        inputHandler::updateInput();
        controller::createCommand(inputHandler::throttleUp, inputHandler::throttleDown, inputHandler::toggleswitch);
        outputHandler::sendOutput(controller::commandArray);
    }
    return 0;
}