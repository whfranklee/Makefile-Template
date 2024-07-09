#include "comDef.h"
#include "sender.h"
#include "receiver.h"
#include "responser12.h"
using namespace std;
/************************
�첽��Ϣ����ģ��:
(1) �Ƚ����ն���responser ע�ᵽ��Ϣת����(receiver)�У�
     ������ÿ��responser �����ȼ�(low, mid, high)��
(2) ͨ��sender���ͽ��յĶ���(responser)���ͣ�ִ�еĺ�
     ������������Ҫ�Ĳ�����
(3) ��Ϣת����(receiver) ����ѭ���Ӷ����а����ȼ���
     ȡ��Ϣ�����Ҹ�����Ϣ������(responser), ������Ϣ
     ת������Ӧ��responser��
(4) responer ��ע�ᵽreceiver �Ժ�Ϳ����߳��ڵȴ���Ϣ��
*************************/

static responser1 res1_instance;
static responser2 res2_instance;

 int main(void)
 {
      receiver* const rec_instance = getReceiverInstance(); //���constָ�룬ָ�����ݿɱ��ַ���ɱ�
      sender* const send_instance = getSenderInstance();
      rec_instance->addResponser(RES1, MID, &res1_instance);
      rec_instance->addResponser(RES2, HIGH, &res2_instance);
 	while(1){
		 MSG_INFO msgInfo;

		 msgInfo.resType = RES1;  //���ö�������
		 msgInfo.msg.eventID = R1_FUN1; //����Ӧ����ʲô����
		 send_instance->sendMsg(rec_instance, &msgInfo); //���Ϳ�ʼ��Ϣ����������

		 //rec_instance->deleteResponser(RES1);

		 msgInfo.msg.eventID = R1_FUN2;
		 int a = 10, b = 122;
		 send_instance->msgDataPack((char*)msgInfo.msg.msgData,
		 	sizeof(int), &a,
		 	sizeof(int), &b, -1);
		 send_instance->sendMsg(rec_instance, &msgInfo);

		 msgInfo.resType = RES2;
		 msgInfo.msg.eventID = R2_FUN1;
		 double c = 4.23, d = 2.32;
		 send_instance->msgDataPack(msgInfo.msg.msgData,
		 	sizeof(double), &c,
		 	sizeof(double), &d, -1);
		 send_instance->sendMsg(rec_instance, &msgInfo);

		 msgInfo.msg.eventID = R2_FUN2;
		 a = 20;
		 b = 30;
		 send_instance->msgDataPack(msgInfo.msg.msgData,
		 	sizeof(int), &a,
		 	sizeof(int), &b, -1);
		 send_instance->sendMsg(rec_instance, &msgInfo);

		 sleep(1);
 	}
 } 

