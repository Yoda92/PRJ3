#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>

#define PATH_LENGTH_GPIO 30
#define PATH_LENGTH_ADC 15
#define PATH_LENGTH_LCD 10
#define GPIO_THROTTLE_UP 22
#define GPIO_THROTTLE_DOWN 27
#define GPIO_SWITCH 17

class inputHandler {       
  public:
    static void init(void);   
    static void updateInput(void);
    static int throttleUp, throttleDown, toggleswitch, adc0, adc1, lcdCurrent;
  private:
    static void readValue(char*, int, int&);
    static void writeValue(char*, const char*);
    static char buffer_throttle_up[PATH_LENGTH_GPIO], buffer_throttle_down[PATH_LENGTH_GPIO], buffer_switch[PATH_LENGTH_GPIO], buffer_adc[PATH_LENGTH_ADC], buffer_lcd[PATH_LENGTH_LCD];
};