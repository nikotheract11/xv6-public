#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
struct a{
	int val;
	int pos[10];
} A[10];

void acq_w(char *addr);
void rel_w(char *addr);
void setupsems(char* addr);
int
main(void)
{
	sem_t sem;
	sem_init(&sem,9);
	char key[15]="11111111111";
	char *addr;
	
	addr = (char*)shmget(11,key);
	int i=1997;
//	setupsems(addr);
//	acq_w(addr);
	memmove(addr,&i,sizeof(int));
//	rel_w(addr);
	for(i=0;i<10;i++){
		if(A[i].val != 0) printf(1,"--------------\n");
	}
	for(int i=0;i<100000000;i++){
                for(int j=1;j<1000000000;j++){
			for(int h=0;h<10000000000;h++){
                        	float a=0.1;
	                        a*=i;
        	                a/=j;
			}
                }
        }
//	char f;
//	gets(&f,100);
	int j;
	memmove(&j,addr,sizeof(int));

	printf(1,"iii=%d\n",j);
	exit();
}

void setupsems(char* addr){
	char init_byte = 1;
	int off=sizeof(char),readers=0;
	sem_t r_sem,w_sem;
        sem_init(&r_sem,1);
        sem_init(&w_sem,1);

	memmove(addr,&init_byte,sizeof(char));
	
	memmove(&addr[off],&r_sem,sizeof(sem_t));
	off+=sizeof(sem_t);

	memmove(&addr[off],&w_sem,sizeof(sem_t));
        off+=sizeof(sem_t);

        memmove(&addr[off],&readers,sizeof(int));
        off+=sizeof(int);


}


void acq_r(char *addr){
        int readers=0;
        sem_t *r_sem,*w_sem;
//	getData(&init_byte,&readers,w_sem,r_sem);
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

void acq_w(char *addr){
        sem_t *w_sem,*r_sem;
    //    getData(&init_byte,&readers,w_sem,r_sem);
	r_sem = (sem_t*)(addr + sizeof(char)) ;
        w_sem = (sem_t*)(r_sem + sizeof(sem_t));    	

	sem_down(w_sem);

}


void rel_w(char *addr){
        sem_t *w_sem,*r_sem;
//        getData(&init_byte,&readers,w_sem,r_sem);
	r_sem = (sem_t*)(addr + sizeof(char)) ;
        w_sem = (sem_t*)(r_sem + sizeof(sem_t));
        sem_up(w_sem);
}
