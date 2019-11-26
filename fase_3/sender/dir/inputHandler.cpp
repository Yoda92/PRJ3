#include "inputHandler.hpp"

// Global values
int inputHandler::throttleUp = 0;
int inputHandler::throttleDown = 0;
int inputHandler::toggleswitch = 0;
int inputHandler::adc0 = 0;
int inputHandler::adc1 = 0;

// Device address buffers
char inputHandler::buffer_throttle_up[PATH_LENGTH_GPIO] = {0};
char inputHandler::buffer_throttle_down[PATH_LENGTH_GPIO] = {0};
char inputHandler::buffer_switch[PATH_LENGTH_GPIO] = {0};
char inputHandler::buffer_adc[PATH_LENGTH_ADC] = {0};
char inputHandler::buffer_lcd[PATH_LENGTH_LCD] = {0};

void inputHandler::init(void)
{
    // Set device address variables
    snprintf(buffer_throttle_up, PATH_LENGTH_GPIO, "/sys/class/gpio/gpio%d/value", GPIO_THROTTLE_UP);
    snprintf(buffer_throttle_down, PATH_LENGTH_GPIO, "/sys/class/gpio/gpio%d/value", GPIO_THROTTLE_DOWN);
    snprintf(buffer_switch, PATH_LENGTH_GPIO, "/sys/class/gpio/gpio%d/value", GPIO_SWITCH);
    snprintf(buffer_adc, PATH_LENGTH_ADC, "/dev/spi_drv0");
    snprintf(buffer_lcd, PATH_LENGTH_LCD, "/dev/LCD");
}

void inputHandler::updateInput(void)
{
    // Read GPIO values
    readValue(buffer_throttle_up, 2, throttleUp);
    readValue(buffer_throttle_down, 2, throttleDown);
    readValue(buffer_switch, 2, toggleswitch);
    // Read both ADC channels
    writeValue(buffer_adc, "0");
    readValue(buffer_adc, 5, adc0); 
    writeValue(buffer_adc, "1");
    readValue(buffer_adc, 5, adc1); 
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