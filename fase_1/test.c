#include <stdio.h>

int main(void)
{
    int currentThrottle = 0;
    int MAX_THROTTLE = 10000000;
    while(1)
    {
     int currentThrottle_ = 0;
     currentThrottle_ = (int) ((float) currentThrottle/(MAX_THROTTLE/10));
     printf("%d", currentThrottle_);
     currentThrottle++;
    }
}