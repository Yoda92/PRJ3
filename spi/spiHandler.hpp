#include <stdint.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

class spiHandler {
  public: 
    static void init(void);
    static void transmit(&uint8_t, &uint8_t);
  private:
    static int spi_dev_fd;
    static char bits;
    static unsigned int speed;
    static unsigned char mode;
};