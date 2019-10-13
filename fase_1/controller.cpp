#include "controller.hpp"

void controller::createCommand(int throttleUp, int throttleDown, int toggleswitch)
{
    if (toggleswitch == 1)
    {
        setCommandArray(0, 0, 0);
        exit(1);
    }
    // Update throttle value
    currentThrottle = currentThrottle + throttleUp - throttleDown;
    setThrottle();
}

void controller::setThrottle(void)
{
    // Throttle edgecases
    if (currentThrottle > maxThrottle)
    {
        currentThrottle = maxThrottle;
        setCommandArray(1, 0, 1);
    }
    else if (currentThrottle < minThrottle)
    {
        currentThrottle = minThrottle;
        setCommandArray(0, 0, 0);       
    }
    // Return MSD from currentThrottle and set throttle accordingly
    int currentThrottle_ = (int) ((float) currentThrottle/(maxThrottle/10));
    switch (currentThrottle_)
    {
        case 0 : {
            setCommandArray(0,0,0);
            break;
        }
        case 1 ... 2 : {
            setCommandArray(0,0,1);
            break;
        }
        case 3 ... 4 : {
            setCommandArray(0,1,0);
            break;
        }
        case 5 ... 6 : {
            setCommandArray(0,1,1);
            break;
        }
        case 7 ... 8 : {
            setCommandArray(1,0,0);
            break;
        }
        case 9 ... 10 : {
            setCommandArray(1,0,1);
            break;
        }
    }
}

void controller::setCommandArray(int command_0, int command_1, int command_2)
{
    commandArray[0] = command_0;
    commandArray[1] = command_1;
    commandArray[2] = command_2;
}

