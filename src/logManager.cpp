/*
 * logManager.cpp
 *
 *  Created on: May 9, 2019
 *      Author: parallels
 */

#include "../inc/logManager.h"
#include "../inc/linux_kbhit.h"

struct mq_attr initAttribute()
{
	struct mq_attr attr;
	//First set up the attribute structure
	attr.mq_maxmsg = 30;
	attr.mq_msgsize = MSG_SIZE; //1024
	attr.mq_flags = 0; // BLOCK or NON-BLOCK

	return attr;
}

mqd_t  onOpenlogClient(struct mq_attr attr)
{
	//open msg queue
	mqd_t mq;
	mq = mq_open(QUEUE, O_RDWR | O_CREAT, 0666, &attr);

	return mq;
}

void onSendlogClient(mqd_t mq, struct mq_attr attr, DATA data)
{
	int i = 0;
	//set send data

	while(!_kbhit()) //아무 키 입력시 종료
	{
		memset(data.msgdata, 0x00, sizeof(data.msgdata));

		int selected = LOG_LEVEL_INFORMATION;
		switch(selected)
		{
		case LOG_LEVEL_INFORMATION:
			{
				data.loglv = LOG_LEVEL_INFORMATION;
				memcpy(data.msgdata, msg[0], sizeof(msg[0]));
				data.seq = i;
				break;
			}
		case LOG_LEVEL_WARNING:
			{
				data.loglv = LOG_LEVEL_WARNING;
				memcpy(data.msgdata, msg[1], sizeof(msg[1]));
				data.seq = i;
				break;
			}
		case LOG_LEVEL_ERROR:
			{
				data.loglv = LOG_LEVEL_ERROR;
				memcpy(data.msgdata, msg[2], sizeof(msg[2]));
				data.seq = i;
				break;
			}
		case LOG_LEVEL_CRITICAL:
			{
				data.loglv = LOG_LEVEL_CRITICAL;
				memcpy(data.msgdata, msg[3], sizeof(msg[3]));
				data.seq = i;
				break;
			}
		}

		//메세지큐 전송
		if((mq_send(mq, (char*)&data, attr.mq_msgsize, 1)) == -1)
		{
			perror("send error : ");
			exit(0);
		}

		//전송된 데이터 인쇄
		printf("send %d type : %x text : %s \n", data.seq, data.loglv, data.msgdata);
		fflush(stdout);
		i++;
		sleep(1);

	}

	return;
}

void onCloselogClient(mqd_t mq)
{
	// This will now be a temporary queue...as soon as it's closed,
	// it will be removed
	mq_unlink(QUEUE);
	//Close all open message queue descriptor
	mq_close(mq);
}

void *logManagerProc(void *arg)
{
	DATA data;
	mqd_t mqdes;
	struct mq_attr mqattr; //to store queue attributes
//	struct sigevent sigevent; //for notification

	mqattr = initAttribute();
	mqdes = onOpenlogClient(mqattr);
	if(mqdes == -1)
	{
		perror("open error ");
		exit(0);
	}
	onSendlogClient(mqdes, mqattr, data);
	onCloselogClient(mqdes);
	return NULL;
}

void start_thread(int num, char *tname)
{
	// 쓰레드 시작
	thread_exit = 0; //종료 온오프
	thread_id = pthread_create(&m_thread[num], NULL, logManagerProc, (void*)tname);

	return;
}


void exit_thread(int num)
{
	// 쓰레드 종료
	thread_exit = 1; //종료 온오프
	pthread_join(m_thread[num], NULL);

	return;
}

int main(int argc, char **argv)
{

	char name1[] = "logManager"; //쓰레드 이름

	start_thread(0, name1); //쓰레드 시작

	exit_thread(0); //쓰레드 종료


	return 0;
}


