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
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRJ3 Gruppe 9");
MODULE_DESCRIPTION("GPIO LCD driver");

#define GPIO_MAJOR 22 
#define GPIO_MINOR 0
#define GPIO_NBR_MINORS 255

//LCD 1602a

/*gpio pin assignments
LCD_RS = 13;
LCD_RW = 16;
LCD_E = 19;
LCD_D7 = 27;
LCD_D6 = 26;
LCD_D5 = 25;
LCD_D4 = 24;
LCD_D3 = 23;
LCD_D2 = 22;
LCD_D1 = 21;
LCD_D0 = 20;
*/
static int devno;
static int connection=0;
static struct cdev GPIO_cdev;
struct class *LCD_class;
struct device *LCD_device;

int LCD_devs_cnt = 1;
static int devno; 


static int GPIO_open(struct inode *inode, struct file *filep);
static int GPIO_release(struct inode *inode, struct file *filep);
static int GPIO_probe(struct platform_device *pdev);
static int GPIO_remove(struct platform_device *pdev);
ssize_t GPIO_write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos);
void asciiToLCD(char);
void lcdInnit(void);
void writeLCD(char *input);
void shiftDisplayLeft(void);
void setDDRAM_Adress(char *binary);

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
    cdev_init(&GPIO_cdev, &GPIO_fops); //Initialize c-dev
    err = cdev_add(&GPIO_cdev, devno, LCD_devs_cnt); //Check for failure
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
    printk("Innit\n");
    return 0;
    
    err_plat_unreg:
        platform_driver_unregister(&my_gpio_platform_driver);
        class_destroy(LCD_class);
    err_dev_unreg:
        cdev_del(&GPIO_cdev);
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
    char buffer[256]="\0";
    char in_buf;
    char clearbuf[2];
    clearbuf[0]='q';
    
    in_buf=copy_from_user(buffer, buf, count);
    sscanf(buffer, "%c", &in_buf);
    for (size_t i = 0; i < strlen(buffer)-1; i++)
    {
        printk("%c\n",buffer[i]);
    }

    writeLCD(buffer);
    writeLCD("  ");
    setDDRAM_Adress("1001000");

    return count;
}

static void GPIO_driver_exit(void) 
{
    platform_driver_unregister(&my_gpio_platform_driver);
    class_destroy(LCD_class);
    cdev_del(&GPIO_cdev);
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
            err=gpio_direction_output(LCD_devs[i].no, 0);   //Sætter som output.
        }
        
        if (err<0)
        {
            printk("Error in gpio_direction\n");
            goto err_free;
        }
    
        
    }
    LCD_device=device_create(LCD_class, NULL, MKDEV(MAJOR(devno),0),NULL, "LCD");
    lcdInnit();
    mdelay(1);
    printk("You've been probed%d%d\n",LCD_devs[0].no,LCD_devs[10].no);
    writeLCD("Throttle: ");
    setDDRAM_Adress("0000001");
    writeLCD("Connection:off ");
    setDDRAM_Adress("1001000");

    return 0;

    err_free:
    for (size_t i = 0; i < LCD_devs_cnt; i++)
    {
        device_destroy(LCD_class, MKDEV(MAJOR(devno),MINOR(devno)));
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



void lcdInnit(void)
{
    for (int i = 0; i < LCD_devs_cnt; i++) //For loop for Entry Mode Set
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
    gpio_set_value(LCD_devs[10].no,0); // Enable active low
    mdelay(1);
    gpio_set_value(LCD_devs[10].no,1); 

    for (int i = 0; i < LCD_devs_cnt; i++) // For loop for Display ON/OFF
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
    gpio_set_value(LCD_devs[10].no,0);
    mdelay(1);
    gpio_set_value(LCD_devs[10].no,1); 
    for (int i = 0; i < LCD_devs_cnt; i++) // For loop for Funktion Set
    {
        if (i==5||i==4||i==3)
        {
            gpio_set_value(LCD_devs[i].no,1);
        }
        else
        {
            gpio_set_value(LCD_devs[i].no,0);
        }
    }
    gpio_set_value(LCD_devs[10].no,0);
    mdelay(1);
    gpio_set_value(LCD_devs[10].no,1); 

    
}

void writeLCD(char *input)
{
	int bitArray[8];
    int LCDcommand=0;
    

    switch (input[0])
    {
    case 'q':
        for (int i = 9; i >= 0; i--)
        {
            if (i==0)
            {
                gpio_set_value(LCD_devs[i].no,1);
                printk("1\n");
            }
            else
            {
                gpio_set_value(LCD_devs[i].no,0);
                printk("0\n");
            }
            
        }
        gpio_set_value(LCD_devs[10].no,0);
        mdelay(10);
        gpio_set_value(LCD_devs[10].no,1);
        writeLCD("Throttle: ");
        setDDRAM_Adress("0000001");
        if (connection==0)
        {
            writeLCD("Connection:off ");
            setDDRAM_Adress("1001000");
        }
        else
        {
            writeLCD("Connection:on ");
            setDDRAM_Adress("1001000");
        } 
        LCDcommand=1;
        break;
    case 'p':
        setDDRAM_Adress("1101001");
        writeLCD("on  ");
        setDDRAM_Adress("1001000");
        connection=1;
        LCDcommand=1;
        break;
    case 'i':
        lcdInnit();
        mdelay(1);
        printk("You've been probed%d%d\n",LCD_devs[0].no,LCD_devs[10].no);
        writeLCD("Throttle: ");
        setDDRAM_Adress("0000001");
        writeLCD("Connection:off ");
        setDDRAM_Adress("1001000");
        LCDcommand=1;
        break;
    case 'u':
        setDDRAM_Adress("1101001");
        writeLCD("off");
        setDDRAM_Adress("1001000");
        connection=0;
        LCDcommand=1;
        break;
        
    default:
        break;
    }

    if (LCDcommand==0)
    {
        for (int i = 0; i < strlen(input)-1; i++) {

            for (int j = 7; j >= 0; --j) {
                bitArray[j] = (input[i] & (1 << j)) ? 1 : 0;
            }
            gpio_set_value(LCD_devs[9].no,1);
            gpio_set_value(LCD_devs[8].no,0);
            for (int n = 7; n >= 0; --n) 
            {
                gpio_set_value(LCD_devs[n].no, bitArray[n]);
            }
            gpio_set_value(LCD_devs[10].no,0);
            mdelay(10);
            gpio_set_value(LCD_devs[10].no,1);
        }
	}   	
}

void setDDRAM_Adress(char *binary)
{
    for (size_t i = 0; i < strlen(binary); i++)
    {
        gpio_set_value(LCD_devs[i].no,(int)binary[i]-48); //ascii 1 or 0 type conversion to int 1 or 0
    }
    gpio_set_value(LCD_devs[7].no,1);
    gpio_set_value(LCD_devs[8].no,0);
    gpio_set_value(LCD_devs[9].no,0);
    gpio_set_value(LCD_devs[10].no,0);
    mdelay(10);
    gpio_set_value(LCD_devs[10].no,1); //sending set adress command 
}


 module_init(GPIO_driver_init);
 module_exit(GPIO_driver_exit);
