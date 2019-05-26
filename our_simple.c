#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

#include <linux/sched/signal.h>


void findOldestChildren(struct task_struct *task){
	int counter=0;
	pid_t pid=-1;
	char *executableName="No children.";
	int child_start_time=-1;
	int min_start_time=-1;
	struct list_head *list;
	struct task_struct *child;


	list_for_each(list, &task->children){
		child= list_entry(list, struct task_struct, sibling);
		printk(KERN_ALERT "----IAMACHILD---- PID/NAME: %d/%s\n", child->pid, child->comm);
		if(counter==0)	min_start_time= child->start_time;
		child_start_time = child->start_time;
		printk("CHILD START TIME: %d", child_start_time);
		if(child_start_time < min_start_time){
			min_start_time=child_start_time;
			pid = child->pid;
			executableName = child->comm;
		}
		findOldestChildren(child);
		counter++;
	}
	printk(KERN_ALERT "**OLDEST** PID/NAME: %d/%s\n", pid, executableName);
}

int processID=0;
static int simple_init(void)
{
	struct task_struct *task;
	struct list_head *list;

	pid_t child_pid;
	int min_start_time;
	int child_start_time;
	bool pid_found=false;
	
	if(processID==-1) printk(KERN_ALERT "No processID provided.");
	else{
		for_each_process(task){
			printk(KERN_ALERT "PID/NAME: %d/%s\n", task->pid, task->comm);

			if (task->pid == processID){
				pid_found=true;
				printk(KERN_ALERT "I KNOW YOUR PID!!!");
				findOldestChildren(task);
			}		
		}
		if(pid_found){
		printk(KERN_ALERT "Module param: %d", processID );
		printk(KERN_ALERT "hello...\n");
		}
		else printk(KERN_ALERT "There is no such process.\n");
	}
	return 0;
}

static void simple_cleanup(void)
{
	printk(KERN_WARNING "bye1 ...\n");
}

module_init(simple_init);
module_exit(simple_cleanup);
module_param(processID, int ,0);
