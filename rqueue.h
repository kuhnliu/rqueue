#ifndef __RQUEUE_H_
#define __RQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#define RQUEUE_QUEUE_FULL -1
#define RQUEUE_QUEUE_EMPTY -2

struct rqueue {
	unsigned int len;
	unsigned int head;
	unsigned int tail;
	void *queue[0];
};

static struct rqueue *
rqueue_init(unsigned int queue_len)
{
	struct rqueue *rqueue = NULL;
	int malloc_size = sizeof(struct rqueue)+sizeof(void *)*queue_len;
	rqueue = (struct rqueue *)malloc(malloc_size);
	if (rqueue != NULL) {
		memset(rqueue, 0, malloc_size);
		rqueue->len = queue_len;
		rqueue->head = 0;
		rqueue->tail = 0;
	}
	return rqueue;
}

static void
rqueue_destroy(struct rqueue *rqueue)
{
	free(rqueue);
}

static int
rqueue_enqueue(struct rqueue *rqueue, void *item)
{
	if((unsigned int)(rqueue->head - rqueue->tail) < rqueue->len) {
		rqueue->queue[rqueue->head % rqueue->len] = item;
		rqueue->head += 1;
		return 0;
	}
	return RQUEUE_QUEUE_FULL;
}

static int
rqueue_dequeue(struct rqueue *rqueue, void **item)
{
	if ((unsigned int)(rqueue->head - rqueue->tail) > 0) {
		*item = rqueue->queue[rqueue->tail % rqueue->len];
		rqueue->tail += 1;
		return 0;
	}
	return RQUEUE_QUEUE_EMPTY;
}

#ifdef __cplusplus
}
#endif
#endif
