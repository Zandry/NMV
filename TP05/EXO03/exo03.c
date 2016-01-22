#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/kobject.h>

MODULE_DESCRIPTION("A exo03 kernel module");
MODULE_AUTHOR("Jonathan - Minh-Hieu");
MODULE_LICENSE("GPL");

char exo3_content[PAGE_SIZE] = "Hello sysfs !";

ssize_t my_strncpy(char *dest, const char *src, ssize_t n)
{
    ssize_t i;

   for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

   return i-1;
}

static ssize_t exo03attr_show(struct kobject *kobj, 
			struct kobj_attribute *attr, char *buf)
{
	return my_strncpy(buf, exo3_content, PAGE_SIZE);
}

static ssize_t exo03attr_store(struct kobject *kobj, 
			struct kobj_attribute *attr, const char *buf, size_t count)
{
	return my_strncpy(exo3_content, buf, PAGE_SIZE);
}

struct kobj_attribute exo03_attrb = __ATTR_RW(exo03attr); 

static int exo03_init(void)
{
	pr_info("Exo03 module loaded\n");
	sysfs_create_file(kernel_kobj, &(exo03_attrb.attr));
	return 0;
}

static void exo03_exit(void)
{
	sysfs_remove_file(kernel_kobj, &(exo03_attrb.attr));
	pr_info("Exo03 module unloaded\n");
}

module_init(exo03_init);
module_exit(exo03_exit);
