#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "mqueue.h"
#define NTIMES 5*1024*1024
mqd_t mymq;
struct mq_attr mymqattr;
struct mq_attr mqattr;
void* threadfunc(void* para)
{
	char buf[1024];
	int i;
	for (i = 0; i < NTIMES; i++){
		if (mq_receive(mymq, buf, 1024, NULL) == -1){
			perror("mq_recvive");
			exit(EXIT_FAILURE);
		}
	}
	return NULL;
}
int main()
{
	pthread_t t1;
	int i;
	char buf[1024];
	mymqattr.mq_flags = 0;
	mymqattr.mq_maxmsg = 10;
	mymqattr.mq_msgsize = 1024;
	mymqattr.mq_curmsgs = 0;
	mymq = mq_open("/mymq", O_RDWR | O_CREAT | O_EXCL, S_IRWXU, &mymqattr);
	if (mq_getattr(mymq, &mqattr) == -1){
		perror("mq_getattr");
		exit(EXIT_FAILURE);
	}
	//printf("mq_flags = %d/n", mqattr.mq_flags);
	//printf("mq_maxmsg = %d/n", mqattr.mq_maxmsg);
	//printf("mq_msgsize = %d/n", mqattr.mq_msgsize);
	//printf("mq_curmsgs = %d/n", mqattr.mq_curmsgs);
	if (mymq == -1){
		perror("mq_open");
		exit(EXIT_FAILURE);
	}
	pthread_create(&t1, NULL, threadfunc, NULL);
	for (i = 0; i < NTIMES; i++){
		if (mq_send(mymq, buf, 1024, 0) == -1){
			perror("mq_send");
			exit(EXIT_FAILURE);
		}
	}
	pthread_join(t1, NULL);
	if (mq_close(mymq) == -1){
		perror("mq_close");
		exit(EXIT_FAILURE);
	}
	if (mq_unlink("/mymq") == -1){
		perror("mq_unlink");
		exit(EXIT_FAILURE);
	}
	return 0;
}
