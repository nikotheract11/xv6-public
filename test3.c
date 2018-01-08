#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "sem.h"
#include "structs.h"
void acq_r(char* addr,sem_t *r_sem,sem_t *w_sem){
        int readers=0;
        sem_down(r_sem);

        readers++;
        memmove(&addr[sizeof(char)+2*sizeof(sem_t)],&readers,sizeof(int));

        if(readers == 1)
                sem_down(w_sem);
        sem_up(r_sem);
}

void rel_r(char *addr,sem_t *r_sem,sem_t *w_sem){
        int readers=0;
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
//        char key[16]="111011111111";
	//char key2[16]="11111111111";
  //      char *addr;//,*addr2;
	struct sh_key key;
	strcpy(key.key,"11111111111");
        //addr = (char*)shmget(2,key2);
	char *add=(char*)shmget(&key);

	//addr2 = (char*)shmget(key2);
	int i;
//	sh_key_t key="11111111111";
//	printf(1,"cmp=%d\n",strcmp(key,key2));
	sem_t r_sem,w_sem;
        memmove(&r_sem,&add[sizeof(char)],sizeof(sem_t));
        memmove(&w_sem,&add[sizeof(char)+sizeof(sem_t)],sizeof(sem_t));

	int j;
	 j=sizeof(char) + 2*sizeof(sem_t)+ sizeof(int);
	acq_r(add,&r_sem,&w_sem);
	//memmove(&i,&addr[j],sizeof(int));
	memmove(&i,&add[j],sizeof(int));
	memmove(&i,&add[j],sizeof(int));

	rel_r(add,&r_sem,&w_sem);
        printf(1,"aaaaaa=%d\n",i);
        exit();
}

