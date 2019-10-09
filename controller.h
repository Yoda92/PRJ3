#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

// GPIO device definitions
#define OUTPUT_ACCELERATION_1 5
#define OUTPUT_ACCELERATION_2 6
#define OUTPUT_SWITCH 7

static int fd_acceleration_1, fd_acceleration_2, fd_switch;

void sendCommand(int[3]);
void controllerInit(void);
void controllerExit(void);