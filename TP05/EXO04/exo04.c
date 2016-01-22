#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/kobject.h>

MODULE_DESCRIPTION("A exo04 kernel module");
MODULE_AUTHOR("Jonathan - Minh-Hieu");
MODULE_LICENSE("GPL");
int target = 0;

module_param(target, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(target, "Process PID");

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

void free_pid(struct pid * s)
{
	if (s != NULL)
	{
		put_pid(s);
		pr_warn("Compteur libere !\n");
	}
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

ssize_t my_strncpy(char *dest, const char *src, ssize_t n)
{
    ssize_t i;

   for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

   return i;
}

static ssize_t taskmonitor_show(struct kobject *kobj, 
			struct kobj_attribute *attr, char *buf)
{
	//get_sample (&pid_for_monitor, &sample);
	return sprintf(buf, "PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), sample.utime, sample.stime);
}

static ssize_t taskmonitor_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	/* Exo4 - q2 */
	// test buf
	
	if((strncmp(buf, "stop", 4) == 0) && (monitor_thread != NULL)){
		pr_info("Thread stop !");
		kthread_stop(monitor_thread);
		monitor_thread = NULL;
	}
	else if((strncmp(buf, "start", 5) == 0) && (monitor_thread == NULL)){
		pr_info("Thread start !");
		monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
	}
	/* */
	return count;
}

struct kobj_attribute exo04_attrb = __ATTR_RW(taskmonitor); 

static int exo04_init(void)
{
	pr_info("exo04 module loaded\n");
	sysfs_create_file(kernel_kobj, &(exo04_attrb.attr));
	pid_for_monitor.p = monitor_pid(target);
	if (pid_for_monitor.p != NULL)
		monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
	return 0;
}

static void exo04_exit(void)
{
	sysfs_remove_file(kernel_kobj, &(exo04_attrb.attr));
	if (monitor_thread)
		kthread_stop(monitor_thread);
	free_pid(pid_for_monitor.p);
	pr_info("exo04 module unloaded\n");
}

module_init(exo04_init);
module_exit(exo04_exit);
