#include<linux/input.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/io.h>
#include<linux/init.h>
#include<linux/workqueue.h>
#include <linux/slab.h>

#define BUTTON_PORT 0

static void work_function(struct work_struct *work);

static struct input_dev *button;

static struct workqueue_struct *queue;

static unsigned long delay;

struct delayed_work *dwork;

DECLARE_DELAYED_WORK(work, work_function);

static void work_function(struct work_struct *work)
{
	
	printk(KERN_INFO "workqueue\n");
	input_report_key(button, BTN_0, inb(BUTTON_PORT) & 1);
	input_sync(button);
	dwork=(struct delayed_work *)kmalloc(sizeof(struct delayed_work), GFP_KERNEL);


        INIT_DELAYED_WORK((struct delayed_work*)dwork, work_function);

        queue_delayed_work(queue, dwork, delay);


}



static int __init insert_module(void)
{
	int error;

	button= input_allocate_device()	;

	if(!button)
	{
		printk(KERN_ALERT "device has been not allocated\n");
		error = -ENOMEM;
		return error;
	}

	button -> evbit[0] = BIT_MASK(EV_KEY);
	button -> keybit[BIT_WORD(BTN_0)]= BIT_MASK(BTN_0);

	error = input_register_device(button);

	if (error) 
	{
		printk(KERN_INFO "failed to register device\n");

		input_free_device(button);
	}
	
	delay= msecs_to_jiffies(1000);
	printk(KERN_INFO "work loaded\n");

	queue = create_singlethread_workqueue("isub.c");
	queue_delayed_work(queue, &work,delay);

	return 0;
}

static void remove_module(void)
{

	input_unregister_device(button);
	cancel_delayed_work(dwork);
        flush_workqueue(queue);

        destroy_workqueue(queue);

}

module_init(insert_module);
module_exit(remove_module);

MODULE_LICENSE("GPL");
