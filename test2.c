#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
void acq_r(char* addr,sem_t *r_sem,sem_t *w_sem){
        int readers=0;
        //sem_t *r_sem,*w_sem;
//      getData(&init_byte,&readers,w_sem,r_sem);
        //r_sem = (sem_t*)(addr + sizeof(char)) ;
        //w_sem = (sem_t*)(r_sem + sizeof(sem_t));
        sem_down(r_sem);

        readers++;
        memmove(&addr[sizeof(char)+2*sizeof(sem_t)],&readers,sizeof(int));

        if(readers == 1)
                sem_down(w_sem);
        sem_up(r_sem);
}

void rel_r(char *addr,sem_t *r_sem,sem_t *w_sem){
        int readers=0;
     //   sem_t *r_sem,*w_sem;
  //      getData(&init_byte,&readers,w_sem,r_sem);
       // r_sem = (sem_t*)(addr + sizeof(char)) ;
       // w_sem = (sem_t*)(r_sem + sizeof(sem_t));

        sem_down(r_sem);

        readers--;
        memmove(&addr[sizeof(char)+2*sizeof(sem_t)],&readers,sizeof(int));

        if(readers == 0)
                sem_up(w_sem);
        sem_up(r_sem);
}

	int
main(void)
{
        char key[15]="11111111111";
        char *addr;
        addr = (char*)shmget(21,key);
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
	printf(1,"OOOOO:%d\n",r_sem.locked);
	int i,j=3;
	memmove(&i,&j,sizeof(int));
	j=sizeof(char) + 2*sizeof(sem_t)+ sizeof(int);
       // printf(1,"i=%d\n",i);
	acq_r(addr,&r_sem,&w_sem);
	memmove(&i,&addr[j],sizeof(int));
	rel_r(addr,&r_sem,&w_sem);
	//char key2[15]="11111111111";
	//char *add=shmget(1,key2);
//	char f;
//	gets(&f,100);
	//memmove(&j,add,sizeof(int));
	printf(1,"+++++=%d\n",i);
	shmrem(1,key);
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

