#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/kobject.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#define HELLO _IOR('N',0,char*)
#define WHO _IOR('N',1,char*)

MODULE_DESCRIPTION("A exo05 kernel module");
MODULE_AUTHOR("Jonathan - Minh-Hieu");
MODULE_LICENSE("GPL");

struct file_operations ops;
int major_num = 0;
const char *name = "hello";
char driver_content[1024] = "Hello ioctl !";

/*
Function: long driver_cmd (struct file * f, unsigned int requete, unsigned long param)
@f: descriptor 
@requete: query number
@param: a number of type long possible to be converted to pointer of type char. This variable used to stock data from (to) driver
*/
long driver_cmd (struct file * f, unsigned int requete, unsigned long param)
{
	char * ptr = (char *) param;
	char buf[512];
	switch(requete)
	{
		case HELLO:
			copy_to_user(ptr, driver_content, strlen(driver_content));
			break;
		case WHO:
			copy_from_user(buf, ptr, 512);
			strcpy(driver_content, "Hello ");
			strcat(driver_content, buf);
			strcat(driver_content, " !");
			break;
		default:
			pr_err("Bad command !\n");
			return -ENOTTY;
			break;
	}
	return 0;
}

static int exo05_init(void)
{
	pr_info("exo05 module loaded\n");
	
	ops.unlocked_ioctl = driver_cmd;
	
	major_num = register_chrdev(0, name, &ops);
	
	pr_info("Hello : %d\n", major_num);
	return 0;
}

static void exo05_exit(void)
{
	unregister_chrdev(major_num, name);
	pr_info("exo05 module unloaded\n");
}

module_init(exo05_init);
module_exit(exo05_exit);
