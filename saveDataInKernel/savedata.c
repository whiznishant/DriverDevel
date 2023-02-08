#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/slab.h>  //For Kmalloc

static dev_t mdev;

static struct cdev myCdev;

static struct class *myClass;
static char *devName = "savedata";
static char *className = "testModules";

static char *dataFromUser;

static int my_open(struct inode *in, struct file *fp)
{
    pr_info("Open called on %s\n",devName);
    return 0;
}

static int my_release(struct inode *in, struct file *fp)
{

    pr_info("Release called on %s\n",devName);
    return 0;
}

static ssize_t my_read(struct file *fp,char *buf, size_t sz,loff_t *offst)
{
    int len = 0;
    pr_info("Read called on %s\n",devName);
    len = strlen(dataFromUser);
    if(sz > len)
        sz = len;

    if( copy_to_user(buf,dataFromUser,len) )
    {
        pr_err("Some Data cannot be read from Kernel\n");
        return -EFAULT;
    }
    pr_info("%ld --- Data size in read", sz);
    //return sz;
    return 0;
}

static ssize_t my_write(struct file *fp,const char *buf, size_t sz,loff_t *offst)
{
    if( copy_from_user(dataFromUser,buf,sz))
    {
        pr_err("Some data cannot be written to kernel");
        return -EFAULT;
    }
    pr_info("Write called on %s -- %s -- sz = %ld\n",devName,dataFromUser,sz);
    return sz;
}



// fs structure to initialize cdev
static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write
};


int __init myModInit(void)
{
    dataFromUser = kmalloc(1024,GFP_ATOMIC);
    if( dataFromUser == NULL )
    {
        pr_err("Kmalloc failed\n");
        return -1;
    }
    
    //Reserve a character device Major number.
    if( alloc_chrdev_region(&mdev,0,1,devName) < 0 )
    {
        pr_err("Cannot assign Major Number \n");
        return -1;
    }
    pr_info("Major number %d assigned to %s \n",MAJOR(mdev),devName);

    //Alloc, Init and Add a chdev.
    //
    //Could have dynamically allocated cdev with cdev_alloc() but trying static.
    // This registers the character device driver with the kernel, the device is live.
    cdev_init(&myCdev,&fops);  
    
    if(cdev_add(&myCdev,mdev,1) < 0)
    {
        pr_err("cdev_add failed \n");
        goto class_err;
    }

    //We need a device node to access this character device from user space.
    //This creates a file /dev/*devName
    //Populate the class structure.
    myClass = class_create(THIS_MODULE,className);
    if(IS_ERR(myClass))
    {
        pr_err("Creating class %s for module %s failed \n",className,devName);
        goto class_err;
    }
    //create device.
    if(IS_ERR(device_create(myClass,NULL, mdev, NULL,devName)))
    {
        pr_err("Creating device for %s module failed\n",devName);
        goto device_err;
    }

    pr_info("Module %s successfully inserted \n" , devName);
    return 0;

device_err:
    cdev_del(&myCdev);
    class_destroy(myClass);
class_err:
    unregister_chrdev_region(mdev,1);
    return -1;
}


void __exit myModCleanup(void)
{
    kfree(dataFromUser);
    device_destroy(myClass,mdev);
    cdev_del(&myCdev);
    class_destroy(myClass);
    unregister_chrdev_region(mdev,1);
}

module_init(myModInit);
module_exit(myModCleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishant Singh");


