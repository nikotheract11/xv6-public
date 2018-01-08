#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
#include "structs.h"

void acq_w(char* addr,sem_t *w_sem){
        sem_down(w_sem);
        memmove(addr + sizeof(char)+sizeof(sem_t),w_sem,sizeof(sem_t));


}


void rel_w(char* addr,sem_t *w_sem){
        sem_up(w_sem);
        memmove(addr + sizeof(char)+sizeof(sem_t),w_sem,sizeof(sem_t));
}
void acq_r(char* addr,sem_t *r_sem,sem_t *w_sem){
        int readers=0;
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
        int readers=1;
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
        strcpy(key.key,"11111111111");
        char *addr;
        addr = (char*)shmget(&key);
        if((int)addr == -1) exit();
	sem_t r_sem,w_sem;
        memmove(&r_sem,&addr[sizeof(char)],sizeof(sem_t));
        memmove(&w_sem,&addr[sizeof(char)+sizeof(sem_t)],sizeof(sem_t));
	int i,j=3;
        memmove(&i,&j,sizeof(int));
        j=sizeof(char) + 2*sizeof(sem_t)+ sizeof(int);

	acq_r(addr,&r_sem,&w_sem);
        memmove(&i,&addr[j],sizeof(int));
        rel_r(addr,&r_sem,&w_sem);
	printf(1,"test4 before:%d ",i);

	acq_w(addr,&w_sem);
	i=2018;
	memmove(&addr[j],&i,sizeof(int));
	rel_w(addr,&w_sem);
	printf(1,"after should be:%d\n",i);
	exit();

}


