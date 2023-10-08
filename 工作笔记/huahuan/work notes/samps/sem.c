#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pthread.h"
#include "semaphore.h"
#include "assert.h"
#define MAX_QUEUE_SIZE 10
#define NTIMES 5*1024*1024
sem_t sem_empty;
sem_t sem_occupy;
char queue[MAX_QUEUE_SIZE + 1][1024];
int front,rear;
void insertQueue(char *buf, unsigned size)
{
	sem_wait(&sem_empty);
	memcpy(queue[front], buf, size);
	front = (front + 1) % (MAX_QUEUE_SIZE + 1);
	sem_post(&sem_occupy);
	assert(front != rear);
}
void deleteQueue(char *buf, unsigned size)
{
	sem_wait(&sem_occupy);
	assert(front != rear);
	memcpy(buf, queue[rear], size);
	rear = (rear + 1) % (MAX_QUEUE_SIZE + 1);
	sem_post(&sem_empty);
}
void* threadfunc(void* para)
{
	char buf[1024];
	int i;
	for (i = 0; i < NTIMES; i++){
		deleteQueue(buf, 1024);
	}
	return NULL;
}
int main()
{
	pthread_t t1;
	int i;
	char buf[1024];
	front = 0;
	rear = 0;
	sem_init(&sem_occupy, 0, 0);
	sem_init(&sem_empty, 0, MAX_QUEUE_SIZE);
	pthread_create(&t1, NULL, threadfunc, NULL);
	for (i = 0; i < NTIMES; i++){
		insertQueue(buf, 1024);
	}
	pthread_join(t1, NULL);
	sem_destroy(&sem_occupy);
	sem_destroy(&sem_empty);
	return 0;
}
