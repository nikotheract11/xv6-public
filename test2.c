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
        char key[16]="11111111111";
        char *addr;
        addr = (char*)shmget(21,key);
	for(int i=0;i<10000000;i++){
		for(int j=1;j<100000000;j++){
			float a=0.1;
			a*=i;
			a/=j;
		}
	}
	int i,j=3;
	memmove(&i,&j,sizeof(int));
        printf(1,"i=%d\n",i);
	memmove(&i,addr,sizeof(int));
	char key2[16]="42342342";
	char *add=shmget(1,key2);
//	char f;
//	gets(&f,100);
	memmove(&j,add,sizeof(int));
	printf(1,"strcmp=%d, j=%d\n",i,j);
        exit();
}

