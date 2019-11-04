#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRJ3 Gruppe 9");
MODULE_DESCRIPTION("GPIO LCD driver");

#define GPIO_MAJOR 22 
#define GPIO_MINOR 0
#define GPIO_NBR_MINORS 255

//LCD 1602a

//original variables
static const unsigned int LCD_RS = 25;
static const unsigned int LCD_RW = 22;
static const unsigned int LCD_E = 22;
static const unsigned int LCD_D7 = 23;
static const unsigned int LCD_D6 = 24;
static const unsigned int LCD_D5 = 27;
static const unsigned int LCD_D4 = 17;
static const unsigned int LCD_D3 = 17;
static const unsigned int LCD_D2 = 17;
static const unsigned int LCD_D1 = 17;
static const unsigned int LCD_D0 = 17;
static int devno;
    //static int devno[11] = {0};
static struct cdev GPIO_cdev;

//array to store all GPIO numbers in
unsigned int gpioArray[11] = {LCD_RS, LCD_RW, LCD_E, LCD_D7, LCD_D6, LCD_D5, LCD_D4, LCD_D3, LCD_D2, LCD_D1, LCD_D0}

int GPIO_open(struct inode *inode, struct file *file);
int GPIO_release(struct inode *inode, struct file *file);
int GPIO_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos);

struct file_operations GPIO_fops = {
    .owner      = THIS_MODULE,
    .open       = GPIO_open,
    .release    = GPIO_release,
    .read       = GPIO_write,
};

static int GPIO_driver_init(void) {
    int err = 0;    //variable to use for error handling

    //request gpio
    for (int i = 0; i < 11; ++i) {
        err = gpio_request(gpioArray[i], "LCD_GPIO");
        if (err != 0) {
            printk(KERN_ALERT "ERROR: Can't request selected gpio number\n");
            goto err_exit;
        }
    }

    //set gpio direction
    for (int i = 0; i < 11; ++i) {
        err = gpio_direction_output(gpioArray[i]);
        if (err != 0) {
            printk(KERN_ALERT "ERROR: Can't request selected direction\n");
            goto err_exit;
        }
    }

    //make device number
    devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
    // for (int i = 0; i > 11; ++i) {
    //     devno[i] = MKDEV(GPIO_MAJOR, ++GPIO_MINOR);
    // }

    //register device
    err = register_chrdev_region(devno, GPIO_NBR_MINORS, "LCD_GPIO");
    // for (int i = 0; i < 11; ++i) {
    //     err = register_chrdev_region(devno[i], GPIO_NBR_MINORS, "LCD_GPIO");
    //     if (err != 0) {
    //         printk(KERN_ALERT "ERROR: Can't request selected device number\n");
    //         goto err_free_buf;
    //     }
    // }
    if (err != 0) {
        printk(KERN_ALERT "ERROR: Can't request selected device number\n");
        goto err_free_buf;
    }

    //cdev initiate
    cdev_init(&GPIO_cdev, &GPIO_fops);

    //add device
    err = cdev_add(&GPIO_cdev, devno, GPIO_NBR_MINORS);
    // for (int i = 0; i < 11; ++i) {
    //     err = cdev_add(&GPIO_cdev, devno[i], GPIO_NBR_MINORS);
    //     if (err != 0) {
    //         printk(KERN_ALERT "ERROR: Can't add selected device %d\n", GPIO_num);
    //         goto err_dev_unregister;
    //     }
    // }

    if (err != 0) {
        printk(KERN_ALERT "ERROR: Can't add selected device %d\n", GPIO_num);
        goto err_dev_unregister;
    }

    printk(KERN_INFO "LCD driver has been initiated correctly\n");

    //error functions
    err_exit:
        return err;

    err_free_buf:
        for (int i = 0; i < 11; ++1) {
            gpio_free(gpioArray[i]);
        };

    err_dev_unregister:
        unregister_chrdev_region(devno, GPIO_NBR_MINORS);

    return 0;
}

int GPIO_open(struct inode *inode, struct file *filep) {
    printk("Opening GPIO [major], [minor]: %i %i\n", GPIO_MAJOR, GPIO_MINOR);
    return 0;
}

int GPIO_release(struct inode *inode, struct file *filep) {
    printk("Releasing GPIO [major], [minor]: %i %i\n", GPIO_MAJOR, GPIO_MINOR);
    return 0;
}

ssize_t GPIO_write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos) {
    int err = 0;        //error handler
    char temp_buf[12];  //the size is 3 due to gpio pin + array start
    int temp_buf_int;
    err = copy_from_user(temp_buf, buf, count);
    if (err != 0) {
        printk(KERN_ALERT "Not all bytes were copied for device %d\n", LED3_num);
        return 0;
    }
    sscanf(temp_buf, "%d", &temp_buf_int);
    gpio_set_value(LED3_num, temp_buf_int);
    *f_pos += count;
    return count;    
}

static void GPIO_driver_exit(void) {
// Delete Cdev
    for (int i = 0; i < 11; ++1) {
        cdev_del(&GPIO_cdev);
    }
// Unregister Device
    for (int i = 0; i < 11; ++1) {
        unregister_chrdev_region(devno, GPIO_NBR_MINORS);
    }
// Free GPIO
    for (int i = 0; i < 11; ++1) {
        gpio_free(gpioArray[i]);
    }
    
    printk(KERN_INFO "LCD driver has been abandoned correctly\n");
 }

 module_init(GPIO_driver_init);
 module_exit(GPIO_driver_exit);