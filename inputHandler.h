#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

// GPIO device definitions
#define GPIO_ACCELERATION_1 2
#define GPIO_ACCELERATION_2 3
#define GPIO_SWITCH 4

void inputHandlerInit(void);   
void inputHandlerExit(void);
int* getInput(void);
static void updateInput(void);
static int fd_acceleration_1, fd_acceleration_2, fd_switch, input[3];