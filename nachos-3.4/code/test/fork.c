#include "syscall.h"
// #include "system.h"

int global_cnt=0;

void sum(){

	int i;

	for (i=0;i<100;i++)
	{
	    global_cnt++;
	}
	Exit(i);
}

int main()
{
	// printf("in main\n");
	
	int i=0;
	
	global_cnt++;
	// DEBUG('a', "before fork\n");
	Fork(sum);
	// DEBUG('a', "after fork\n");
	Yield();
	// DEBUG('a', "after  yield\n");

	global_cnt++;
	// printf("after count increase\n");
	Fork(sum);
	// printf("after second fork\n");

	Yield();
	
	global_cnt++;
	Fork(sum);

	Yield();
	
	global_cnt++;
	Exit(global_cnt); 

}