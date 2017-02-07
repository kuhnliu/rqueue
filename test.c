#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "rqueue.h"

struct rqueue *rqueue = NULL;

void *
func_enqueue(void *arg)
{
	long i;
	int num = atoi((char *)arg);
	for (i = 1; i <= num; i++) {
		while(RQUEUE_QUEUE_FULL == rqueue_enqueue(rqueue, (void *)(long)i)){
		}
	}
	
	return NULL;
}

void *
func_dequeue(void *arg)
{
	int i = 0;
	long item = 0;
	int num = atoi((char *)arg);
	for (i = 1; i <= num; i++) {
		while(RQUEUE_QUEUE_EMPTY == rqueue_dequeue(rqueue, (void **)&item)){
		}
		printf(" [%d:%ld]", i, item);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thd1, thd2;

	assert(argc == 3);
	assert(atoi(argv[1]) >= atoi(argv[2]));

	rqueue = rqueue_init(100);
	printf ("enqueue %d\n", atoi(argv[1]));
	printf ("dequeue %d\n", atoi(argv[2]));
	pthread_create(&thd1, NULL, func_enqueue, argv[1]);
	pthread_create(&thd2, NULL, func_dequeue, argv[2]);
	pthread_join(thd1, NULL);
	pthread_join(thd2, NULL);
	return printf("\n==>remain %u-%u %u\n", (unsigned int)(rqueue->head), (unsigned int)(rqueue->tail), (unsigned int)(rqueue->head - rqueue->tail));
}
