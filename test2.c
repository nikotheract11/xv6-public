#include "types.h"
#include "stat.h"
#include "user.h"
int
main(void)
{
        char key[16]="11111111111";
        char *addr;
        addr = (char*)shmget(key);
	for(int i=0;i<10000000;i++){
		for(int j=1;j<10000000;j++){
			float a=0.1;
			a*=i;
			a/=j;
		}
	}
	int i,j=3;
	memmove(&i,&j,sizeof(int));
        printf(1,"i=%d\n",i);
	memmove(&i,addr,sizeof(int));
        exit();
}
