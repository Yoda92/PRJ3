#include "controller.h"
#include "inputHandler.h"

#define true 1

int main(void)
{
    controllerInit();
    inputHandlerInit();
    // Program loop
    while (true)
    {
        sendCommand(getInput());
    }
    return 0;
}