#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
#include "structs.h"
void acq_r(char* addr,sem_t *r_sem,sem_t *w_sem){
        int readers;
	memmove(&readers,&addr[sizeof(char)+2*sizeof(sem_t)],sizeof(int));
        sem_down(r_sem);
	memmove(addr + sizeof(char),r_sem,sizeof(sem_t));

        readers++;
        memmove(&addr[sizeof(char)+2*sizeof(sem_t)],&readers,sizeof(int));

        if(readers == 1){
                sem_down(w_sem);
		memmove(addr + sizeof(char)+sizeof(sem_t),w_sem,sizeof(sem_t));
	}

        sem_up(r_sem);
	memmove(addr + sizeof(char),r_sem,sizeof(sem_t));
}

void rel_r(char *addr,sem_t *r_sem,sem_t *w_sem){
        int readers;
	memmove(&readers,&addr[sizeof(char)+2*sizeof(sem_t)],sizeof(int));
        sem_down(r_sem);
	memmove(addr + sizeof(char),r_sem,sizeof(sem_t));

        readers--;
        memmove(&addr[sizeof(char)+2*sizeof(sem_t)],&readers,sizeof(int));

        if(readers == 0){
                sem_up(w_sem);
		 memmove(addr + sizeof(char)+sizeof(sem_t),w_sem,sizeof(sem_t));
	}

        sem_up(r_sem);
	memmove(addr + sizeof(char),r_sem,sizeof(sem_t));
}

/* This test reads the integer that test.c wrote 
 * and removes the shared pages from its virtual address space
 * */
	
int
main(void)
{
	struct sh_key key;
	strcpy(key.key,"1111111111111111");
        char *addr;
        addr = (char*)shmget(&key);
	if((int)addr == -1) exit();
	for(int i=0;i<1000000000;i++){
                for(int j=1;j<100000000;j++){
                        for(int h=0;h<100000000;h++){
                                float a=0.1;
                                a*=i;
                                a/=j;
                        }
                }
        }
	sem_t r_sem,w_sem;
	memmove(&r_sem,&addr[sizeof(char)],sizeof(sem_t));
	memmove(&w_sem,&addr[sizeof(char)+sizeof(sem_t)],sizeof(sem_t)); 
	printf(1,"w=%d\n",(w_sem).locked);
        printf(1,"r=%d\n",(r_sem).locked);
	int i,j=3;
	memmove(&i,&j,sizeof(int));
	j=sizeof(char) + 2*sizeof(sem_t)+ sizeof(int);
	acq_r(addr,&r_sem,&w_sem);
	memmove(&i,&addr[j],sizeof(int));
	rel_r(addr,&r_sem,&w_sem);
	printf(1,"i=%d\n",i);
	shmrem(&key);
	for(int i=0;i<10000000000;i++){
                for(int j=1;j<10000000000;j++){
                        for(int h=0;h<10000000000;h++){
                                float a=0.1;
                                a*=i;
                                a/=j;
                        }
                }
        }

        exit();
}

