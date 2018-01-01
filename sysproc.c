#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_shmget(sh_key_t key) {
	int i,counter=0,pos;
	void* va,pa;									// virtual address
	pde_t *pgdir = myproc()->pgdir;
	int fe=-1;									// first empty pos
	char feflag=0;									// first empty space flag

	for(i=0;i<32;i++){
		if(shared[i] == 0 && !feflag){
			fe = i;
			feflag = 1;
			continue;
		}
		if(shared[i] != 0 && key == shared[i]->key){
			counter = shared[i]->counter;
			pa = shared[i]->pa;
			exists = 1;
			break;	
		}
	}
	if((i >= 32 && !feflag) || counter >= 16) return -1;						// 32 shared page allready
	//else if( i >= 32) i = fe;
	if(counter == 0 ) {
		shared[i]->key = key;
		pa = kalloc();
		if(pa == 0) {
			cprintf("System out of Memory\n");
			return -1;
		}
		shared[i]->pa = pa;
		// memset needed?
		memset(pa,0,PGSIZE);
	}
	for(int j=0;j<32;j++){
		if (myproc()->pos[j] <= 0) {
			myproc()->pos[j] = i+1; 					// i+1 to identify if this pos contains data or 0 from initialization
			break;
		}
	}
	va = (void*) KERNBASE - (j+1)*PGSIZE;
	if(mappages(pgdir, (char*)va, PGSIZE, V2P(pa), PTE_W|PTE_U) < 0)
		return -1;
	shared[i]->pairs[counter]->pid = myproc()->pid; 				// do i need proc pointer?
	shared[i]->pairs[counter]->pos = j;						// with j known, va = KERNBASE - (j+1)*PGSIZE
	shared[i]->counter++;
	return va;	// what to return?



}

int sys_shmrem(sh_key_t key){
	int i;
	for(i=0;i<32;i++){
		if(shared[i] == 0) 
			continue;
		if(shared[i]->key == key) {
			int pos;
			for(int j=0;j<16;j++){
				if (shared[i]->pairs[j]->pid == myproc()->pid){
					pos = shared[i]->pairs[j]->pos;
					shared[i]->pairs[j] = 0;
				}
				shared[i]->counter--;
				pte_t *p = walkpgdir(myproc()->pgdir,KERNBASE-myproc()->pos[pos]*PGSIZE,0);	//right indexing?? 
				*p = 0;
				myproc()->pos[pos] = -1;					 // free space for other sp
			}
			if(shared[i]->counter == 0) {						 // if last, free pa
				kfree(shared[i]->pa);
				shared[i] = 0;
				return 0;
			}

		}
	}
	return 1;
}

int sys_shmrem(sh_key_t key){
        int i;
        for(i=0;i<32;i++){
                if(shared[i] == 0)
                        continue;
                if(shared[i]->key == key)
			break;
	}
	int pos;
	if(i >= 32) 
		return -1;
        for(int j=0;j<16;j++){
                if (shared[i]->pairs[j]->pid == myproc()->pid){
                        pos = shared[i]->pairs[j]->pos;
			shared[i]->pairs[j] = 0;
                }
                shared[i]->counter--;
                pte_t *p = walkpgdir(myproc()->pgdir,KERNBASE-myproc()->pos[pos]*PGSIZE,0);
                *p = 0;
                myproc()->pos[pos] = -1;                                         // free space for other sp
        }
        if(shared[i]->counter == 0) {                                            // if last, free pa
                kfree(shared[i]->pa);
                shared[i] = 0;
                return 0;
        }
}
