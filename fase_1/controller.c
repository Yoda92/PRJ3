#include "controller.h"

void controllerInit(void)
{
    // Set device address variables
    sprintf(buffer_acceleration_1, "/sys/class/gpio/gpio%d/value\0", OUTPUT_ACCELERATION_1);
    sprintf(buffer_acceleration_2, "/sys/class/gpio/gpio%d/value\0", OUTPUT_ACCELERATION_2);
    sprintf(buffer_switch, "/sys/class/gpio/gpio%d/value\0", OUTPUT_SWITCH);
}

void sendCommand(struct input inputContainer)
{
    inputContainer_ = updateThrottle(inputContainer);
    int fd_acceleration_1, fd_acceleration_2, fd_switch;
    // Set filedescripters and open devices
    fd_acceleration_1 = open(buffer_acceleration_1, O_WRONLY);
    fd_acceleration_2 = open(buffer_acceleration_2, O_WRONLY);
    fd_switch = open(buffer_switch, O_WRONLY);
    // Write values to output
    char temp1[3], temp2[3], temp3[3];
    sprintf(temp1, "%d", inputContainer_.throttleUp);
    sprintf(temp2, "%d", inputContainer_.throttleDown);
    sprintf(temp3, "%d", inputContainer_.toggleswitch);
    write(fd_acceleration_1, temp1, 3);
    write(fd_acceleration_2, temp2, 3);
    write(fd_switch, temp3, 3);
    // Close
    close(fd_acceleration_1);
    close(fd_acceleration_2);
    close(fd_switch);
}

struct input updateThrottle(struct input inputHandler)
{
    struct input inputHandler_ = inputHandler;
    currentThrottle = (unsigned long) inputHandler_.throttleUp - (unsigned long) inputHandler_.throttleDown;
    inputHandler_.throttleUp
    return inputHandler_;
}

