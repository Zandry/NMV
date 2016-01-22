#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/kobject.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/shrinker.h>
#include <linux/slab.h>

#define LLUI long long unsigned int

MODULE_DESCRIPTION("A exo02 kernel module with slab strategy");
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

struct kmem_cache * task_sample_slab;

struct mutex m;  

struct task_monitor {
	struct pid *p;
	unsigned long nb_sample;
	struct task_sample *last_sample;
};
struct task_monitor pid_for_monitor;
static struct task_struct *monitor_thread;
struct shrinker shrink;

static unsigned long shrink_scan(struct shrinker *shrink, struct shrink_control *sc)
{
	int nr_to_scan = sc->nr_to_scan;
	unsigned long freed = 0;
	
	struct list_head *first_sample;
	struct list_head *pos, *tmp;
	
	mutex_lock(&m);
	first_sample = ((pid_for_monitor.last_sample)->list).next;
	
	list_for_each_safe(pos, tmp, first_sample)
	{
		if (nr_to_scan > freed)
		{
			list_del(pos);
			//free(list_entry(pos, struct task_sample, list));
			kmem_cache_free(task_sample_slab, list_entry(pos, struct task_sample, list));
			freed++;
		}
		else
		{
			break;
		}
	}
	
	pid_for_monitor.nb_sample -= freed;
	pr_info("shrink_scan : %d objets demande,\n\t%lu objets liberes (reste %lu objets) !\n", nr_to_scan, freed, pid_for_monitor.nb_sample);
	mutex_unlock(&m);
	return freed;
}

static unsigned long shrink_count(struct shrinker *shrink, struct shrink_control *sc)
{
	pr_info("shrink_count : %lu objets trouves !\n", pid_for_monitor.nb_sample);
	return pid_for_monitor.nb_sample;
}



struct pid * monitor_pid(pid_t nr)
{
	struct pid * s = find_get_pid(nr);
	
	if (s == NULL)
		pr_warn("Processus introuvable !\n");
	else
		pr_info("Processus trouve !\n");
	return s;
}

void free_pid(struct pid * s)
{
	if (s != NULL)
	{
		put_pid(s);
		pr_info("Compteur libere !\n");
	}
}

/*void freeHistory(struct task_sample *from)
{
	struct list_head *pos, *tmp;

	list_for_each_safe(pos, tmp, &(from->list))
	{
		free(list_entry(pos, struct task_sample, list));
	}
	free(from);
}*/

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
	
	//s = (struct task_sample*)malloc(sizeof(struct task_sample));
	s = (struct task_sample*)kmem_cache_alloc(task_sample_slab, SLAB_HWCACHE_ALIGN);
	if (s == NULL)
	{
		pr_warn("Malloc failed !\n");
		mutex_unlock(&m);
		return;
	}
	get_sample (&pid_for_monitor, s);
	INIT_LIST_HEAD(&(s->list));
	
	if(pid_for_monitor.last_sample != NULL)
	{
		list_add(&(s->list), &((pid_for_monitor.last_sample)->list));
	}
	pid_for_monitor.last_sample = s;
	pid_for_monitor.nb_sample++;
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
	
		pr_info("PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), (LLUI)s.utime, (LLUI)s.stime);
		
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(1*HZ);
	}	
	return 0;
}

static ssize_t taskmonitor_show(struct kobject *kobj, 
			struct kobj_attribute *attr, char *buf)
{
	int count = 0;
	int tmp_count = 0;
	struct task_sample *s;
	struct list_head *first_sample = ((pid_for_monitor.last_sample)->list).next;
	char tmp_buf[1024];
	buf[0] = 0;

	list_for_each_entry_reverse(s, first_sample, list)
	{
		tmp_count = sprintf(tmp_buf, "PID %d usr %llu sys %llu\n", pid_nr(pid_for_monitor.p), (LLUI)s->utime, (LLUI)s->stime);
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

struct kobj_attribute exo02_attrb = __ATTR_RW(taskmonitor); 

static int exo02_init(void)
{
	mutex_init(&m);
	task_sample_slab = KMEM_CACHE(task_sample,  SLAB_NOLEAKTRACE);
	shrink.count_objects = shrink_count;
	shrink.scan_objects = shrink_scan;
	shrink.seeks = DEFAULT_SEEKS;
	register_shrinker(&shrink);
	sysfs_create_file(kernel_kobj, &(exo02_attrb.attr));
	pid_for_monitor.p = monitor_pid(target);
	pid_for_monitor.last_sample = NULL;
	if (pid_for_monitor.p != NULL)
		monitor_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
		
	pr_info("exo02 module loaded\n");
	return 0;
}

static void exo02_exit(void)
{
	unregister_shrinker(&shrink);
	sysfs_remove_file(kernel_kobj, &(exo02_attrb.attr));
	
	if (monitor_thread)
		kthread_stop(monitor_thread);
		
	//freeHistory(pid_for_monitor.last_sample);
	kmem_cache_destroy(task_sample_slab);
	free_pid(pid_for_monitor.p);
	pr_info("exo02 module unloaded\n");
}

module_init(exo02_init);
module_exit(exo02_exit);