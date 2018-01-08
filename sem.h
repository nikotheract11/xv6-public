struct sem_t {
	uint locked;
	struct spinlock lk;
};
typedef struct sem_t sem_t;	

//void sem_init(sem_t *sem, int value);

//void sem_down(sem_t *sem);
//void sem_up(sem_t *sem);
