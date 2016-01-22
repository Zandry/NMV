#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");

void affiche_block(struct super_block *sb, void * a){
	pr_info("uuid=%X-%X-%X-%X-%X%X type=%s\n", *((int*)(&(sb->s_uuid)[0])), 
							*((short*)(&(sb->s_uuid)[4])), 
							*((short*)(&(sb->s_uuid)[6])), 
							*((short*)(&(sb->s_uuid)[8])), 
							*((short*)(&(sb->s_uuid)[10])),*((int*)(&(sb->s_uuid)[12])), (sb->s_type)->name);
}

static int hello_init(void)
{
	pr_info("Hello, world\n");
	/* Parcourir la liste des super_block */
	iterate_supers(affiche_block, NULL);
	/**/
	return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
	pr_info("Goodbye, cruel world\n");
}
module_exit(hello_exit);
