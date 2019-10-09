#include "controller.h"
#include "inputHandler.h"

#define true 1

int main(void)
{
    inputHandlerInit();
    controllerInit();
    // Program loop
    while (true)
    {
        sendCommand(getInput());
        usleep(100000); // 100 ms delay
    }
    inputHandlerExit();
    return 0;
}