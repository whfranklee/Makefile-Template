#ifndef __APL_MY_RESPONSER__
#define __APL_MY_RESPONSER__
#include "comDef.h"

/* ��ϢӦ��ĸ��� */
class responser
{
public:
	responser();
	~responser();
	virtual void onResponser() = 0;  //���麯��ʹ responser ����ʵ����
	void msgDataUnPack(char* msgData, ...);
	void setEventMsg(const EVENT_MSG* srcMsg);
       pthread_cond_t* getPthreadCond();
protected:
	EVENT_MSG eventMsg;
	pthread_cond_t	cond;
	pthread_mutex_t mutex;
};
#endif

