/* steps to use this driver
 * 1. insert the driver using insmod
 * 2. device file will be created in /dev. check  it using ls -l /dev/my_device
 * 3. Now, complile and run the userspace code using sudo ./a.out.
 * 4. write/read value using ioctl.
 * 5. check output using dmesg.
 * 6. remove the driver using rmmod.  */


#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

// ioctl commands definition

/* here, 3 arguments represents 
 * _IOX(magic number is differntial charecter,
        command number is number assigned to ioctl command,
   	int32_t is our type of data)   */

#define WR_VALUE _IOW('a','a',int *) // ioctl write command
#define RD_VALUE _IOR('a','b',int *) // ioctl read command




int value[10] ; // value will be responsible for ioctl communication.

dev_t device =0;
static struct class *device_class;
static struct cdev my_cdev;


static int __init insert_module(void);
static void __exit remove_module(void);


// basic file operations prototypes to perform device operations.
// Here, we will need only open and close fops to perform IOCTL

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);

/*------------------- ioctl function prototype--------------------*/

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// file ops structure

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = device_open,

/*unlocked_ioctl is predefined in fops data structure.we are assigning my_ioctl function to this which will be called by ioctl() system call from user space application. */

	.unlocked_ioctl = my_ioctl,  // called by ioctl()
	.release = device_release,
};

static int device_open( struct inode *inode,struct file *file)
{
	printk(KERN_INFO "Device file has been opened\n");
	return 0;
}


static int device_release(struct inode *inode , struct file *file)
{
	printk(KERN_INFO "device file has been closed");
	return 0;
}
	
/* in my_ioctl function we need to implement the command which we have defined
already above. here we have two cases which will be included. i.e, WR_VALUE/RD_VALUE. */

static long my_ioctl(struct file *file , unsigned int cmd, unsigned long arg)
{
	switch( cmd)
	{
		case WR_VALUE: ;
		      
		      int i;
		      copy_from_user( value,(int*) arg, sizeof(value));
		      for(i=0 ;i<10 ; i++){
	printk(KERN_INFO "value recieved from user: value[%d]=%d\n",i+1,value[i]);}
		      break;

		case RD_VALUE:

			copy_to_user((int *) arg , value, sizeof(value));
			break;
	}
	return 0;
}


static int __init insert_module(void)
{
	
//we will create device file dynamically and major number will be allocated to device

	if(( alloc_chrdev_region(&device, 0, 1, "my_device"))< 0)
	{
		printk(KERN_INFO "failed to register device number");
		return -1;
	}

	cdev_init(&my_cdev,&fops);

	// adding device to the system
	
	if((cdev_add(&my_cdev,device,1)) < 0)
	{
            printk(KERN_INFO "Cannot add the device to the system\n");
	    unregister_chrdev_region(device,1);
       	    return -1;
	}


// register the device class

	if( (device_class = class_create(THIS_MODULE,"my_class"))== NULL)
	{
		printk(KERN_ALERT "device class creation failed\n");
		unregister_chrdev_region(device,1);
	        return -1;
	}
// now we will register our device driver

	if((device_create(device_class,NULL,device,NULL,"my_device"))==NULL)
	{
		printk(KERN_ALERT "failed to  create the  device\n");
		class_destroy(device_class);
	}

	printk(KERN_INFO "You have inserted your device driver\n");
	return 0;
}

void __exit remove_module(void)
{
	printk(KERN_INFO "You have removed your driver\n");

	device_destroy(device_class,device);
	class_destroy(device_class);
	unregister_chrdev_region(device,1);
}

module_init(insert_module);
module_exit(remove_module);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IOCTL");















