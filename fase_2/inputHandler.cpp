#include "inputHandler.hpp"

int inputHandler::throttleUp = 0;
int inputHandler::throttleDown = 0;
int inputHandler::toggleswitch = 0;
int inputHandler::adc0 = 0;
int inputHandler::adc1 = 0;

char inputHandler::buffer_throttle_up[PATH_LENGTH_GPIO] = {0};
char inputHandler::buffer_throttle_down[PATH_LENGTH_GPIO] = {0};
char inputHandler::buffer_switch[PATH_LENGTH_GPIO] = {0};

void inputHandler::init(void)
{
    // Set device address variables
    snprintf(buffer_throttle_up, PATH_LENGTH_GPIO, "/sys/class/gpio/gpio%d/value", GPIO_THROTTLE_UP);
    snprintf(buffer_throttle_down, PATH_LENGTH_GPIO, "/sys/class/gpio/gpio%d/value", GPIO_THROTTLE_DOWN);
    snprintf(buffer_switch, PATH_LENGTH_GPIO, "/sys/class/gpio/gpio%d/value", GPIO_SWITCH);
    mcp3202Handler::init();
}

void inputHandler::updateInput(void)
{
    // Read GPIO values
    readValue(buffer_throttle_up, 2, throttleUp);
    readValue(buffer_throttle_down, 2, throttleDown);
    readValue(buffer_switch, 2, toggleswitch);
    // Read both ADC channels
    adc0 = mcp3202Handler::getValue(0);
    adc1 = mcp3202Handler::getValue(1);
}

void inputHandler::readValue(char *buffer, int size, int &input)
{
    char readdata[size];
    // Set filedescripters and open device
    int fd = open(buffer, O_RDONLY);
    // read value to readdata
    read(fd, readdata, size);
    input = atoi(readdata);
    // Close filedescriptor
    close(fd);
}

void inputHandler::writeValue(char *buffer, const char * value)
{
    // Set filedescripters and open device
    int fd = open(buffer, O_WRONLY);
    // write value to device
    write(fd, (const void *) value, strlen(value));
    // Close filedescriptor
    close(fd);
}