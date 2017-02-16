#ifndef __RBUFF_H_
#define __RBUFF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>


struct rbuff {
	unsigned int len;
	unsigned int head;
	unsigned int tail;
	char buff[0];
};

static struct rbuff *
rbuff_init(unsigned int buff_len)
{
	struct rbuff *rbuff = NULL;
	int malloc_size = sizeof(struct rbuff)+sizeof(void *)*buff_len;
	rbuff = (struct rbuff *)malloc(malloc_size);
	if (rbuff != NULL) {
		memset(rbuff, 0, malloc_size);
		rbuff->len = buff_len;
		rbuff->head = 0;
		rbuff->tail = 0;
	}
	return rbuff;
}

static void
rbuff_destroy(struct rbuff *rbuff)
{
	free(rbuff);
}

static int
rbuff_write(struct rbuff *rbuff, const void *buf, int len)
{
    int writen = 0;
	while(((rbuff->head - rbuff->tail) < rbuff->len) && len--) {
		rbuff->buff[rbuff->head % rbuff->len] = ((char *)buf)[writen++];
		rbuff->head += 1;
	}
    return writen;
}

static int
rbuff_read(struct rbuff *rbuff, void *buf, int len)
{
    int readn = 0;
	while (((rbuff->head - rbuff->tail) > 0) && len--) {
		((char *)buf)[readn++] = rbuff->buff[rbuff->tail % rbuff->len];
		rbuff->tail += 1;
	}
    return readn;
}

#ifdef __cplusplus
}
#endif
#endif
