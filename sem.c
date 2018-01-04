#include "types.h"                                                                      
#include "defs.h"                                                                       
#include "param.h"                                                                      
#include "x86.h"                                                                        
#include "memlayout.h"                                                                  
#include "mmu.h"                                                                        
#include "proc.h"                                                                       
#include "spinlock.h"                                                                   
#include "sleeplock.h"   

void
sem_init(sem_t *sem, int value)
{
	initlock(&sem->lk,"semaphore");
	sem->locked = value;
}

void
sem_down(sem_t *sem){
	ackquire(&sem->lk);
	while (lk->locked <= 0){
		sleep(sem,&sem->lk);
	}
	sem->locked--;
	release(&sem->lk);
}

void
sem_up(sem_t sem){
	acquire(&sem->lk);
	sem->locked++;
	release(&sem->lk);
}

