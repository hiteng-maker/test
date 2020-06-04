/* -----------------steps to use this driver--------------------
 * 1. insert using insmod
 * 2. check dmesg if working well or not.
 * 3. run 'sudo evtest'.
 * 4. select event number.
 * 5. you will see events generating for value 0 and 1 respectively.
 */

/* this device has just two values 0 and 1. after reporting these two value events will be generated. the driver will look like below: */


#include<linux/input.h> // when you are playing with input drivers.
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/io.h>
#include<linux/init.h>
#include<linux/workqueue.h>
#include <linux/slab.h> // kmalloc


int count=0; 

static void work_function(struct work_struct *work);


// this structure represents an input device subsystem in kernel

static struct input_dev *button; 

static struct workqueue_struct *queue;

static unsigned long delay;  // delay to be given in workqueue scheduling.

struct delayed_work *dwork; // delayed work variable

DECLARE_DELAYED_WORK(work, work_function);

static void work_function(struct work_struct *work)
{
	
	printk(KERN_INFO "workqueue count = %d\n", count);

/* upon every one sec input_report_key will check the value of BTN_0. Here,
 * it will check for 0 and 1 after every one second*/

	input_report_key(button, BTN_0, count%2);
	input_sync(button);
	dwork=(struct delayed_work *)kmalloc(sizeof(struct delayed_work), GFP_KERNEL);


        INIT_DELAYED_WORK((struct delayed_work*)dwork, work_function);

        queue_delayed_work(queue, dwork, delay);

/* this will be responsible for changing the value of 0 and 1 to 3rd argument of input_report_key() upon every call of work_function after every one second. */
	count++; 

}

// upon inserting the driver following operations will be performed.

static int __init insert_module(void)
{
	int error;

     /*	It will allocate a new input device structure. This way it well tell
      *	other parts of input subsystem what it is. Our device will generate
      *	only EV_KEY type events and only from BTN_0 code. */

	button= input_allocate_device()	;

	if(!button)
	{
		printk(KERN_ALERT "device has been not allocated\n");
		error = -ENOMEM;
		return error;
	}
/* we have set only two bits to evbit and keybit.
 * evbit and keybit are members of the input_dev structure. Where,
 * evbit bitmaps types of events supported by device. Here, it's EV_KEY.
 * keybit bitmap of keys/button device has. Here, it's BTN_0 */

	button -> evbit[0] = BIT_MASK(EV_KEY);
	button -> keybit[BIT_WORD(BTN_0)]= BIT_MASK(BTN_0);

	// it register the input device structure to system.
	
	error = input_register_device(button);

	if (error) 
	{
		printk(KERN_INFO "failed to register device\n");

		input_free_device(button);
	}
	
	// It will give delay of one sec to the scheduling of work.
	
	delay= msecs_to_jiffies(1000);
	printk(KERN_INFO "work loaded\n");

	queue = create_singlethread_workqueue("isub.c");
	queue_delayed_work(queue, &work,delay);

	return 0;
}

static void remove_module(void)
{

	input_unregister_device(button);// will remove input device after rmmod
	cancel_delayed_work(dwork);
        flush_workqueue(queue);

        destroy_workqueue(queue);

}

module_init(insert_module);
module_exit(remove_module);

MODULE_LICENSE("GPL");
