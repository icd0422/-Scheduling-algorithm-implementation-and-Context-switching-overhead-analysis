#include<sched.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/types.h>
#define TOTALFORK 5 //총 생성해야할 프로세스 수
#define SCHED_MYFIFO 7

 struct sched_attr {
   __u32 size;

   __u32 sched_policy;
   __u64  sched_flags;

   /* SCHED_NORMAL, SCHED_BATCH */
   __s32 sched_nice;

   /* SCHED_FIFO, SCHED_RR */
   __u32 sched_priority;

   /* SCHED_DEADLINE (nsec) */
   __u64 sched_runtime;
   __u64 sched_deadline;
   __u64 sched_period;
 };

static int sched_setattr(pid_t pid, const struct sched_attr* attr, unsigned int flags)
{
	return syscall(SYS_sched_setattr, pid, attr, flags) ;
}

int main()
{
	pid_t pids[TOTALFORK], pid;
	int runProcess = 0; //생성한 프로세스 수
	int state;
	
	int result ;
	
	struct sched_attr attr ;
	
	attr.size = sizeof(attr);
	attr.sched_flags = 0;
	attr.sched_nice = 0;
	attr.sched_priority = 0;
	
	attr.sched_policy = SCHED_MYFIFO;
    attr.sched_runtime = 10 * 1000 * 1000;
    attr.sched_period = attr.sched_deadline = 30 * 1000 * 1000;
	
	
	result = syscall(314,getpid(),&attr,0);//sched_setattr(getpid(),&attr,0) ;
	if(result == -1)
		{
			perror("Error calling sched_setattr") ;
		}
	
	while(runProcess < TOTALFORK) { //5개의 프로세스를 loop 를 이용하여 생성
	
		pids[runProcess] = fork();//fork 생성
		
		//pid = wait(&state); 
		
        //0보다 작을 경우 에러 (-1)
		if(pids[runProcess] < 0) 
		{
			return -1;
		}
		
		else if(pids[runProcess] == 0) 
		{//자식 프로세스
			int t = 0 ;
		
			while(1)
				{
					int i,j,k ;
					
					int sum = 0 ;
		
					for(i=0 ; i<100 ; i++)
						{
							for(j=0 ; j<1000 ; j++)
								{
									for(k=0 ; k<1000 ; k++)
										{
											sum = sum + i - j + k ;
										}
								}
						}
		
					t++;
					printf("%d번 프로세스 %d번 진행됨\n", runProcess, t) ;
					sum = 0 ;
					if(t==3)//계산기 3번 되면 종료 되게끔 해줌
					{
						printf("%d프로세스 종료됨\n", runProcess);
						break ;
					}					
				}
			exit(0);
		} 
		
		else 
		{ //부모 프로세스
			//printf("parent %ld, child %ld\n", (long)getpid(), (long)pids[runProcess]);
			//printf("%d번 자식프로세스 생성\n",runProcess);
			
			runProcess++;
		}
	}
	
	printf("부모프로세스 종료됨\n") ;
	
	return 0 ;
}