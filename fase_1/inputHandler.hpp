#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

class inputHandler {       
  public:
    static void init(void);   
    static void updateInput(void);
    static int throttleUp, throttleDown, toggleswitch;
  private:
    static const int PATH_LENGTH = 30;
    static const int GPIO_THROTTLE_UP = 22;
    static const int GPIO_THROTTLE_DOWN = 27;
    static const int GPIO_SWITCH = 17;
    static char buffer_throttle_up[PATH_LENGTH], buffer_throttle_down[PATH_LENGTH], buffer_switch[PATH_LENGTH];
};