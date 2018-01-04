struct sem_t {
	uint locked;
	struct spinlock lk;
}
	
