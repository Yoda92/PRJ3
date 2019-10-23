#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

class controller {
  public: 
    static void createCommand(int&, int&, int&, int&, int&);
    static uint8_t commandByte;
  private:
    static void setThrottle(void);
    static void setDirection(int&, int&);
    static int currentThrottle;
    static const int maxThrottle = 1000;
    static const int upperThreshold = 3072;
    static const int lowerThreshold = 1024
};

// Protokol:
// 0000 : 0% 
// 0001 : 10% 
// 0010 : 20%
// .. 
// 1010 : 100%

// 
