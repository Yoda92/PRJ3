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
    const int PATH_LENGTH = 30;
    const int GPIO_THROTTLE_UP = 17;
    const int GPIO_THROTTLE_DOWN = 23;
    const int GPIO_SWITCH = 18;
    char buffer_throttle_up[PATH_LENGTH], buffer_throttle_down[PATH_LENGTH], buffer_switch[PATH_LENGTH];
    char readdata_throttle_up[3], readdata_throttle_down[3], readdata_switch[3];
    int ret1, ret2, ret3;
};