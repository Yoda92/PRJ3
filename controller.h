#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

// GPIO device definitions
#define OUTPUT_ACCELERATION_1 5
#define OUTPUT_ACCELERATION_2 6
#define OUTPUT_SWITCH 7

void sendCommand(int[3]);
void controllerInit(void);

char buffer_acceleration_1[30], buffer_acceleration_2[30], buffer_switch[30];