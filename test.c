#include "types.h"
#include "stat.h"
#include "user.h"
struct a{
	int val;
	int pos[10];
} A[10];
int
main(void)
{
	char key[16]="11111111111";
	char *addr;
	addr = (char*)shmget(key);
	int i=1997;
	memmove(addr,&i,sizeof(int));

	for(i=0;i<10;i++){
		if(A[i].val != 0) printf(1,"--------------\n");
	}
	int j;
	memmove(&j,addr,sizeof(int));

	printf(1,"%d,iii=%d\n",(int)addr,j);
	exit();
}
