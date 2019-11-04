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
static const unsigned int LCD_D7 = 23;
static const unsigned int LCD_D6 = 24;
static const unsigned int LCD_D5 = 27;
static const unsigned int LCD_D4 = 17;
static int devno;
static struct cdev GPIO_cdev;

unsigned int arrayGPIO[6] = {LCD_RS, LCD_RW, LCD_D7, LCD_D6, LCD_D5, LCD_D4}

//interrupt variables
unsigned int gpio2irq;
    //below is to use in our interrupt routine, as well as out read function
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;
static int isr_gpio_value;  //our "interrupt service routine" gpio value
static int proc_gpio_value; //gpio value to proc our read function on

static irqreturn_t mygpio_isr(int irq, void *dev_id);
int GPIO_open(struct inode *inode, struct file *file);
int GPIO_release(struct inode *inode, struct file *file);
int GPIO_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos);

struct file_operations GPIO_fops = {
    .owner      = THIS_MODULE,
    .open       = GPIO_open,
    .release    = GPIO_release,
    .read       = GPIO_read,
};

static irqreturn_t mygpio_isr(int irq, void *dev_id) {
    printk(KERN_INFO "ATTENTION: irq event has occured at irq line %d\n", irq);
    flag = 1;   //sets the flag, when an interrupt is registered
    wake_up_interruptible(&wq);
    isr_gpio_value = gpio_get_value(GPIO_num);
    return IRQ_HANDLED;
}

static int GPIO_driver_init(void) {
    int err = 0;    //variable to use for error handling

    //request gpio
    for (int i = 0; i < 6; ++i) {
        err = gpio_request(arrayGPIO[i], "LCD_GPIO");
        if (err != 0) {
            printk(KERN_ALERT "ERROR: Can't request selected gpio number\n");
            goto err_exit;
        }
    }
    

    //set gpio direction
    for (int i = 0; i < 6; ++i) {
        err = gpio_direction_output(arrayGPIO[i]);
        if (err != 0) {
            printk(KERN_ALERT "ERROR: Can't request selected direction\n");
            goto err_exit;
        }
    }


    //make device number
    devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);

    //register device
    err = register_chrdev_region(devno, GPIO_NBR_MINORS, "GPIO");
    if (err != 0) {
        printk(KERN_ALERT "ERROR: Can't request selected device number\n");
        goto err_free_buf;
    }

    //cdev initiate
    cdev_init(&GPIO_cdev, &GPIO_fops);

    //add device
    err = cdev_add(&GPIO_cdev, devno, GPIO_NBR_MINORS);
    if (err != 0) {
        printk(KERN_ALERT "ERROR: Can't add selected device %d\n", GPIO_num);
        goto err_dev_unregister;
    }

    //request irq
    gpio2irq = gpio_to_irq(GPIO_num);
    err = request_irq(gpio2irq, mygpio_isr, IRQF_TRIGGER_FALLING, "GPIO_irq", NULL);
    if (err != 0) {
        printk(KERN_ALERT "ERROR: Can't request irq");
        cdev_del(&GPIO_cdev);
        unregister_chrdev_region(devno, GPIO_NBR_MINORS);
        gpio_free(GPIO_num);
        return 0;
    }
    else {
        printk(KERN_INFO "Requested irq line is %d", gpio2irq);
    }

    printk(KERN_INFO "GPIO has been initiated correctly\n");

    //error functions
    err_exit:
        return err;

    err_free_buf:
        gpio_free(GPIO_num);

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

ssize_t LCD_write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos) {
    int err = 0;    //error handler
    char temp_buf[7];   //the size is 3 due to gpio pin + array start
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
// Free irq
    free_irq(gpio_to_irq(GPIO_num), NULL);
// Delete Cdev
    cdev_del(&GPIO_cdev);
// Unregister Device
    unregister_chrdev_region(devno, GPIO_NBR_MINORS);
// Free GPIO
    gpio_free(GPIO_num);
    
    printk(KERN_INFO "GPIO has been abandoned correctly\n");
 }

 module_init(GPIO_driver_init);
 module_exit(GPIO_driver_exit);