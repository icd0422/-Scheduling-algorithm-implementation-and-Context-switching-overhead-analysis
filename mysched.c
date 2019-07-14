#include <linux/module.h>
#include "../sched.h"

/*
 * HCPARK: my scheduling class: ONE_PROCESS_SCHED
 * Modify the above "include sched.h" line according to your environment
 *
 */

extern void (*set_class_my) (struct task_struct *p);

struct task_struct *p;

//extern struct sched_class idle_sched_class;

static void
check_preempt_curr_fifo(struct rq *rq, struct task_struct *p, int flags)
{
	/* we're never preempted */
	printk(KERN_DEBUG "MYMOD: check_preempt_curr_fifo CALLED\n");
}

static struct task_struct *
pick_next_task_fifo(struct rq *rq, struct task_struct *prev, struct rq_flags *rf)
{
	//printk(KERN_DEBUG "MYMOD: pick_next_task_fifo CALLED\n");

	if(p!=NULL) {
		put_prev_task(rq, prev);
		//printk(KERN_DEBUG "MYMOD: pick_next_task_fifo return task p\n");
		return p;
	}
		
	return NULL;
}

static void
enqueue_task_fifo(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "MYMOD: enqueue_task_fifo CALLED\n");
	add_nr_running(rq, 1);
}

static void
dequeue_task_fifo(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "MYMOD: dequeue_task_fifo CALLED\n");
	sub_nr_running(rq, 1);
}

static void yield_task_fifo(struct rq *rq)
{
	printk(KERN_DEBUG "MYMOD: yield_task_fifo CALLED\n");
	BUG();
}

static void put_prev_task_fifo(struct rq *rq, struct task_struct *prev)
{

}

static void task_tick_fifo(struct rq *rq, struct task_struct *curr, int queued)
{	
	//printk(KERN_DEBUG "MYMOD: task_tick_fifo CALLED\n");
}

static void set_curr_task_fifo(struct rq *rq)
{
	printk(KERN_DEBUG "MYMOD: set_curr_task_fifo CALLED\n");
}

static void switched_to_fifo(struct rq *rq, struct task_struct *new)
{
	printk(KERN_DEBUG "MYMOD: switched_to_fifo CALLED new = %x\n", new);
	p = new;
}

static void
prio_changed_fifo(struct rq *rq, struct task_struct *p, int oldprio)
{
	printk(KERN_DEBUG "MYMOD: prio_changed_fifo CALLED\n");
}

static unsigned int
get_rr_interval_fifo(struct rq *rq, struct task_struct *task)
{
	printk(KERN_DEBUG "MYMOD: get_rr_interval_fifo CALLED\n");
	return 0;
}

static void update_curr_fifo(struct rq *rq)
{
}

const struct sched_class my_sched_class = {
	.next			= &idle_sched_class,

	.enqueue_task		= enqueue_task_fifo,
	.dequeue_task		= dequeue_task_fifo,
	.yield_task		= yield_task_fifo,

	.check_preempt_curr	= check_preempt_curr_fifo,

	.pick_next_task		= pick_next_task_fifo,
	.put_prev_task		= put_prev_task_fifo,


	.set_curr_task          = set_curr_task_fifo,
	.task_tick		= task_tick_fifo,

	.get_rr_interval	= get_rr_interval_fifo,

	.prio_changed		= prio_changed_fifo,
	.switched_to		= switched_to_fifo,
	.update_curr		= update_curr_fifo,
};

void myclass (struct task_struct *p) {
		printk(KERN_DEBUG "MYMOD: myclass CALLED\n");
		p->sched_class = &my_sched_class;
}

static int __init init_mysched(void)
{
	const struct sched_class *class;
	
	for_each_class(class)
		printk(KERN_DEBUG "INIT_MOD: class = %x\n", class);		
	
	set_class_my = myclass;

	fair_sched_class.next = &my_sched_class;
	
	p = NULL;

	for_each_class(class)
		printk(KERN_DEBUG "INIT_MOD: class = %x\n", class);

	return 0;
	
}

static void __exit exit_mysched(void)
{
	const struct sched_class *class;

	p = NULL;

	set_class_my = NULL;

	fair_sched_class.next = &idle_sched_class;
	
	for_each_class(class)
		printk(KERN_DEBUG "EXIT_MOD: class = %x\n", class);

	return;

}

MODULE_AUTHOR("Hyunchan Park");
MODULE_DESCRIPTION("My Scheduler");
MODULE_LICENSE("GPL");

module_init(init_mysched)
module_exit(exit_mysched)


