#include "spiHandler.hpp"
// File descriptor
int spiHandler::spi_dev_fd;
char spiHandler::bits = 8;
unsigned int spiHandler::speed = 1000000;
unsigned char spiHandler::mode = SPI_MODE_0;

void spiHandler::init(void)
{
    // Open SPI device in READ-WRITE mode
    spi_dev_fd = open("/dev/spidev0.0",O_RDWR);
    // Error handling
    if (spi_dev_fd < 0)
    {
        printf("Error opening device.\n");
        return;
    }

    // Configure write to mode 0
    if (ioctl(spi_dev_fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        printf("Error configuring write to mode0.\n");
        return;
    }

    // Configure read to mode 0
    if (ioctl(spi_dev_fd, SPI_IOC_RD_MODE, &mode) < 0)
    {
        printf("Error configuring read to mode0.\n");
        return;
    }

    // Configure write bits to 8
    if (ioctl(spi_dev_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
    {
        printf("Error configuring write bits to 8.\n");
        return;
    }

    // Configure read bits to 8
    if (ioctl(spi_dev_fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0)
    {
        printf("Error configuring read bits to 8.\n");
        return;
    }   

    // Configure write speed to 1 MHz
    if (ioctl(spi_dev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        printf("Error configuring write max speed to 1 MHz.\n");
        return;
    }

    // Configure read speed to 1 MHz
    if (ioctl(spi_dev_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
    {
        printf("Error configuring read max speed to 1 MHz.\n");
        return;
    }

    // Success configuring SPI_device
    printf("Device configuration success.\n");
}

void spiHandler::transmit(uint8_t* tx_buffer, uint8_t* rx_buffer, uint8_t cs)
{
    // local file descriptor
    int local_fd = spi_dev_fd;
    // Set rx and tx buffer
    // Buffer length set to 1 byte
    struct spi_ioc_transfer spi_transfer=
    {
        .tx_buf = (unsigned long) tx_buffer,
        .rx_buf = (unsigned long) rx_buffer,
        .len = 1,				
        .cs_change = cs,		
    };

    // Transmission
    if (ioctl(local_fd, SPI_IOC_MESSAGE(1), &spi_transfer) < 0)
    {
        printf("Transmission was not succesfull.\n");
        return;
    }

    // Transmission succes
    // printf("Transmission complete.\n");
}