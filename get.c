void*
shmget(char key[16])
{
        int i,counter=0;//,pos;
        void *va,*pa=0;                                                                 // virtual address
        pde_t *pgdir = myproc()->pgdir;
        int fe=-1;                                                                      // first empty pos
        char feflag=0;

	for(i=0;i<32;i++){
		if(shared[i].counter != 0 && !strncmp(key,shared[i].key,15)){
			feflag = 1;
			counter = shared[i].counter;
		}
	}
	if(feflag == 0)
		for(i=0;i<32;i++)
			if(shared[i].counter == 0) break;
	if(i<0 || i>32) return -1;
	if(feflag == 0){
		strncpy(shared[i].key,key,15);
		pa = kalloc();
		shared[i].pa=pa;
		memset(pa,0,PGSIZE);
	}
	for( j=0;j<32;j++){ 
		if (myproc()->pos[j] <= 0) {
			myproc()->pos[j] = i+1;
			break;
		}
	}
	va = (void*) (KERNBASE - (j+1)*PGSIZE); 
	if(map(pgdir, (char*)va, PGSIZE, V2P(pa), PTE_W|PTE_U) < 0) return -1;
	int k;
	for(k=0;k<16;k++)
		if(shared[i].pairs[k].pos==0) 
			break;
	shared[i].pairs[k].pid = myproc()->pid;
	shared[i].pairs[k].pos = j+1;
	shared[i].counter=++counter;
	return va;
}




