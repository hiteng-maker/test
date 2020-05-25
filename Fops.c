#include<linux/init.h>     // responsible for module_init() and module_exit()
#include<linux/module.h>   // must included when you play with kernel modules.
#include<linux/kernel.h>   // responsible for every kernel MACROs like KERN_ALERT & other functions which resides in kernel like printk().
#include<linux/device.h>  // responsible for craeting device file in kernel dynamically
#include<linux/fs.h>  // its a collection of function pointers where file_operation structure is defined.
#include<linux/uaccess.h> // for copy_to/from_user().

#define DEVICE_NAME "hitenchar"
#define CLASS_NAME "hiten"


// here all the variable must be declared as static so our data remained unchanged till end of the program

static int  majornumber;   // device number which will be created during runtime.
static char message[256]={0};  //string from user_space will be stored here.
static int size_of_message= 0;  // number of data bytes which will be used in copy_to_user().

//the device-driver class and structure pointer. we initilize them as NULL to avoid any adress related issues in memory.

static struct class* hitencharclass = NULL;  
static struct device* hitenchardevice = NULL;

// Here is prototype of the functions which we want to use in our device file operation. this must be declared before file ops structure.

static int dev_open(struct inode *, struct file *);  // this is first ops performed on a device file. equivalent to open() system call in user
static int dev_release(struct inode* , struct file *); // called when device file is closed. i.e., close() in user_space.
static ssize_t dev_read(struct file *, char *, size_t,loff_t *);  // to read device file. copy_to_user() will be used here.
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *); // this will write data from user to kernel.

/* this structure is used to assign our basic function to the standard file ops structure
 * which is defined in linux/fs.h */

static struct file_operations fops=
{
	.open=dev_open,
	.read=dev_read,
	.write=dev_write,
	.release=dev_release,
};

// standard initialization function will come in action when we load the device driver in kernel using insmod.
// __init is macro which means for built in driver and not any linux kernel module.

static int __init hitenchar_init(void)
{
	printk(KERN_ALERT "initiazing driver\n");
	majornumber = register_chrdev(0, DEVICE_NAME, &fops);  // to allocate device number dynamically
	if(majornumber< 0)
	{
		printk(KERN_ALERT "FAILED TO REGISTER\n");
		return majornumber;
	}
	printk(KERN_ALERT "registered with %d major number\n",majornumber);

	/* THIS_MODULE is macro defined is module.h which is basically pointer to the module
	 *  which owns this structure. this is used to being prevented from unloading the module while in use.*/

	hitencharclass = class_create( THIS_MODULE, CLASS_NAME);   // creating class for device. 
	if(hitencharclass< 0)
	{
		printk(KERN_ALERT "FAILED TO CREATE A CLASS\n");
		return -1;
	}
	printk(KERN_ALERT "device class registered\n");

	/* after creation of class for device, the next step is to create device for our driverr.
	 * after this device will be alive in our system. good to remember that only after device creation
	 * we cann  call the function from file ops.*/

	hitenchardevice = device_create( hitencharclass , NULL , MKDEV(majornumber, 0), NULL, DEVICE_NAME);
	
	if(hitenchardevice< 0)
	{
		printk(KERN_ALERT "failed to create device\n");
		return -1;
	}
	printk(KERN_ALERT "Device has been registered\n");
	return 0;
}


// when we unload using rmmod. following ops will be performed.

static void __exit hitenchar_exit(void)
{
	device_destroy(hitencharclass,MKDEV(majornumber,0));
	class_unregister(hitencharclass);
	class_destroy(hitencharclass);
	unregister_chrdev(majornumber, DEVICE_NAME);
	printk(KERN_INFO "DEVICE MODULE UNLOADED\n");
}

// dev_open will take two arguments where inodep and filep are the pointer to the structures defined is fs.h

static int dev_open(struct inode* inodep, struct file* filep)
{
	printk(KERN_INFO "DEVICE HAS BEEN OPENED\n");
	return 0;
}

// to copy the block of data to user space.

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
	error_count = copy_to_user( buffer,message,size_of_message);   // standard format (to, from, size)
	if(error_count ==0)
	{
		printk(KERN_ALERT "SENT %d CHAR SENT TO USER\n",size_of_message);
		return (size_of_message=0);
	}
	else
	{
		printk(KERN_ALERT "failed to sent\n");
		return 0;
	}
}	


// copy the data from user space to kernel space

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	sprintf(message ," %s(%zu letters)", buffer, len);   // %zu specifier is used when perform with size_t data type.
	size_of_message = strlen(message);
	printk(KERN_INFO "recieved %zu char from user and char are %s\n", len,message);
	return len;
}

// this will simply close the device file pointed by same open().

static int dev_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "DEVICE CLOSED\n");
	return 0;
}

module_init(hitenchar_init);
module_exit(hitenchar_exit);

// this are the macros defined in kernel.h and it tells kernel about the driver which we can see by modifo command.

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("HITEN GONDALIYA");    
MODULE_DESCRIPTION("MY VERY FIRST CHAR DRIVER");
MODULE_VERSION("0.1");            
