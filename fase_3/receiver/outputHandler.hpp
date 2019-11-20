#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>

class outputHandler {
  public: 
    static void sendOutput(uint8_t);
    static void init(void);
  private:
    static void writeValue(char *, char *);
    static const int PATH_LENGTH_ADC = 15;
    static char buffer_psoc[PATH_LENGTH_ADC];
};