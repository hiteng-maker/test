#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hiten_gondaliya");
MODULE_DESCRIPTION("WORKQUEUE");


static struct workqueue_struct *queue;

static void work_function(struct work_struct *work)
{
	printk(KERN_INFO "workqueue example\n");
}

DECLARE_WORK(work, work_function);

static int __init insert_module(void)
{
	queue = create_workqueue("wqinit.c");
	queue_work(queue, &work);
	
	return 0;
}

static void remove_module(void)
{
	cancel_work_sync(&work);
	flush_workqueue(queue);

	destroy_workqueue(queue);
}


module_init(insert_module);
module_exit(remove_module);
