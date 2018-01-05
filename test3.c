#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
        char key[16]="111011111111";
	char key2[16]="111111111111";
        char *addr,*addr2;
        addr = (char*)shmget(key);
	addr2 = (char*)shmget(key2);
        printf(1,"%d,%d\n",(int)addr,(int)addr2);
        exit();
}

