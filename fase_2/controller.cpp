#include "controller.hpp"

int controller::currentThrottle = 0;
uint8_t controller::commandByte;

void controller::createCommand(int &throttleUp, int &throttleDown, int &toggleswitch, int &adc0, int &adc1)
{
    commandByte = 0b00000000;
    if (toggleswitch == 0)
    {
        currentThrottle = 0;
        return;
    }
    // Update throttle value and set acutal throttle
    currentThrottle = currentThrottle + throttleUp - throttleDown;
    setThrottle();
    // Update direction given adc state
    setDirection(adc0, adc1);
}

void controller::setThrottle(void)
{
    // Throttle edgecases
    if (currentThrottle >= maxThrottle)
    {
        currentThrottle = maxThrottle;
        commandByte &= (0b11111010);  
        return;
    }
    if (currentThrottle <= minThrottle)
    {
        currentThrottle = 0;
        return;
    }
    // Return MSD from currentThrottle and set throttle accordingly
    uint8_t currentThrottle_ = (int) ((float) currentThrottle/(maxThrottle/10));
    // currentThrottle_ is whole number between 0 and 10
    commandByte &= currentThrottle_;
}

void controller::setDirection(int &adc0, int &adc1)
{
    // adc0 --> left/right
    // adc1 --> up/down

    // Reset directions
    commandByte &= (0b00001111);  

    // Check for left/right position
    if (adc0 > upperThreshold)
    {
        commandByte &= (0b10001111);  
    }
    else if(adc0 < lowerThreshold)
    {
        commandByte &= (0b01001111); 
    }

    // Check for up/down position
    if (adc1 > upperThreshold)
    {
        commandByte &= (0b00101111);  
    }
    else if(adc1 < lowerThreshold)
    {
        commandByte &= (0b00011111); 
    }
}

