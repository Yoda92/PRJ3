#include "outputHandler.hpp"

char outputHandler::buffer_psoc[PATH_LENGTH_ADC] = {0};

void outputHandler::init(void)
{
    // Set device address variables
    snprintf(buffer_psoc, PATH_LENGTH_ADC, "/dev/spi_drv1");
}

void outputHandler::sendOutput(uint8_t byte)
{
    char buffer[9];
    sprintf(buffer, "%d", byte);
    writeValue(buffer_psoc, buffer);
}

void outputHandler::writeValue(char *buffer, char *value)
{
    // Set filedescripters and open device
    int fd = open(buffer, O_WRONLY);
    // write value to device
    write(fd, (const void *) value, strlen(value));
    // Close filedescriptor
    close(fd);
}

