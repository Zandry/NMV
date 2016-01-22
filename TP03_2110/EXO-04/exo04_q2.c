#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");
char *type_name = "ext4";

module_param(type_name, charp, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(type_name, "typeSuperBlock");


void affiche_block(struct super_block *sb, void * a){
	/* extern struct timespec current_fs_time(struct super_block *sb); */
	/* static inline void getnstimeofday(struct timespec *ts){
		getnstimeofday64(ts);
 		}*/
 	struct timespec timespec_var = current_fs_time(sb); 
	pr_info("uuid=%X-%X-%X-%X-%X%X type=%s time = %ld.%ld\n", *((int*)(&(sb->s_uuid)[0])), 
							*((short*)(&(sb->s_uuid)[4])), 
							*((short*)(&(sb->s_uuid)[6])), 
							*((short*)(&(sb->s_uuid)[8])), 
							*((short*)(&(sb->s_uuid)[10])),*((int*)(&(sb->s_uuid)[12])), (sb->s_type)->name, timespec_var.tv_sec, timespec_var.tv_nsec);
}

static int hello_init(void)
{
	struct file_system_type *type = get_fs_type(type_name);
	pr_info("Hello, world\n");
	/* Parcourir la liste des super_block avec le type */
	iterate_supers_type(type, affiche_block, NULL);
	/* Libération de la ressource */
	put_filesystem(type);
	return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
	pr_info("Goodbye, cruel world\n");
}
module_exit(hello_exit);
