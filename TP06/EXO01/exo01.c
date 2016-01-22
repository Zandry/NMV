#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/kobject.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/decompress/mm.h>
#include <linux/shrinker.h>

MODULE_DESCRIPTION("A exo04 kernel module");
MODULE_AUTHOR("Jonathan - Minh-Hieu");
MODULE_LICENSE("GPL");
int target = 0;

module_param(target, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(target, "Process PID");

struct task_sample {
	struct list_head list; // q1
	cputime_t utime;
	cputime_t stime;
};

struct mutex m;  

struct task_monitor {
	struct pid *p;
	struct task_sample *first_sample;
	struct task_sample *last_sample;
};
struct task_monitor pid_for_monitor;
static struct task_struct *monitor_thread;


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

void freeHistory(struct task_sample *from)
{
	struct task_sample *ptr = from;
	struct list_head *pos, *tmp;

	list_for_each_safe(pos, tmp, &(from->list))
	{
		free(list_entry(pos, struct task_sample, list));
	}
	free(from);
}

bool get_sample (struct task_monitor *tm, struct task_sample *s)
{
	struct task_struct * task = get_pid_task(tm->p, PIDTYPE_PID);
	
	s->utime = task->utime;
	s->stime = task->stime;

	return pid_alive(task);
}

//q2
void save_sample(void){
	struct task_sample *s;
	mutex_lock(&m);
	
	s = (struct task_sample*)malloc(sizeof(struct task_sample));
	get_sample (&pid_for_monitor, s);
	INIT_LIST_HEAD(&(s->list));
	
	if(pid_for_monitor.last_sample == NULL){
		pid_for_monitor.first_sample = s;
	}else{
		list_add(&(s->list), &((pid_for_monitor.last_sample)->list));
	}
	pid_for_monitor.last_sample = s; 
	mutex_unlock(&m);
}

int monitor_fn(void *unused)
{
	struct task_sample s;
	while (!kthread_should_stop())
	{
		save_sample();
		if (get_sample (&pid_for_monitor, &s))
		{
			pr_info("Processus en vie !\n");
		}
		else
		{
			pr_warn("Processus mort !\n");
			break;
		}
	
		pr_info("PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), s.utime, s.stime);
		
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
	int count = 0;
	int tmp_count = 0;
	struct list_head *pos;
	struct task_sample *s;
	char tmp_buf[1024];
	buf[0] = 0;

	list_for_each_prev(pos,  &((pid_for_monitor.first_sample)->list))
	{
		s = list_entry(pos, struct task_sample, list);
		tmp_count = sprintf(tmp_buf, "PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), s->utime, s->stime);
		if (count + tmp_count < PAGE_SIZE - 1)
		{
			strcat(buf, tmp_buf);
			count += tmp_count;
		}
	}
		
	return count;
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
	mutex_init(&m); 
	pr_info("exo01 module loaded\n");
	sysfs_create_file(kernel_kobj, &(exo04_attrb.attr));
	pid_for_monitor.p = monitor_pid(target);
	pid_for_monitor.last_sample = NULL;
	if (pid_for_monitor.p != NULL)
		monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
	return 0;
}

static void exo04_exit(void)
{
	sysfs_remove_file(kernel_kobj, &(exo04_attrb.attr));
	
	if (monitor_thread)
		kthread_stop(monitor_thread);
		
	freeHistory(pid_for_monitor.first_sample);
	free_pid(pid_for_monitor.p);
	pr_info("exo01 module unloaded\n");
}

module_init(exo04_init);
module_exit(exo04_exit);
