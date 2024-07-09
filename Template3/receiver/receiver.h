#ifndef __RECEIVER__
#define __RECEIVER__
#include "comDef.h"

class responser;
/* ��Ϣ������(��Ϣת������) */
class receiver
{
public:
	receiver();
	~receiver();
	void processMsg();  //������Ϣ������(Ӧ����) ����Ϣת����Ӧ����
	void addResponser(RES_TYPE resType, RES_PRIOR res_prior, responser* const pRes); //ע��Ӧ����
	void deleteResponser(RES_TYPE resType); //ɾ��Ӧ����
	void addMsgIntoQue(MSG_INFO msgInfo);  //����Ϣ����ȴ�����
private:
	typedef std::map<RES_TYPE, responser* const> RECE_MAP; //�������ͼ�����ָ��ӳ��
	RECE_MAP recMap;   
	RES_PRIOR *priortyMap;   //�洢Ӧ���������ȼ�
	std::queue<MSG_INFO> recHighQueue;   // ��Ϣ����-- ���ȼ���
	std::queue<MSG_INFO> recMidQueue;   // ��Ϣ����-- ���ȼ���ͨ
	std::queue<MSG_INFO> recLowQueue;   // ��Ϣ����-- ���ȼ���
	pthread_t recThreadId;
	pthread_mutex_t mutex;
	static int responserMaxNum;  //��������ɵ�Ӧ��������
	int responserNum;
};

receiver* getReceiverInstance();
#endif
