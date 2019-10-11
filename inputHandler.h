#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define PATH_LENGTH 30

// GPIO device definitions
#define GPIO_ACCELERATION_1 17
#define GPIO_ACCELERATION_2 23
#define GPIO_SWITCH 18

// Declarations
void inputHandlerInit(void);   
void updateInput(void);
int * getInput(void);

int input[3];
char buffer_acceleration_1[PATH_LENGTH], buffer_acceleration_2[PATH_LENGTH], buffer_switch[PATH_LENGTH];
char readdata_acceleration_1[3], readdata_acceleration_2[3], readdata_switch[3];
int ret1, ret2, ret3;