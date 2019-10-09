#include "controller.h"

void controllerInit(void)
{
    // Set device address variables
    char buffer_acceleration_1[100], buffer_acceleration_2[100], buffer_switch[100];
    sprintf(buffer_acceleration_1, "/sys/class/gpio/gpio%d/value\0", OUTPUT_ACCELERATION_1);
    sprintf(buffer_acceleration_2, "/sys/class/gpio/gpio%d/value\0", OUTPUT_ACCELERATION_2);
    sprintf(buffer_switch, "/sys/class/gpio/gpio%d/value\0", OUTPUT_SWITCH);
    // Set filedescripters and open devices
    fd_acceleration_1 = open(buffer_acceleration_1, O_WRONLY);
    fd_acceleration_2 = open(buffer_acceleration_2, O_WRONLY);
    fd_switch = open(buffer_switch, O_WRONLY);
}

void controllerExit(void)
{
    close(fd_acceleration_1);
    close(fd_acceleration_2);
    close(fd_switch);
}

void sendCommand(int input[3])
{
    char temp1 = input[0] + '0';
    char temp2 = input[1] + '0';
    char temp3 = input[2] + '0';
    write(fd_acceleration_1, &temp1, 1);
    write(fd_acceleration_2, &temp2, 1);
    write(fd_switch, &temp3, 1);
}