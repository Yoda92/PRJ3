#include "inputHandler.hpp"

int inputHandler::throttleUp = 0;
int inputHandler::throttleDown = 0;
int inputHandler::toggleswitch = 0;

char inputHandler::buffer_throttle_up[PATH_LENGTH] = {0};
char inputHandler::buffer_throttle_down[PATH_LENGTH] = {0};
char inputHandler::buffer_switch[PATH_LENGTH] = {0};

void inputHandler::init(void)
{
    // Set device address variables
    snprintf(buffer_throttle_up, PATH_LENGTH, "/sys/class/gpio/gpio%d/value", GPIO_THROTTLE_UP);
    snprintf(buffer_throttle_down, PATH_LENGTH, "/sys/class/gpio/gpio%d/value", GPIO_THROTTLE_DOWN);
    snprintf(buffer_switch, PATH_LENGTH, "/sys/class/gpio/gpio%d/value", GPIO_SWITCH);
}

void inputHandler::updateInput(void)
{
    char readdata_throttle_up[3], readdata_throttle_down[3], readdata_switch[3];
    int ret1, ret2, ret3;
    // Set filedescripters and open devices
    int fd_throttle_up, fd_throttle_down, fd_switch;
    fd_throttle_up = open(buffer_throttle_up, O_RDONLY);
    fd_throttle_down = open(buffer_throttle_down, O_RDONLY);
    fd_switch = open(buffer_switch, O_RDONLY);
    // Update input values
    ret1 = read(fd_throttle_up, readdata_throttle_up, 3);
    ret2 = read(fd_throttle_down, readdata_throttle_down, 3);
    ret3 = read(fd_switch, readdata_switch, 3);
    // Set input
    throttleUp = atoi(readdata_throttle_up);
    throttleDown = atoi(readdata_throttle_down);
    toggleswitch = atoi(readdata_switch);
    // Close
    close(fd_throttle_up);
    close(fd_throttle_down);
    close(fd_switch);
}