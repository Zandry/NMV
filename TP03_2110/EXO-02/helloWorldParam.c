#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");
char *whom = "null";
int howmany = 1;

module_param(whom, charp, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(whom, "A character string");
module_param(howmany, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(howmany, "An integer");

static int hello_init(void)
{
	int i = 0;
	for(i = 0; i<howmany; i++){
		pr_info("(%d)Hello, %s\n", i, whom);
	}
	return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
	pr_info("Goodbye, %s\n", whom);
}
module_exit(hello_exit);

