#include "inputHandler.h"

void inputHandlerInit(void)
{
    // Set device address variables
    char buffer_acceleration_1[100], buffer_acceleration_2[100], buffer_switch[100];
    sprintf(buffer_acceleration_1, "/sys/class/gpio/gpio%d/value\0", GPIO_ACCELERATION_1);
    sprintf(buffer_acceleration_2, "/sys/class/gpio/gpio%d/value\0", GPIO_ACCELERATION_2);
    sprintf(buffer_switch, "/sys/class/gpio/gpio%d/value\0", GPIO_SWITCH);
    // Set filedescripters and open devices
    fd_acceleration_1 = open(buffer_acceleration_1, O_RDONLY);
    fd_acceleration_2 = open(buffer_acceleration_2, O_RDONLY);
    fd_switch = open(buffer_switch, O_RDONLY);
}

void inputHandlerExit(void)
{
    close(fd_acceleration_1);
    close(fd_acceleration_2);
    close(fd_switch);
}

void updateInput(void)
{
    // Update input values
    char readdata_acceleration_1, readdata_acceleration_2, readdata_switch;
    read(fd_acceleration_1, &readdata_acceleration_1, 1);
    read(fd_acceleration_2, &readdata_acceleration_2, 1);
    read(fd_switch, &readdata_switch, 1);
    input[0] = (int) readdata_acceleration_1;
    input[1] = (int) readdata_acceleration_2;
    input[2] = (int) readdata_switch;
}

int* getInput(void)
{
    updateInput();
    return input;
}