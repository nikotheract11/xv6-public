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
void
sys_sem_init(sem_t *sem, int value)
{
	initlock(&sem->lk,"semaphore");
	sem->locked = value;
}

void
sys_sem_down(sem_t *sem){
	acquire(&sem->lk);
	while (sem->locked <= 0){
		sleep(sem,&sem->lk);
	}
	sem->locked--;
	release(&sem->lk);
}

void
sys_sem_up(sem_t *sem){
	acquire(&sem->lk);
	sem->locked++;
	release(&sem->lk);
}

