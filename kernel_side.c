#include <linux/init.h> 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
#include <linux/string.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEUCENG");
MODULE_DESCRIPTION("An example Linux kernel module.");
MODULE_VERSION("0.01");

static ssize_t read_file(char* filename, void* buffer, size_t size, loff_t offset)
{
	struct file  *f;
	mm_segment_t old_fs;
	ssize_t      bytes_read = 0;

	f = filp_open(filename, O_RDONLY, 0);
	if (!f)
	{
		printk(KERN_INFO "filp_open failed\n");
		return -ENOENT;
	}

	// prepare for calling vfs_* functions with kernel space allocated parameters
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	{
		bytes_read = vfs_read(f, buffer, size, &offset);
	}
	set_fs(old_fs);

	filp_close(f,NULL);

	return bytes_read;
}

static void one_process_info(char* waydoc)
{
  char buffer[128];
  const size_t size = 128;
  ssize_t bytes_read;
  int i;
  
  for(i=0; i<128; i++){
	  buffer[i]=0;
  }

  //char *waydoc = "/proc/544/status";
  bytes_read = read_file( waydoc, buffer, size, 0 );
  printk(KERN_INFO "\n\n%s\n", buffer);
  return;
}


struct task_struct *task;        /*    Structure defined in sched.h for tasks/processes    */
struct task_struct *task_child;        /*    Structure needed to iterate through task children    */
struct list_head *list;            /*    Structure needed to iterate through the list in each task->children struct    */

int all_task(void)                    /*    Init Module    */
{
    printk(KERN_INFO "%s","LOADING MODULE\n");    /*    good practice to log when loading/removing modules    */
    
    for_each_process( task ){            /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
        printk(KERN_INFO "\n PARENT PID: %d PROCESS: %s STATE: %ld", task->pid, task->comm, task->state);/*    log parent id/executable name/state    */

//        list_for_each(list, &task->children){                        /*    list_for_each MACRO to iterate through task->children    */
//
//            task_child = list_entry( list, struct task_struct, sibling );    /*    using list_entry to declare all vars in task_child struct    */
//    
//            printk(KERN_INFO "\nCHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld",task->comm, task->pid, /*    log child of and child pid/name/state    */
//                task_child->pid, task_child->comm, task_child->state);
//        }
        printk("-----------------------------------------------------");    /*for aesthetics*/
    }    
    

    return 0;

}


SYSCALL_DEFINE3(lab2,int,choice,char*,filePath_status,char*,filePath_cmdline){

	if(choice==1){		// no paramaters 

		printk(KERN_INFO "\nRight Usage:\n\t-all prints some information (process id and its argument/s) about all processes\n\t-p takes process id and prints the details of it\n\t-k takes process id\n");
	}

	else if(choice==2){ // paramater is -all

		all_task();
	}
	
	else if (choice==3){

		one_process_info(filePath_status);
		printk(KERN_INFO "Cmdline: ");
		one_process_info(filePath_cmdline);
	}
	
	else if(choice==4){

		printk(KERN_INFO "\nProgress was killed");
	}

	return 0;

}