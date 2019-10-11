#include "inputHandler.h"

void inputHandlerInit(void)
{
    // Set device address variables
    snprintf(buffer_acceleration_1, PATH_LENGTH, "/sys/class/gpio/gpio%d/value", GPIO_ACCELERATION_1);
    snprintf(buffer_acceleration_2, PATH_LENGTH, "/sys/class/gpio/gpio%d/value", GPIO_ACCELERATION_2);
    snprintf(buffer_switch, PATH_LENGTH, "/sys/class/gpio/gpio%d/value", GPIO_SWITCH);
}

void updateInput(void)
{
    int fd_acceleration_1, fd_acceleration_2, fd_switch;
    // Set filedescripters and open devices
    fd_acceleration_1 = open(buffer_acceleration_1, O_RDONLY);
    fd_acceleration_2 = open(buffer_acceleration_2, O_RDONLY);
    fd_switch = open(buffer_switch, O_RDONLY);
    // Update input values
    ret1 = read(fd_acceleration_1, readdata_acceleration_1, 3);
    ret2 = read(fd_acceleration_2, readdata_acceleration_2, 3);
    ret3 = read(fd_switch, readdata_switch, 3);
    // Set input
    input[0] = atoi(readdata_acceleration_1);
    input[1] = atoi(readdata_acceleration_2);
    input[2] = atoi(readdata_switch);
    // Close
    close(fd_acceleration_1);
    close(fd_acceleration_2);
    close(fd_switch);
}

int * getInput(void)
{
    updateInput();
    return input;
}