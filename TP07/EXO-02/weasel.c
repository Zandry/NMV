#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/dcache.h>
#include <linux/proc_fs.h>

MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");

const char * weasel_msg = "I'm a weasel";

static int weasel_show(struct seq_file *m, void *v)
{
	struct hlist_node * pos;
	
	hlist_bl_for_each_entry(struct dentry, pos, (dentry_hashtable->first), d_hash)
	{
		seq_printf(m, "%s\n", (struct dentry)(pos)->d_name.name);
	}
}

static int weasel_open(struct inode *inode, struct file *file)
{
        return single_open(file, weasel_show, NULL);
}

/* Question 3 */
/*static ssize_t weasel_read (struct file *filp, char __user *buf, size_t size, loff_t *offp)
{
	return simple_read_from_buffer(buf, size, offp, weasel_msg, strlen(weasel_msg));
}

static const struct file_operations weasel_fops = {
         .owner = THIS_MODULE,
         .llseek = noop_llseek,
         .read = weasel_read,
         .open = simple_open,
 };*/
 
static const struct file_operations weasel_fops = {
         .owner = THIS_MODULE,
         .llseek = seq_lseek,
         .read = seq_read,
         .open = weasel_open,
         .release = single_release,
 };

static int hello_init(void)
{
	pr_info("Addresse : 0x%X, taille : %d\n", dentry_hashtable, 1 << d_hash_shift);
	
	proc_create("weasel", 0666, NULL, &weasel_fops);
	return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
	remove_proc_entry("weasel", NULL);
	pr_info("Goodbye\n");
}
module_exit(hello_exit);

