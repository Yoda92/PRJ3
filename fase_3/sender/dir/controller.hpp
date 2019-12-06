#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>

#define maxThrottle 250
#define upperThreshold 3072
#define lowerThreshold 1024

class controller {
  public: 
    static void createCommand(int&, int&, int&, int&, int&);
    static uint8_t commandByte;
  private:
    static void setThrottle(void);
    static void setDirection(int&, int&);
    static int currentThrottle;
};
