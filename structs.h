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
	struct sh_key key;
	pair pairs[16];
	int counter;
	void* pa;
};

typedef struct sh_page sh_page;


