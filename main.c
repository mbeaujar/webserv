#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

struct var {
	int n;
};


void *routine(void *b) {
	struct var *a = (struct var*)b;
	while (a->n < 10000) {
		// printf("%d ", a->n);
		a->n += 1;
		// fflush(NULL);
	}
	return NULL;
}


int main() {
	pthread_t id;
	pthread_t ide;
	struct var a;
	a.n = 5;

	pthread_create(&id, NULL, routine, &a);
	
	printf("NTM\n");
	pthread_join(id, NULL);
	return 0;
}