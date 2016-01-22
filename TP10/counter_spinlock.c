#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>

MODULE_DESCRIPTION("A simple counter with spinlock synchronization");
MODULE_AUTHOR("Johnathan - Minh-Hieu");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.2");

#define MAX_COUNTERS 256

static unsigned short nr_workers = 16;
static unsigned long nr_iters = 24;
module_param(nr_workers, ushort, 0400);
module_param(nr_iters, ulong, 0400); /* 2^nr_iters iterations */

static struct task_struct *compute_thread[MAX_COUNTERS];
static struct task_struct *monitor_thread;

DECLARE_COMPLETION(counter_complete);

static long counter = 0; /* default to 0 */

DEFINE_SPINLOCK(counter_spin);

/*
 * Increment counter nr_iters times
 */
int inc_fn(void *data)
{
	unsigned long n;

	for (n = 0; n < 1<<nr_iters && !kthread_should_stop(); n++)
	{
		spin_lock(&counter_spin);
		counter++;
		spin_unlock(&counter_spin);
	}

/*	pr_info("end of thread %s\n", current->comm); */
	complete(&counter_complete);
	return 0;
}

/*
 * Decrement counter nr_iters times
 */
int dec_fn(void *data)
{
	unsigned long n;

	for (n = 0; n < 1<<nr_iters && !kthread_should_stop(); n++)
	{
		spin_lock(&counter_spin);
		counter--;
		spin_unlock(&counter_spin);
	}

/*	pr_info("end of thread %s\n", current->comm); */
	complete(&counter_complete);
	return 0;
}

/*
 * Start computing threads and monitor their execution
 */
int monitor_fn(void *data)
{
	int i;
	struct timespec start = {.tv_sec = 0, .tv_nsec = 0},
			end = {.tv_sec = 0, .tv_nsec = 0},
			delta;

	getnstimeofday(&start);

	/* start compute threads */
	for (i = 0; i < nr_workers; i++)
		wake_up_process(compute_thread[i]);

	/* wait for completion of computing threads */
	for (i = 0; i < nr_workers && !kthread_should_stop(); i++)
		if (wait_for_completion_interruptible(&counter_complete))
			return -ERESTARTSYS;

	getnstimeofday(&end);
	delta = timespec_sub(end, start);

	pr_info("counter=%ld time=%ld.%09ld\n", counter,
			delta.tv_sec, delta.tv_nsec);
	return 0;
}

static int counter_init(void)
{
	int i, err;

	if (nr_workers > MAX_COUNTERS)
		nr_workers = MAX_COUNTERS;

	 /* half threads does inc, half does dec */
	for (i = 0; i < nr_workers/2; i++) {
		compute_thread[i] = kthread_create(inc_fn, NULL, "inc (%d)", i);
		if (IS_ERR(compute_thread[i])) {
			err = PTR_ERR(compute_thread[i]);
			goto abort;
		}
	}

	for (; i < nr_workers; i++) {
		compute_thread[i] = kthread_create(dec_fn, NULL, "dec (%d)", i);
		if (IS_ERR(compute_thread[i])) {
			err = PTR_ERR(compute_thread[i]);
			goto abort;
		}
	}

	/*
	 * Start monitoring thread. We start computing threads inside
	 * monitoring thread to ensure time measurement consistency.
	 */
	monitor_thread = kthread_run(monitor_fn, NULL, "counter monitor");
	if (IS_ERR(monitor_thread)) {
		err = PTR_ERR(monitor_thread);
		goto abort;
	}

	return 0;
abort:
	for (i--; i >= 0; i--)
		kthread_stop(compute_thread[i]);
	return err;
}

/*
 * XXX: do not remove module until completion of all threads
 */
static void counter_exit(void)
{
	//spin_destroy(&counter_spin);
	pr_info("Counter with spinlock module unloaded");
}

module_init(counter_init);
module_exit(counter_exit);
