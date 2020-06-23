#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/init.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hiten_gondaliya");
MODULE_DESCRIPTION("WORKQUEUE");


static void work_function(struct work_struct *work);



static unsigned long delay;

static struct workqueue_struct *queue;

struct delayed_work *dwork;




DECLARE_DELAYED_WORK(work, work_function);

static void work_function(struct work_struct *work)
{
	printk(KERN_INFO "workqueue example\n");
	dwork=(struct delayed_work *)kmalloc(sizeof(struct delayed_work), GFP_KERNEL);
	
	
	INIT_DELAYED_WORK((struct delayed_work*)dwork, work_function);
	
	queue_delayed_work(queue, dwork, delay);
	
}

//DECLARE_DELAYED_WORK(work, work_function);

static int __init insert_module(void)
{
	delay= msecs_to_jiffies(1000);
	printk(KERN_INFO "Work loaded \n");

		queue = create_singlethread_workqueue("wqinit.c");
		queue_delayed_work(queue, &work,delay);
		
	return 0;
}

static void remove_module(void)
{
	cancel_delayed_work(dwork);
	flush_workqueue(queue);
	
	destroy_workqueue(queue);
	
}


module_init(insert_module);
module_exit(remove_module);
