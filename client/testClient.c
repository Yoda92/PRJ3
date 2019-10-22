#include "client.h"

int main(void)
{
    initClient();
    while(1)
    {
        sendMessage("Hello World!");
        sleep(1);
    }
    return 0;
}