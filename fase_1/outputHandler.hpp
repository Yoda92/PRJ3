#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

class outputHandler {
  public: 
    static void sendOutput(int[]);
    static void init(void);
  private:
    static const int GPIO_OUTPUT_0 = 5;
    static const int GPIO_OUTPUT_1 = 6;
    static const int GPIO_OUTPUT_2 = 7;
    static char buffer_output_0[30], buffer_output_1[30], buffer_output_2[30];
};