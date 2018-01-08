#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
#include "structs.h"

void acq_w(char *addr,sem_t *w_sem);
void rel_w(char *addr,sem_t *w_sem);
char* setupsems(char* addr,sem_t*,sem_t*);

/* This test writes an interger (1997) on the first place after semaphores 
 * also checks if fork() works fine and the restrictions
*/
int
main(void)
{
	sem_t r_sem,w_sem;
	char *addr;
	struct sh_key key;
        strcpy(key.key,"1a11000011111111");
	if((addr = (char*)shmget(&key))<(char*)0) exit();
	char buff[100] = "This is a shared memory test";
	addr=setupsems(addr,&r_sem,&w_sem);

	acq_w(addr,&w_sem);
	strcpy(addr,buff);
	rel_w(addr,&w_sem);
	printf(1,"s=%s\n",addr);

//	if(fork()<0) exit();
//	if(fork()<0) exit();
//	if(fork()<0) exit();
	printf(1,"w=%d\n",w_sem.locked);
	printf(1,"r=%d\n",r_sem.locked);
	for(int i=0;i<100000000;i++){
                for(int j=1;j<1000000000;j++){
			for(int h=0;h<10000000000;h++){
                        	float a=0.1;
	                        a*=i;
        	                a/=j;
			}
                }
        }
	exit();
}

char* setupsems(char* addr,sem_t *r_sem,sem_t *w_sem){
	char init_byte = 1;
	int off=sizeof(char),readers=0;
        sem_init(r_sem,1);
        sem_init(w_sem,1);

	memmove(addr,&init_byte,sizeof(char));
	
	memmove(&addr[off],r_sem,sizeof(sem_t));
	off+=sizeof(sem_t);

	memmove(&addr[off],w_sem,sizeof(sem_t));
        off+=sizeof(sem_t);

        memmove(&addr[off],&readers,sizeof(int));
        off+=sizeof(int);
	return &addr[off];

}

void acq_w(char* addr,sem_t *w_sem){
	sem_down(w_sem);
	memmove(addr + sizeof(char)+sizeof(sem_t),w_sem,sizeof(sem_t));


}


void rel_w(char* addr,sem_t *w_sem){
        sem_up(w_sem);
	memmove(addr + sizeof(char)+sizeof(sem_t),w_sem,sizeof(sem_t));
}
