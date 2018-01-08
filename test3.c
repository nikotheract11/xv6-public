#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
#include "structs.h"
void acq_r(char* addr,sem_t *r_sem,sem_t *w_sem){
        int readers=0;
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

int
main(void)
{
	struct sh_key key;
	strcpy(key.key,"1111111111111111");
	char *add=(char*)shmget(&key);
	if((int)add < 0) exit();
	int i;
	sem_t r_sem,w_sem;
        memmove(&r_sem,&add[sizeof(char)],sizeof(sem_t));
        memmove(&w_sem,&add[sizeof(char)+sizeof(sem_t)],sizeof(sem_t));
	int j;
	 j=sizeof(char) + 2*sizeof(sem_t)+ sizeof(int);
	acq_r(add,&r_sem,&w_sem);
	memmove(&i,&add[j],sizeof(int));

	rel_r(add,&r_sem,&w_sem);
        printf(1,"i=%d\n",i);
        exit();
}

