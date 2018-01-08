#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
void acq_r(char *addr){
        int readers=0;
        sem_t *r_sem,*w_sem;
//      getData(&init_byte,&readers,w_sem,r_sem);
        r_sem = (sem_t*)(addr + sizeof(char)) ;
        w_sem = (sem_t*)(r_sem + sizeof(sem_t));
        sem_down(r_sem);

        readers++;
        memmove(&addr[sizeof(char)+2*sizeof(sem_t)],&readers,sizeof(sem_t));

        if(readers == 1)
                sem_down(w_sem);
        sem_up(r_sem);
}

void rel_r(char *addr){
        int readers=0;
        sem_t *r_sem,*w_sem;
  //      getData(&init_byte,&readers,w_sem,r_sem);
        r_sem = (sem_t*)(addr + sizeof(char)) ;
        w_sem = (sem_t*)(r_sem + sizeof(sem_t));

        sem_down(r_sem);

        readers--;
        memmove(&addr[sizeof(char)+2*sizeof(sem_t)],&readers,sizeof(sem_t));

        if(readers == 0)
                sem_up(w_sem);
        sem_up(r_sem);
}


int
main(void)
{
//        char key[16]="111011111111";
	char key2[16]="11111111111";
        char *addr;//,*addr2;
        addr = (char*)shmget(2,key2);
	char *add=(char*)shmget(2,key2);

	//addr2 = (char*)shmget(key2);
int i;	
	int j;
	 j=sizeof(char) + 2*sizeof(sem_t)+ sizeof(int);
//	acq_r(addr);
	memmove(&i,&addr[j],sizeof(int));
	memmove(&i,&add[j],sizeof(int));
//	memmove(&i,&addr[j],sizeof(int));

//	rel_r(addr);
        printf(1,"aaaaaa=%d\n",i);
        exit();
}

