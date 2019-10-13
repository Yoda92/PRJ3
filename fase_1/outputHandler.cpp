#include "outputHandler.hpp"

char outputHandler::buffer_output_0[30] = {0};
char outputHandler::buffer_output_1[30] = {0};
char outputHandler::buffer_output_2[30] = {0};

void outputHandler::init(void)
{
    // Set device address variables
    sprintf(buffer_output_0, "/sys/class/gpio/gpio%d/value\0", GPIO_OUTPUT_0);
    sprintf(buffer_output_1, "/sys/class/gpio/gpio%d/value\0", GPIO_OUTPUT_1);
    sprintf(buffer_output_2, "/sys/class/gpio/gpio%d/value\0", GPIO_OUTPUT_2);
}

void outputHandler::sendOutput(int command[3])
{
    // Set filedescripters and open devices
    int fd_output_0, fd_output_1, fd_output_2, fd_output_3;
    fd_output_0 = open(buffer_output_0, O_WRONLY);
    fd_output_1 = open(buffer_output_1, O_WRONLY);
    fd_output_2 = open(buffer_output_2, O_WRONLY);
    // Write values to output
    char temp0[3], temp1[3], temp2[3];
    sprintf(temp0, "%d", command[0]);
    sprintf(temp1, "%d", command[1]);
    sprintf(temp2, "%d", command[2]);
    write(fd_output_0, temp0, 3);
    write(fd_output_1, temp1, 3);
    write(fd_output_2, temp2, 3);
    // Close
    close(fd_output_0);
    close(fd_output_1);
    close(fd_output_2);
}

