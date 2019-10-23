#include <stdint.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

class spiHandler {
  public: 
    static void init(int, char*);
    static void transmit(int, uint8_t*, uint8_t*, int cs = 1);
  private:
    static int spi_dev_fd_0, spi_dev_fd_1;
    static char bits;
    static unsigned int speed;
    static unsigned char mode;
};