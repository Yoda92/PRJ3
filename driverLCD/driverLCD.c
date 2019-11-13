#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/kdev_t.h>
#include <linux/err.h>
#include <linux/of_gpio.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRJ3 Gruppe 9");
MODULE_DESCRIPTION("GPIO LCD driver");

#define GPIO_MAJOR 22 
#define GPIO_MINOR 0
#define GPIO_NBR_MINORS 255

//LCD 1602a

//gpio pin assignments
static const unsigned int LCD_RS = 13;
static const unsigned int LCD_RW = 16;
static const unsigned int LCD_E = 19;
static const unsigned int LCD_D7 = 27;
static const unsigned int LCD_D6 = 26;
static const unsigned int LCD_D5 = 25;
static const unsigned int LCD_D4 = 24;
static const unsigned int LCD_D3 = 23;
static const unsigned int LCD_D2 = 22;
static const unsigned int LCD_D1 = 21;
static const unsigned int LCD_D0 = 20;
static int devno;
    //static int devno[11] = {0};
static struct cdev GPIO_cdev[2];
struct class *LCD_class;
struct device *LCD_device;

int LCD_devs_cnt = 0;
static int devno; 


static int GPIO_open(struct inode *inode, struct file *filep);
static int GPIO_release(struct inode *inode, struct file *filep);
static int GPIO_probe(struct platform_device *pdev);
static int GPIO_remove(struct platform_device *pdev);
ssize_t GPIO_write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos);
void asciiToLCD(char);
void lcdInnit(void);

struct file_operations GPIO_fops = {
    .owner      = THIS_MODULE,
    .open       = GPIO_open,
    .release    = GPIO_release,
    .write       = GPIO_write,
};

static const struct of_device_id my_gpio_platform_device_match[] =
{
{ .compatible = "PRJ3, driverLCD",}, {},
};

struct gpio_dev {
  int no;   // GPIO number
  int flag; // 0: in, 1: out
};

static struct gpio_dev LCD_devs[255];

static struct platform_driver my_gpio_platform_driver ={
    .probe = GPIO_probe,
    .remove = GPIO_remove,
    .driver = {
    .name = "my_gpio_driverLCD",
    .of_match_table = my_gpio_platform_device_match,
    .owner = THIS_MODULE, }, 
};

static int GPIO_driver_init(void) 
{
    
    int err=0;
    err=alloc_chrdev_region(&devno,0,LCD_devs_cnt,"gpio"); //Register device
    if (err<0)
    {
       goto err_free;
    }
    cdev_init(GPIO_cdev, &GPIO_fops); //Initialize c-dev
    err = cdev_add(GPIO_cdev, devno, LCD_devs_cnt); //Check for failure
    if(err<0)
    {
        goto err_dev_unreg;
    }
    LCD_class=class_create(THIS_MODULE,"LCD_class");
    err=platform_driver_register(&my_gpio_platform_driver);
    if(err<0)
    {
        goto err_plat_unreg;
    }
    return 0;
    
    err_plat_unreg:
        platform_driver_unregister(&my_gpio_platform_driver);
        class_destroy(LCD_class);
    err_dev_unreg:
        cdev_del(GPIO_cdev);
        unregister_chrdev_region(devno,LCD_devs_cnt); //Unregister device
    err_free:

    return err;   
}

int GPIO_open(struct inode *inode, struct file *filep) 
{
    int minor = iminor(filep->f_inode);
    printk("Opening LCD driver [major], [minor]: %i %i\n", MAJOR(devno), minor);
    return 0;
}

int GPIO_release(struct inode *inode, struct file *filep) 
{
    int minor = iminor(filep->f_inode);
    printk("Releasing LCD driver [major], [minor]: %i %i\n", MAJOR(devno), minor);
    return 0;
}

ssize_t GPIO_write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos)
{
    char buffer[12];
    char in_buf;
    int minor = iminor(filep->f_inode);
    
    in_buf=copy_from_user(buffer, buf, count);
    sscanf(buffer, "%d", &in_buf);
    asciiToLCD(in_buf);
    return count;
}

static void GPIO_driver_exit(void) 
{
    platform_driver_unregister(&my_gpio_platform_driver);
    class_destroy(LCD_class);
    cdev_del(GPIO_cdev);
    unregister_chrdev_region(devno, LCD_devs_cnt);
    
    printk(KERN_INFO "LCD driver has been abandoned correctly\n");
 }

static int GPIO_probe(struct platform_device *pdev)
{
    int err = 0;
    struct device *dev = &pdev->dev; // Device ptr derived from current platform_device
    struct device_node *np = dev->of_node; // Device tree node ptr
    enum of_gpio_flags flag;
    int gpios_in_dt = 0;
    printk("Get ready for a probing\n");

    gpios_in_dt=of_gpio_count(np);
    LCD_devs_cnt=0;
    for (int i=0;i<gpios_in_dt ; i++) 
    {

        LCD_devs[i].no=of_get_gpio(np,i);
        /* henter gpio nummer (of_get_gpio) og skriver i struct */ 

        LCD_devs[i].flag=of_get_gpio_flags(np, i, &flag);
        /* Henter gpio flag, dvs retning (of_get_gpio_flags)  og skriver i struct */
        ++LCD_devs_cnt;
    }

    for (int i = 0; i < LCD_devs_cnt; i++)
    {
        char buffer[] ="gpio_00";
        sprintf(buffer,"gpio%d",LCD_devs[i].no);
        err = gpio_request (LCD_devs[i].no, buffer);   //Allokerer gpio
        if (err < 0)
        {   
            printk("Fejl i gpio_request\n");
            goto err_exit;
        }
        if (LCD_devs[i].flag==0)
        {
            err=gpio_direction_input(LCD_devs[i].no);   //Sætter som input.
        }
        else
        {
            err=gpio_direction_output(LCD_devs[i].no, 1);   //Sætter som output.
        }
        
        if (err<0)
        {
            printk("Error in gpio_direction\n");
            goto err_free;
        }
    
        
    }
    LCD_device=device_create(LCD_class, NULL, MKDEV(MAJOR(devno),GPIO_MINOR),NULL, "LCD");
    lcdInnit();

    return 0;

    err_free:
    for (size_t i = 0; i < LCD_devs_cnt; i++)
    {
        device_destroy(LCD_class, MKDEV(MAJOR(devno),i));
        gpio_free(LCD_devs[i].no);
    }    
    err_exit:
    return err;
}

static int GPIO_remove(struct platform_device *pdev)
{
    printk("Your probing session has ended\n");
    for (size_t i = 0; i < LCD_devs_cnt; i++)
    {
        device_destroy(LCD_class, MKDEV(MAJOR(devno),GPIO_MINOR));
        gpio_free(LCD_devs[i].no);
    }
    return 0;
    
}

void asciiToLCD(char input)
{
    switch(input)
    {
        case '0':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,0);
            gpio_set_value(LCD_devs[1].no,0);
            gpio_set_value(LCD_devs[0].no,0);
        }

        case '1':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,0);
            gpio_set_value(LCD_devs[1].no,0);
            gpio_set_value(LCD_devs[0].no,1);
        }
        
        case '2':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,0);
            gpio_set_value(LCD_devs[1].no,1);
            gpio_set_value(LCD_devs[0].no,0);
        }

        case '3':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,0);
            gpio_set_value(LCD_devs[1].no,1);
            gpio_set_value(LCD_devs[0].no,1);
        }

        case '4':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,1);
            gpio_set_value(LCD_devs[1].no,0);
            gpio_set_value(LCD_devs[0].no,0);
        }

        case '5':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,1);
            gpio_set_value(LCD_devs[1].no,0);
            gpio_set_value(LCD_devs[0].no,1);
        }

        case '6':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,1);
            gpio_set_value(LCD_devs[1].no,1);
            gpio_set_value(LCD_devs[0].no,0);
        }

        case '7':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,0);
            gpio_set_value(LCD_devs[2].no,1);
            gpio_set_value(LCD_devs[1].no,1);
            gpio_set_value(LCD_devs[0].no,1);
        }

        case '8':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,1);
            gpio_set_value(LCD_devs[2].no,0);
            gpio_set_value(LCD_devs[1].no,0);
            gpio_set_value(LCD_devs[0].no,0);
        }

        case '9':
        {
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            gpio_set_value(LCD_devs[7].no,0);
            gpio_set_value(LCD_devs[6].no,0);
            gpio_set_value(LCD_devs[5].no,1);
            gpio_set_value(LCD_devs[4].no,1);
            gpio_set_value(LCD_devs[3].no,1);
            gpio_set_value(LCD_devs[2].no,0);
            gpio_set_value(LCD_devs[1].no,0);
            gpio_set_value(LCD_devs[0].no,1);
        }
    }
}


void lcdInnit(void)
{
    for (int i = 0; i < LCD_devs_cnt; i++)
    {
        if (LCD_devs[i].no==22||LCD_devs[i].no==21)
        {
            gpio_set_value(LCD_devs[i].no,1);
        }
        else
        {
            gpio_set_value(LCD_devs[i].no,0);
        }
    }

    for (int i = 0; i < LCD_devs_cnt; i++)
    {
        if (i<4)
        {
            gpio_set_value(LCD_devs[i].no,1);
        }
        else
        {
            gpio_set_value(LCD_devs[i].no,0);
        }
    }

    for (int i = 0; i < LCD_devs_cnt; i++)
    {
        if (i==5||i==4||i==2)
        {
            gpio_set_value(LCD_devs[i].no,1);
        }
        else
        {
            gpio_set_value(LCD_devs[i].no,0);
        }
    } 
}

 module_init(GPIO_driver_init);
 module_exit(GPIO_driver_exit);
