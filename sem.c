#include "types.h"                                                                      
#include "defs.h"                                                                       
#include "param.h"                                                                      
#include "x86.h"                                                                        
#include "memlayout.h"                                                                  
#include "mmu.h"                                                                        
#include "proc.h"                                                                       
#include "spinlock.h"                                                                   
#include "sleeplock.h"   
#include "sem.h"
int
sys_sem_init(void)
{
	int a;
	sem_t *sem;
	argptr(0,(char**)&sem,sizeof(sem_t));
	argint(1,&a);
	initlock(&sem->lk,"semaphore");
	sem->locked = a;
	return 0;
}

int
sys_sem_down(void)
{
	sem_t *sem;

        if(argptr(0,(char**)&sem,sizeof(sem_t) < 0)) cprintf("ERROR\n");
	acquire(&sem->lk);
	while (sem->locked == 0){
		sleep(sem,&sem->lk);
	}
	sem->locked--;
	release(&sem->lk);
	return 0;

}

int
sys_sem_up(void){
	sem_t *sem;
        argptr(0,(char**)&sem,sizeof(sem_t));

	acquire(&sem->lk);
	sem->locked++;
	release(&sem->lk);
	return 0;

}

