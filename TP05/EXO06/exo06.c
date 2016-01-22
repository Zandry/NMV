#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/kobject.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#define GET_SAMPLE _IOR('N',0,char*)
#define TASKMON_STOP _IOR('N',1,void*)
#define TASKMON_START _IOR('N',2,void*)
#define TASKMON_SET_PID _IOW('N',3,int)

MODULE_DESCRIPTION("A exo06 kernel module");
MODULE_AUTHOR("Jonathan - Minh-Hieu");
MODULE_LICENSE("GPL");
int target = 0;
module_param(target, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(target, "Process PID");

struct file_operations ops;
int major_num = 0;
const char *name = "exo06";

struct task_sample {
	cputime_t utime;
	cputime_t stime;
};

struct task_monitor {
	struct pid *p;
};

struct task_monitor pid_for_monitor;
static struct task_struct *monitor_thread;
struct task_sample sample;

struct pid * monitor_pid(pid_t nr)
{
	struct pid * s = find_get_pid(nr);
	
	if (s == NULL)
		pr_warn("Processus introuvable !\n");
	else
		pr_warn("Processus trouve !\n");
	return s;
}


bool get_sample (struct task_monitor *tm, struct task_sample *s)
{
	struct task_struct * task = get_pid_task(tm->p, PIDTYPE_PID);
	
	s->utime = task->utime;
	s->stime = task->stime;

	return pid_alive(task);
}

int monitor_fn(void *unused)
{
	while (!kthread_should_stop())
	{
		if (get_sample (&pid_for_monitor, &sample))
		{
			pr_info("Processus en vie !\n");
		}
		else
		{
			pr_warn("Processus mort !\n");
			break;
		}
	
		pr_info("PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), sample.utime, sample.stime);
		
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(5*HZ);
	}	
	return 0;
}

long driver_cmd (struct file * f, unsigned int requete, unsigned long param)
{
	// char * ptr = (char *) param;
	char status[512];
	sprintf(status, "PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), sample.utime, sample.stime);
	switch(requete)
	{
		case GET_SAMPLE:
			copy_to_user(ptr, status, strlen(status));
			break;
		case TASKMON_STOP:
			if (monitor_thread != NULL)
			{
				pr_info("Thread stop !");
				kthread_stop(monitor_thread);
				monitor_thread = NULL;
			}
			break;
		case TASKMON_START:
			if (monitor_thread == NULL)
			{
				pr_info("Thread start !");
				monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
			}
			break;
		case TASKMON_SET_PID:
			pr_info("Monitoring pid : %d", param);
			free_pid(pid_for_monitor.p);
			pid_for_monitor.p = monitor_pid(param);
			if ((pid_for_monitor.p == NULL) && (monitor_thread != NULL))
			{
				pr_info("Thread stop !");
				kthread_stop(monitor_thread);
				monitor_thread = NULL;
			}
			else if ((pid_for_monitor.p != NULL) && (monitor_thread == NULL))
			{
				pr_info("Thread start !");
				monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
			}
			break;
		default:
			pr_err("Bad command !\n");
			return -ENOTTY;
			break;
	}
	return 0;
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
	
	ops.unlocked_ioctl = driver_cmd;
	
	major_num = register_chrdev(0, name, &ops);
	
	pr_info("Driver major num : %d\n", major_num);
	
	if (pid_for_monitor.p != NULL)
		monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
	
	return 0;
}

static void exo02_exit(void)
{
	unregister_chrdev(major_num, name);
	if (monitor_thread)
		kthread_stop(monitor_thread);
	free_pid(pid_for_monitor.p);
	pr_warn("Exo02 module unloaded\n");
}

module_init(exo02_init);
module_exit(exo02_exit);
