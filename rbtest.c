#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "rbuff.h"

#define MAX_MSG_LEN 20

struct msg {
    int len;
    int seq;
    char data[MAX_MSG_LEN];
};
struct rbuff *rbuff = NULL;

pthread_mutex_t iolock = PTHREAD_MUTEX_INITIALIZER;

void *
func_enqueue(void *arg)
{
    struct msg msg;
    void *buffer = NULL;
    int i, writen = 0, left = 0;
	int num = atoi((char *)arg);
    srandom(time(NULL));
	for (i = 1; i <= num; i++) {
        msg.len = random()%MAX_MSG_LEN;
        msg.seq = i;
        buffer = &msg;
        left = msg.len + ((int)((long)(&(((struct msg *)0)->data))));
        while (left > 0) {
		    writen = rbuff_write(rbuff, buffer, left);
            buffer += writen;
            left = left - writen;
        }
        pthread_mutex_lock(&iolock);
		printf("write message %d, length %d\n", msg.seq, msg.len);
        pthread_mutex_unlock(&iolock);
	}
	
	return NULL;
}

void *
func_dequeue(void *arg)
{
    struct msg msg;
    void *buffer = NULL;
    int i, readn = 0, left = 0;
	int num = atoi((char *)arg);
	for (i = 1; i <= num; i++) {
        buffer = &msg;
        left = (int)(long)(&(((struct msg *)0)->data));
		while(left > 0){
            readn = rbuff_read(rbuff, buffer, left);
            buffer += readn;
            left -= readn;
		}
        buffer = &msg.data;
        left = msg.len;
        while(left > 0) {
            readn = rbuff_read(rbuff, buffer, left);
            buffer += readn;
            left -= readn;
        }
        pthread_mutex_lock(&iolock);
		printf("read message %d, length %d\n", msg.seq, msg.len);
        pthread_mutex_unlock(&iolock);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thd1, thd2;

	assert(argc == 3);
	assert(atoi(argv[1]) >= atoi(argv[2]));

	rbuff = rbuff_init(1024);
	printf ("enqueue %d\n", atoi(argv[1]));
	printf ("dequeue %d\n", atoi(argv[2]));
	pthread_create(&thd1, NULL, func_enqueue, argv[1]);
	pthread_create(&thd2, NULL, func_dequeue, argv[2]);
	pthread_join(thd1, NULL);
	pthread_join(thd2, NULL);
	return printf("\n==>remain %u-%u %u\n", (unsigned int)(rbuff->head), (unsigned int)(rbuff->tail), (unsigned int)(rbuff->head - rbuff->tail));
}
