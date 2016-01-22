#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>

MODULE_DESCRIPTION("A exo02 kernel module");
MODULE_AUTHOR("Jonathan - Minh-Hieu");
MODULE_LICENSE("GPL");
int target = 0;

/* Question 2*/
struct task_monitor {
	struct pid *p;
};

struct task_monitor pid_for_monitor;

static struct task_struct *monitor_thread;
/* ========== */


module_param(target, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(target, "Process PID");

int monitor_fn(void *unused)
{
	struct task_struct * task = get_pid_task(pid_for_monitor.p, PIDTYPE_PID);
	
	while (!kthread_should_stop())
	{
		if (pid_alive(task) == 1)
		{
			pr_info("Processus en vie !\n");
		}
		else
		{
			pr_warn("Processus mort !\n");
			break;
		}
		
		pr_info("PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), task->utime, task->stime);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(5*HZ);
		/* kthread lance une thread en mode système (kernel), s'il n'y a pas ces deux ligne et que */
	}	
	return 0;
}

struct pid * monitor_pid(pid_t nr)
{
	struct pid * s = find_get_pid(nr);
	
	if (s == NULL)
		pr_warn("Processus introuvable !\n");
	else
		pr_warn("Processus trouve !\n");
	return s;
}

void free_pid(struct pid * s)
{
	if (s != NULL)
	{
		put_pid(s);
		pr_warn("Compteur libere !\n");
	}
}

static int exo02_init(void)
{
	//struct pid * s;
	pr_warn("Exo02 module loaded\n");
	//s = monitor_pid(target);
	//free_pid(s);
	pid_for_monitor.p = monitor_pid(target);
	if (pid_for_monitor.p != NULL)
		monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
	return 0;
}

static void exo02_exit(void)
{
	if (monitor_thread)
		kthread_stop(monitor_thread);
	free_pid(pid_for_monitor.p);
	pr_warn("Exo02 module unloaded\n");
}

module_init(exo02_init);
module_exit(exo02_exit);
