/*
 * logManager.h
 *
 *  Created on: May 9, 2019
 *      Author: parallels
 */

#ifndef INC_LOGMANAGER_H_
#define INC_LOGMANAGER_H_
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#include "../inc/protocol.h"

#define MSG_SIZE 1024


//structure data
//typedef struct {
//	int seq;
//	char msg[MSG_SIZE];
//	float x;
//	float y;
//	float z;
//}DATA;

//구조체
typedef struct {
	unsigned int seq;		//message sequence
	int operation;			//surgeon operation start = 0, end = 1
	int loglv;				//log level
	char msgdata[MSG_SIZE];
}DATA;

const char msg[][100] = {
	"INFORMATION Message String",
	"WARNINGS Message String",
	"ERROR Message String",
	"CRITICAL Message String",
};

//function
struct mq_attr initAttribute();
mqd_t onOpenlogClient(struct mq_attr attr);
void onSendlogClient(mqd_t mq, struct mq_attr attr, DATA data);
void onCloselogClient(mqd_t mq);

//pThread
#define THREADS_COUNT 1
int thread_exit = 1; //1 = true, 0 =false
static int thread_id;
static pthread_t m_thread[THREADS_COUNT];
void start_thread(int num, char *tname);
void exit_thread(int num);

#endif /* INC_LOGMANAGER_H_ */
