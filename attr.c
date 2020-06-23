/* steps to use this driver.
 * 1. load using insmod attr.ko
 * 2. check if your sys file attribute is created using ls -l /sys/kernel/.
 * 3. switch to root user using sudo su.
 * 4. cd /sys/kernel/hiten_sysfs.
 * 5. cat my_sys.
 * 6. change value using echo > 123 my_sys.
 * 7. check again if value is changed using cat my_sys.
 * 8. remove using rmmod.               
 */



#include<linux/init.h>
#include<linux/module.h>
#include<linux/kobject.h> // for kernel objects
#include<linux/sysfs.h>
#include<linux/kernel.h>
#include<linux/stat.h> // for read/write permissions.

// we are using this variable to be read and written in main memory.

volatile int my_sys=0;  

static int __init insert_module(void);
static void __exit remove_module(void);

/*---------------mandatory sysfs ops functions prototypes--------------*/

// func_show will be called whenever we open/read something to sys attribute.

static ssize_t func_show( struct kobject *kobj, struct kobj_attribute *attr,
		char *buffer);



// func_store will be called whenever we write somthing to sysfs attribute.

static ssize_t func_store( struct kobject *kobj, struct kobj_attribute *attr,
		const char *buffer, size_t count);



// __ATTR macro will essentially set corrosponding attribute name, permissions and callback methods to be used when we access the sys file.

static struct kobj_attribute my_attribute =
__ATTR(my_sys , 0660, func_show, func_store);

// the below structure represnts things that shows up as directory in /sys.

static struct kobject *kobj;



static ssize_t func_show( struct kobject *kobj, struct kobj_attribute *attr,
		char *buffer)
{

	printk(KERN_INFO "the current value of buffer is %d",my_sys);
	return sprintf(buffer,"%d",my_sys);
}


static ssize_t func_store( struct kobject *kobj, struct kobj_attribute *attr,
		const char *buffer, size_t count)
{

	printk(KERN_INFO "value changing in process\n");
	sscanf(buffer,"%d",&my_sys);
	return count;
}



static int __init insert_module(void)
{

	int file;

	printk(KERN_INFO "your module has been inserted\n");



	// now we will create directory dynamically under sysfs/kernel/*.

	kobj= kobject_create_and_add("hiten_sysfs",kernel_kobj);

	if(!kobj)
		return -ENOMEM;
	else
		printk(KERN_INFO "you have created directory under sys/kernel");

	// now we will create our single sys file attribute in sysfs/kernel/*.

	file = sysfs_create_file(kobj, &my_attribute.attr);

	if(file)
		sysfs_remove_file(kobj ,&my_attribute.attr);

	return file;
}


static void __exit remove_module(void)
{

	kobject_put(kobj);
	sysfs_remove_file(kobj, &my_attribute.attr);
	printk(KERN_INFO "You have removed your module\n");

}

module_init(insert_module);
module_exit(remove_module);
MODULE_LICENSE("GPL");
