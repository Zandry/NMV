#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/utsname.h>

extern struct uts_namespace init_uts_ns;

MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");
char *whom = "null";
int howmany = 1;
char sysName[100];

module_param(whom, charp, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(whom, "A character string");
module_param(howmany, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(howmany, "An integer");

static int hello_init(void)
{
	pr_info("%s", init_uts_ns.name.sysname);
	int i = 0;
	for(i = 0; i<howmany; i++){
		pr_info("(%d)Hello, %s\n", i, whom);
	}
	strcpy(sysName, init_uts_ns.name.sysname);
	strcpy(init_uts_ns.name.sysname, "Monsieur NMV");
	
	return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
	pr_info("Goodbye, %s\n", whom);
	strcpy(init_uts_ns.name.sysname, sysName);
}
module_exit(hello_exit);

