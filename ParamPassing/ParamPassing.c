#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>


int x;
int y[4];
int z; //Will recive callback if this changes ..

module_param(x,int,S_IRUSR|S_IWUSR| S_IXUSR);
module_param_array(y,int,NULL,S_IRUSR|S_IWUSR);


int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back function called...\n");
                printk(KERN_INFO "New value of z = %d\n", z);
                return 0;
        }
        return -1;
}



const struct kernel_param_ops my_param_ops = 
{
        .set = &notify_param, // Use our setter ...
        .get = &param_get_int, // .. and standard getter
};


module_param_cb(z,&my_param_ops,&z,S_IWUSR|S_IRUSR);




int myInit(void)
{
   int i;
   printk(KERN_INFO "testmodule init done \n");
   printk(KERN_INFO "This is the integer --- %d\n",x);
   for(i=0;i<(sizeof(y)/sizeof(int));i++)
   {
	printk(KERN_INFO "Array--%d--- = %d\n",i,y[i]);
   }
   return 0;
}

void myCleanup(void)
{
   printk("testmodule cleanup done \n");
}



module_init(myInit);
module_exit(myCleanup);
MODULE_INFO(intree,"Y");
MODULE_LICENSE("GPL");
