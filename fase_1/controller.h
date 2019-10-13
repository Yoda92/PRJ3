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

struct input 
{ 
   int throttleUp, throttleDown, toggleswitch;
};

void sendCommand(struct input);
void controllerInit(void);
struct input updateThrottle(struct input);

char buffer_acceleration_1[30], buffer_acceleration_2[30], buffer_switch[30];

unsigned long currentThrottle;