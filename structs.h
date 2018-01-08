//typedef char sh_key_t[16];
struct sh_key{
	char key[16];
};
typedef struct sh_key* sh_key_t;

struct pair {
        int pid;
        int pos;
};

typedef struct pair pair;
struct sh_page {
	sh_key_t key;
	pair pairs[16];
	int counter;
	void* pa;
};

typedef struct sh_page sh_page;


