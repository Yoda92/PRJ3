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
    static int commandArray[3];
  private:
    static void setCommandArray(int, int, int);
    static void setThrottle(void);
    static int currentThrottle;
    static const int maxThrottle = 100;
    static const int minThrottle = 0;
};

// Protokol:
// 000 : 0% 
// 001 : 20% 
// 010 : 40%
// 011 : 60%
// 100 : 80%
// 101 : 100%
// 110 : Unused
// 111 : Unused