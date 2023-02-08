#include <linux/module.h>   // Mandatory include for modules.
#include <linux/init.h> 
#include <linux/kdev_t.h>   // req for dev_t structure Major 
#include <linux/fs.h>       // req for alloc_chrdev_region and unregister_chrdev_region 
#include <linux/device.h>   // req for class_create and device_create
#include <linux/err.h>
#include <linux/cdev.h>

static dev_t dev = 0;
static char *devName = "bx2Dev";
static struct class *myClass;
static struct device *myDevice;
static struct cdev *myCdev;




/* file_operations elements 
ssize_t (*read) (struct file *,char __user *,size_t,loff_t *);
ssize_t (*write)(struct file *,const char __user *,size_t , loff_t *);
int (*open) ( struct inode *,struct file *);
int (*release) ( struct inode *,struct file *);
*/

static int open(struct inode *in , struct file *fp )
{
    pr_info("Open called for %s device\n",devName);
    return 0;
}

static int release( struct inode *in, struct file *fp )
{
    pr_info("Release called for %s device\n",devName);
    return 0;
}

static ssize_t read( struct file *fp, char __user *user,size_t sz, loff_t *off)
{
    pr_info("Read called for %s device\n",devName);
    return 0;
}

static ssize_t write( struct file *fp,const char __user *user, size_t sz, loff_t *off)
{
    pr_info("Write called for %s device\n",devName);
    return sz;
}

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .open = open,
    .release = release,
    .read = read,
    .write = write
};

int __init myModInit(void)
{
    //Register as character device
    if(alloc_chrdev_region(&dev,0,1,devName) < 0)
    {
        pr_err("Module could not be assigned a major number\n");
        return -1;
    }
    pr_info("Assigned Major Number to %s as %d\n",devName,MAJOR(dev));


    //Create the cdev structure.
    myCdev = cdev_alloc();
    if(IS_ERR(myCdev))
    {
        pr_err("Unable to allocate cdev struct \n");
        goto class_err;
    }
    
    cdev_init(myCdev,&fops);
    cdev_add(myCdev,dev,1);

    //Create a corresponding device file 
    // 1. Create a structure called class . This will be reflected in /sys/class
    // 2. Create device file using this structure.
    myClass = class_create(THIS_MODULE,"learnModules");  // will create /sys/class/learnModules    
    if(IS_ERR(myClass))
    {
        pr_err("Class cannot be created\n");
        goto class_err;
    }
    myDevice = device_create(myClass,NULL,dev,NULL,devName);
    if(IS_ERR(myClass))
    {
        pr_err("Device Creation failed\n");
        goto device_err;
    }

    pr_info("Module inserted succesfully\n");
    return 0;

device_err:
    class_destroy(myClass);
class_err:
    unregister_chrdev_region(dev,1);

return -1;
}

void __exit myModCleanUp(void)
{
    device_destroy(myClass,dev);  // This takes class and dev as argument as opposed to struct device pointer
    class_destroy(myClass);
    cdev_del(myCdev);
    unregister_chrdev_region(dev,1);
    pr_info("Module Unloaded Succesfully\n"); 
}


module_init(myModInit);
module_exit(myModCleanUp);

MODULE_INFO(intree,"yes");
MODULE_LICENSE("GPL");
