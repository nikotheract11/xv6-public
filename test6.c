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
	strcpy(key.key,"1a11000011111111");
        char *addr;
        addr = (char*)shmget(&key);
        if((int)addr == -1) exit();
	sem_t r_sem,w_sem;
        memmove(&r_sem,&addr[sizeof(char)],sizeof(sem_t));
        memmove(&w_sem,&addr[sizeof(char)+sizeof(sem_t)],sizeof(sem_t));
	int i,j=3;
	char buff[50];
        memmove(&i,&j,sizeof(int));
        j=sizeof(char) + 2*sizeof(sem_t)+ sizeof(int);
	printf(1,"w=%d\n",(w_sem).locked);
        printf(1,"r=%d\n",(r_sem).locked);
	
	acq_r(addr,&r_sem,&w_sem);
        memmove(&buff,&addr[j],49*sizeof(char));
        rel_r(addr,&r_sem,&w_sem);
	printf(1,"test6 before:%s ",buff);

	acq_w(addr,&w_sem);
	i=2018;
	memmove(&addr[j],"test6 ",6*sizeof(char));
	rel_w(addr,&w_sem);
	acq_r(addr,&r_sem,&w_sem);
	printf(1,"\nfinal:%s\n ",&addr[j]);
	rel_r(addr,&r_sem,&w_sem);
	exit();

}
