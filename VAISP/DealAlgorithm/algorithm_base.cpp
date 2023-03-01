
#include "algorithm_base.h"
#include "..\DealFile\ini_helper.h"
#include "import_dll_class.h"
#include <QLibrary>
#include <QtConcurrent\QtConcurrent>
#include <QFile>
#include <QDir>

#include <winnt.rh>
#include <winbase.h>

//#pragma comment(lib, "ws2_32.lib") 	
#pragma execution_character_set("utf-8")
/********************************/


/**********************************/
#define MODELCODE_SDG  (QString)"SDG"
#define MODELCODE_LVW  (QString)"LVW"

#define CHECKITEMCODE_ZXXPC     (QString)"ZXXPC"
#define CHECKITEMCODE_HWWDTX    (QString)"HWWDTX"
#define CHECKITEMCODE_LVW_ZXWD  (QString)"LVW_ZXWD"
#define CHECKITEMCODE_LVW_DJWD  (QString)"LVW_DJWD"
#define CHECKITEMCODE_LVW_ZDZHZ  (QString)"LVW_ZDZHZ"
#define CHECKITEMNAME_LVW_ZXWD  "�����¶�"
#define CHECKITEMNAME_LVW_DJWD  "����¶�"
#define CHECKITEMNAME_LVW_ZDZHZ  "���ۺ�"

#define JCPCODE_FELZDZW  (QString)"M0020-09-04"
#define JCPCODE_FELSDG   (QString)"M0020-09-05"

#define RECORDID09    (QString)"20220521183547"
#define TIME09        (QString)"2022-05-21 18:35:47"
#define GROUPNAME09   (QString)"09005006"
#define JCPCODE09     (QString)"NJNZ"
#define VIDEOPATH     (QString)"D:\\ImageMovie\\Resources\\video\\2022052118354709123124NJNZ.mp4"

float djwd_alarm = 0;
float djwd_warn = 0;
float zhz_alarm = 0;
float zhz_warn = 0;
float zxwd_alarm = 0;
float zxwd_warn = 0;

void djwdAlarmSet(struct VIBRATE_DATA *pVectData, struct CARRIAGE *pCarriage, struct WHEELSET *pWeelSet, struct CHECKDATA_VIBRATE *pData)
{
	struct ALARM_DATAS alarm;
	alarm.groupName = pVectData->groupName;
	alarm.alarmTime = pVectData->checkTime;
	alarm.carriagePos = pCarriage->carriagePos;
	alarm.carriageNumber = pCarriage->carriageNumber;
	alarm.modelId = 0;
	alarm.modelCode = MODELCODE_LVW;
	alarm.checkItemId = 0;
	alarm.checkItemCode = CHECKITEMCODE_LVW_DJWD;
	alarm.alarmPos1 = pWeelSet->pos;
	alarm.alarmPos2 = 0;
	alarm.alarmValue1 = pData->value;
	if (alarm.alarmValue1 > djwd_alarm)
	{
		alarm.alarmLevel = 3;
		pVectData->alarmNum++;
		pVectData->alarmDatas.push_back(alarm);
	}
	else if (alarm.alarmValue1 > djwd_warn)
	{
		alarm.alarmLevel = 2;
		pVectData->alarmNum++;
		pVectData->alarmDatas.push_back(alarm);
	}
	//no alarm no push
	return;
}
void zhzAlarmSet(struct VIBRATE_DATA *pVectData, struct CARRIAGE *pCarriage, struct WHEELSET *pWeelSet, struct CHECKDATA_VIBRATE *pData)
{
	struct ALARM_DATAS alarm;
	alarm.groupName = pVectData->groupName;
	alarm.alarmTime = pVectData->checkTime;
	alarm.carriagePos = pCarriage->carriagePos;
	alarm.carriageNumber = pCarriage->carriageNumber;
	alarm.modelId = 0;
	alarm.modelCode = MODELCODE_LVW;
	alarm.checkItemId = 0;
	alarm.checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
	alarm.alarmPos1 = pWeelSet->pos;
	alarm.alarmPos2 = pData->axisArount;
	alarm.alarmValue1 = pData->value;
	if (alarm.alarmValue1 > zhz_alarm)
	{
		alarm.alarmLevel = 3;
		pVectData->alarmNum++;
		pVectData->alarmDatas.push_back(alarm);
	}
	else if (alarm.alarmValue1 > zhz_warn)
	{
		alarm.alarmLevel = 2;
		pVectData->alarmNum++;
		pVectData->alarmDatas.push_back(alarm);
	}
	//no alarm no push
	return;
}
void zxwdAlarmSet(struct VIBRATE_DATA *pVectData, struct CARRIAGE *pCarriage, struct WHEELSET *pWeelSet, struct CHECKDATA_VIBRATE *pData)
{
	struct ALARM_DATAS alarm;
	alarm.groupName = pVectData->groupName;
	alarm.alarmTime = pVectData->checkTime;
	alarm.carriagePos = pCarriage->carriagePos;
	alarm.carriageNumber = pCarriage->carriageNumber;
	alarm.modelId = 0;
	alarm.modelCode = MODELCODE_LVW;
	alarm.checkItemId = 0;
	alarm.checkItemCode = CHECKITEMCODE_LVW_ZXWD;
	alarm.alarmPos1 = pWeelSet->pos;
	alarm.alarmPos2 = pData->axisArount;
	alarm.alarmValue1 = pData->value;
	if (alarm.alarmValue1 > zxwd_alarm)
	{
		alarm.alarmLevel = 3;
		pVectData->alarmNum++;
		pVectData->alarmDatas.push_back(alarm);
	}
	else if (alarm.alarmValue1 > zxwd_warn)
	{
		alarm.alarmLevel = 2;
		pVectData->alarmNum++;
		pVectData->alarmDatas.push_back(alarm);
	}
	//no alarm no push
	return;
}



BOOL IsTextUTF8(char* str, ULONGLONG length)
{
      DWORD nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�  
      UCHAR chr;
       BOOL bAllAscii = TRUE; //���ȫ������ASCII, ˵������UTF-8  
        for (int i = 0; i<length; ++i)
         {
             chr = *(str + i);
             if ((chr & 0x80) != 0) // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx  
                 bAllAscii = FALSE;
             if (nBytes == 0) //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���  
            {
                if (chr >= 0x80)
                 {
                     if (chr >= 0xFC && chr <= 0xFD)
                         nBytes = 6;
                     else if (chr >= 0xF8)
                         nBytes = 5;
                     else if (chr >= 0xF0)
                         nBytes = 4;
                     else if (chr >= 0xE0)
                         nBytes = 3;
                     else if (chr >= 0xC0)
                         nBytes = 2;
                    else
                         return FALSE;

                     nBytes--;
                 }
            }
            else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx  
             {
                if ((chr & 0xC0) != 0x80)
                   return FALSE;

                 nBytes--;
            }
         }
      if (nBytes > 0) //Υ������  
            return FALSE;
    if (bAllAscii) //���ȫ������ASCII, ˵������UTF-8  
          return FALSE;

      return TRUE;
 }
/***********************************/
#include <iostream>
#include <sstream>

#include <qnetworkrequest.h>
#include <qnetworkreply.h>
#include <qnetworkaccessmanager.h>
#include "network_logic.h"//http post json
#pragma comment (lib,"Qt5Network.lib")
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string data((const char*)ptr, (size_t)size * nmemb);

	*((std::stringstream*) stream) << data << endl;

	return size * nmemb;
}

void AlgorithmBase::setAlarmData(float motortemp_alarm, float motortemp_warn, float zdzhz_alarm,
	float zdzhz_warn, float zw_alarm, float zw_warn)
{
	djwd_alarm = motortemp_alarm;
	djwd_warn = motortemp_warn;
	zhz_alarm = zdzhz_alarm;
	zhz_warn = zdzhz_warn;
	zxwd_alarm = zw_alarm;
	zxwd_warn = zw_warn;
}
void AlgorithmBase::setVibData(struct VIBRATE_DATA *pData, vector<double> zdzhz_L, vector<double> zdzhz_R,
	vector<vector<double>> axleDataR, vector<vector<double>> axleDataL,
	vector<vector<double>> electricalDataR, vector<vector<double>> electricalDataL)
{
	//-----����(json�ж���)��c++�ж�Ӧ�������------
	//���ۺ�ֵ��zdzhz_R��zdzhz_L
	//�����¶ȣ�axleDataR[0],axleDataL[0] ----0�����ֵ��1�����ֵ������ֵ��json
	//����¶ȣ�electricalDataR[0], electricalDataL[0] ----0�����ֵ��1�����ֵ������ֵ��json
	//�����¶�(ambientTemperature)��m_dEnvironmentTemp
	//�ٶ�(speed)��m_dTrainSpeed
	//���ʱ��(checkTime)��m_strTrainRunNumber  ----�г�������ˮ������ʱ�������ģ�checkTime;�衰20201121125815��ת����"2022-05-21 18:35:47"��ʽ
	//��¼��(record_id)��m_strTrainRunNumber  ----�г�������ˮ������ʱ�������ģ�record_id:ֱ��ʹ��
	//���ضˣ�m_strMainControlPort.toInt() ---- 0��ż���ˣ�1�������ˣ��ò�������json��ֻ���ڳ���ŵĴ���
	//����(groupName)��m_strTrainNumber -----��ʽ��"005006"������json�ĸ�ʽΪ"09005006"
	//�����(carriageNumber)����������ض˺ͳ��Ž��д�����ʽ"09A005"
	//��·�ţ�m_strLineNumber

	pData->alarmNum = 0;
	QString record_idStr = m_strTrainRunNumber;
	QString m_strLineNumberStr = m_strLineNumber;
	QString m_strTrainNumberStr = m_strTrainNumber;

	//yxw add start
	QString subTrainNumberStr1 = m_strTrainNumberStr.mid(0, 3);
	QString subTrainNumberStr2 = m_strTrainNumberStr.mid(3, 3);
	if (m_strMainControlPort.toInt() % 2 == 1)
	{
		pData->carriages[0].carriageNumber = m_strLineNumberStr + "A" + subTrainNumberStr1;
		pData->carriages[1].carriageNumber = m_strLineNumberStr + "B" + subTrainNumberStr1;
		pData->carriages[2].carriageNumber = m_strLineNumberStr + "C" + subTrainNumberStr1;
		pData->carriages[3].carriageNumber = m_strLineNumberStr + "C" + subTrainNumberStr2;
		pData->carriages[4].carriageNumber = m_strLineNumberStr + "B" + subTrainNumberStr2;
		pData->carriages[5].carriageNumber = m_strLineNumberStr + "A" + subTrainNumberStr2;
	}
	else
	{
		pData->carriages[0].carriageNumber = m_strLineNumberStr + "A" + subTrainNumberStr2;
		pData->carriages[1].carriageNumber = m_strLineNumberStr + "B" + subTrainNumberStr2;
		pData->carriages[2].carriageNumber = m_strLineNumberStr + "C" + subTrainNumberStr2;
		pData->carriages[3].carriageNumber = m_strLineNumberStr + "C" + subTrainNumberStr1;
		pData->carriages[4].carriageNumber = m_strLineNumberStr + "B" + subTrainNumberStr1;
		pData->carriages[5].carriageNumber = m_strLineNumberStr + "A" + subTrainNumberStr1;
	}

	QString m_strCheckTimeStr = record_idStr.mid(0, 4) + "-" + record_idStr.mid(4, 2) + "-" + record_idStr.mid(6, 2) + " " + record_idStr.mid(8, 2) + ":" + record_idStr.mid(10, 2) + ":" + record_idStr.mid(12, 2);
	pData->checkTime = m_strCheckTimeStr;
	//yxw add end


	pData->record_id = record_idStr;
	pData->groupName = "GZDT" + m_strLineNumberStr + "-" + m_strTrainNumberStr;

	pData->passDw = 0;
	pData->checkDir = 0;
	pData->jcpCode = JCPCODE_FELZDZW;
	pData->speed = m_dTrainSpeed;
	pData->ambientTemperature = m_dEnvironmentTemp;
	pData->carriageNum = 6;

	pData->carriages[0].carriagePos = 1;
	pData->carriages[0].wheelSetCount = 4;
	//0 for (int i = 0; i < 4; i++)
	{
		pData->carriages[0].wheelSet[0].pos = 1;
		pData->carriages[0].wheelSet[0].checkDataNum = 4;
		pData->carriages[0].wheelSet[0].checkData[0].img = "";
		pData->carriages[0].wheelSet[0].checkData[0].state = 0;
		pData->carriages[0].wheelSet[0].checkData[0].axisArount = 0;
		pData->carriages[0].wheelSet[0].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
		pData->carriages[0].wheelSet[0].checkData[0].value = zdzhz_R[0];
		pData->carriages[0].wheelSet[0].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
		zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[0], &pData->carriages[0].wheelSet[0].checkData[0]);
		pData->carriages[0].wheelSet[0].checkData[1].img = "";
		pData->carriages[0].wheelSet[0].checkData[1].state = 0;
		pData->carriages[0].wheelSet[0].checkData[1].axisArount = 1;
		pData->carriages[0].wheelSet[0].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
		pData->carriages[0].wheelSet[0].checkData[1].value = zdzhz_L[0];
		pData->carriages[0].wheelSet[0].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
		zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[0], &pData->carriages[0].wheelSet[0].checkData[1]);
		pData->carriages[0].wheelSet[0].checkData[2].img = "";
		pData->carriages[0].wheelSet[0].checkData[2].state = 0;
		pData->carriages[0].wheelSet[0].checkData[2].axisArount = 0;
		pData->carriages[0].wheelSet[0].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
		pData->carriages[0].wheelSet[0].checkData[2].value = axleDataR[0][0];
		pData->carriages[0].wheelSet[0].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
		zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[0], &pData->carriages[0].wheelSet[0].checkData[2]);
		pData->carriages[0].wheelSet[0].checkData[3].img = "";
		pData->carriages[0].wheelSet[0].checkData[3].state = 0;
		pData->carriages[0].wheelSet[0].checkData[3].axisArount = 1;
		pData->carriages[0].wheelSet[0].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
		pData->carriages[0].wheelSet[0].checkData[3].value = axleDataL[0][0];
		pData->carriages[0].wheelSet[0].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
		zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[0], &pData->carriages[0].wheelSet[0].checkData[3]);

	}
	//1 for (int i = 0; i < 4; i++)
			{
				pData->carriages[0].wheelSet[1].pos = 2;
				pData->carriages[0].wheelSet[1].checkDataNum = 4;
				pData->carriages[0].wheelSet[1].checkData[0].img = "";
				pData->carriages[0].wheelSet[1].checkData[0].state = 0;
				pData->carriages[0].wheelSet[1].checkData[0].axisArount = 0;
				pData->carriages[0].wheelSet[1].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[0].wheelSet[1].checkData[0].value = zdzhz_R[1];
				pData->carriages[0].wheelSet[1].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[1], &pData->carriages[0].wheelSet[1].checkData[0]);
				pData->carriages[0].wheelSet[1].checkData[1].img = "";
				pData->carriages[0].wheelSet[1].checkData[1].state = 0;
				pData->carriages[0].wheelSet[1].checkData[1].axisArount = 1;
				pData->carriages[0].wheelSet[1].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[0].wheelSet[1].checkData[1].value = zdzhz_L[1];
				pData->carriages[0].wheelSet[1].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[1], &pData->carriages[0].wheelSet[1].checkData[1]);
				pData->carriages[0].wheelSet[1].checkData[2].img = "";
				pData->carriages[0].wheelSet[1].checkData[2].state = 0;
				pData->carriages[0].wheelSet[1].checkData[2].axisArount = 0;
				pData->carriages[0].wheelSet[1].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[0].wheelSet[1].checkData[2].value = axleDataR[0][1];
				pData->carriages[0].wheelSet[1].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[1], &pData->carriages[0].wheelSet[1].checkData[2]);
				pData->carriages[0].wheelSet[1].checkData[3].img = "";
				pData->carriages[0].wheelSet[1].checkData[3].state = 0;
				pData->carriages[0].wheelSet[1].checkData[3].axisArount = 1;
				pData->carriages[0].wheelSet[1].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[0].wheelSet[1].checkData[3].value = axleDataL[0][1];
				pData->carriages[0].wheelSet[1].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[1], &pData->carriages[0].wheelSet[1].checkData[3]);
			}
			//2 for (int i = 0; i < 4; i++)
			{
				pData->carriages[0].wheelSet[2].pos = 3;
				pData->carriages[0].wheelSet[2].checkDataNum = 4;
				pData->carriages[0].wheelSet[2].checkData[0].img = "";
				pData->carriages[0].wheelSet[2].checkData[0].state = 0;
				pData->carriages[0].wheelSet[2].checkData[0].axisArount = 0;
				pData->carriages[0].wheelSet[2].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[0].wheelSet[2].checkData[0].value = zdzhz_R[2];
				pData->carriages[0].wheelSet[2].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[2], &pData->carriages[0].wheelSet[2].checkData[0]);
				pData->carriages[0].wheelSet[2].checkData[1].img = "";
				pData->carriages[0].wheelSet[2].checkData[1].state = 0;
				pData->carriages[0].wheelSet[2].checkData[1].axisArount = 1;
				pData->carriages[0].wheelSet[2].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[0].wheelSet[2].checkData[1].value = zdzhz_L[2];  //yxw modify
				pData->carriages[0].wheelSet[2].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[2], &pData->carriages[0].wheelSet[2].checkData[1]);
				pData->carriages[0].wheelSet[2].checkData[2].img = "";
				pData->carriages[0].wheelSet[2].checkData[2].state = 0;
				pData->carriages[0].wheelSet[2].checkData[2].axisArount = 0;
				pData->carriages[0].wheelSet[2].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[0].wheelSet[2].checkData[2].value = axleDataR[0][2];
				pData->carriages[0].wheelSet[2].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[2], &pData->carriages[0].wheelSet[2].checkData[2]);
				pData->carriages[0].wheelSet[2].checkData[3].img = "";
				pData->carriages[0].wheelSet[2].checkData[3].state = 0;
				pData->carriages[0].wheelSet[2].checkData[3].axisArount = 1;
				pData->carriages[0].wheelSet[2].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[0].wheelSet[2].checkData[3].value = axleDataL[0][2];
				pData->carriages[0].wheelSet[2].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[2], &pData->carriages[0].wheelSet[2].checkData[3]);
			}
			//3 for (int i = 0; i < 4; i++)
			{
				pData->carriages[0].wheelSet[3].pos = 4;
				pData->carriages[0].wheelSet[3].checkDataNum = 4;
				pData->carriages[0].wheelSet[3].checkData[0].img = "";
				pData->carriages[0].wheelSet[3].checkData[0].state = 0;
				pData->carriages[0].wheelSet[3].checkData[0].axisArount = 0;
				pData->carriages[0].wheelSet[3].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[0].wheelSet[3].checkData[0].value = zdzhz_R[3];
				pData->carriages[0].wheelSet[3].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[3], &pData->carriages[0].wheelSet[3].checkData[0]);
				pData->carriages[0].wheelSet[3].checkData[1].img = "";
				pData->carriages[0].wheelSet[3].checkData[1].state = 0;
				pData->carriages[0].wheelSet[3].checkData[1].axisArount = 1;
				pData->carriages[0].wheelSet[3].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[0].wheelSet[3].checkData[1].value = zdzhz_L[3];
				pData->carriages[0].wheelSet[3].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[3], &pData->carriages[0].wheelSet[3].checkData[1]);
				pData->carriages[0].wheelSet[3].checkData[2].img = "";
				pData->carriages[0].wheelSet[3].checkData[2].state = 0;
				pData->carriages[0].wheelSet[3].checkData[2].axisArount = 0;
				pData->carriages[0].wheelSet[3].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[0].wheelSet[3].checkData[2].value = axleDataR[0][3];
				pData->carriages[0].wheelSet[3].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[3], &pData->carriages[0].wheelSet[3].checkData[2]);
				pData->carriages[0].wheelSet[3].checkData[3].img = "";
				pData->carriages[0].wheelSet[3].checkData[3].state = 0;
				pData->carriages[0].wheelSet[3].checkData[3].axisArount = 1;
				pData->carriages[0].wheelSet[3].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[0].wheelSet[3].checkData[3].value = axleDataL[0][3];
				pData->carriages[0].wheelSet[3].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[0], &pData->carriages[0].wheelSet[3], &pData->carriages[0].wheelSet[3].checkData[3]);
			}

			pData->carriages[1].carriagePos = 2;
			pData->carriages[1].wheelSetCount = 4;
			//0 for (int i = 0; i < 4; i++)
			{
				pData->carriages[1].wheelSet[0].pos = 1;
				pData->carriages[1].wheelSet[0].checkDataNum = 5;
				pData->carriages[1].wheelSet[0].checkData[0].img = "";
				pData->carriages[1].wheelSet[0].checkData[0].state = 0;
				pData->carriages[1].wheelSet[0].checkData[0].axisArount = 0;
				pData->carriages[1].wheelSet[0].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[0].checkData[0].value = zdzhz_R[4];
				pData->carriages[1].wheelSet[0].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[0], &pData->carriages[1].wheelSet[0].checkData[0]);
				pData->carriages[1].wheelSet[0].checkData[1].img = "";
				pData->carriages[1].wheelSet[0].checkData[1].state = 0;
				pData->carriages[1].wheelSet[0].checkData[1].axisArount = 1;
				pData->carriages[1].wheelSet[0].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[0].checkData[1].value = zdzhz_L[4];
				pData->carriages[1].wheelSet[0].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[0], &pData->carriages[1].wheelSet[0].checkData[1]);
				pData->carriages[1].wheelSet[0].checkData[2].img = "";
				pData->carriages[1].wheelSet[0].checkData[2].state = 0;
				pData->carriages[1].wheelSet[0].checkData[2].axisArount = 0;
				pData->carriages[1].wheelSet[0].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[0].checkData[2].value = axleDataR[0][4];
				pData->carriages[1].wheelSet[0].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[0], &pData->carriages[1].wheelSet[0].checkData[2]);
				pData->carriages[1].wheelSet[0].checkData[3].img = "";
				pData->carriages[1].wheelSet[0].checkData[3].state = 0;
				pData->carriages[1].wheelSet[0].checkData[3].axisArount = 1;
				pData->carriages[1].wheelSet[0].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[0].checkData[3].value = axleDataL[0][4];
				pData->carriages[1].wheelSet[0].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[0], &pData->carriages[1].wheelSet[0].checkData[3]);
				pData->carriages[1].wheelSet[0].checkData[4].img = "";
				pData->carriages[1].wheelSet[0].checkData[4].state = 0;
				pData->carriages[1].wheelSet[0].checkData[4].axisArount = 0;
				pData->carriages[1].wheelSet[0].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[1].wheelSet[0].checkData[4].value = electricalDataR[0][0];
				pData->carriages[1].wheelSet[0].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[0], &pData->carriages[1].wheelSet[0].checkData[4]);
			}
			//1 for (int i = 0; i < 4; i++)
			{
				pData->carriages[1].wheelSet[1].pos = 2;
				pData->carriages[1].wheelSet[1].checkDataNum = 5;
				pData->carriages[1].wheelSet[1].checkData[0].img = "";
				pData->carriages[1].wheelSet[1].checkData[0].state = 0;
				pData->carriages[1].wheelSet[1].checkData[0].axisArount = 0;
				pData->carriages[1].wheelSet[1].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[1].checkData[0].value = zdzhz_R[5];
				pData->carriages[1].wheelSet[1].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[1], &pData->carriages[1].wheelSet[1].checkData[0]);
				pData->carriages[1].wheelSet[1].checkData[1].img = "";
				pData->carriages[1].wheelSet[1].checkData[1].state = 0;
				pData->carriages[1].wheelSet[1].checkData[1].axisArount = 1;
				pData->carriages[1].wheelSet[1].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[1].checkData[1].value = zdzhz_L[5];
				pData->carriages[1].wheelSet[1].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[1], &pData->carriages[1].wheelSet[1].checkData[1]);
				pData->carriages[1].wheelSet[1].checkData[2].img = "";
				pData->carriages[1].wheelSet[1].checkData[2].state = 0;
				pData->carriages[1].wheelSet[1].checkData[2].axisArount = 0;
				pData->carriages[1].wheelSet[1].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[1].checkData[2].value = axleDataR[0][5];
				pData->carriages[1].wheelSet[1].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[1], &pData->carriages[1].wheelSet[1].checkData[2]);
				pData->carriages[1].wheelSet[1].checkData[3].img = "";
				pData->carriages[1].wheelSet[1].checkData[3].state = 0;
				pData->carriages[1].wheelSet[1].checkData[3].axisArount = 1;
				pData->carriages[1].wheelSet[1].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[1].checkData[3].value = axleDataL[0][5];
				pData->carriages[1].wheelSet[1].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[1], &pData->carriages[1].wheelSet[1].checkData[3]);
				pData->carriages[1].wheelSet[1].checkData[4].img = "";
				pData->carriages[1].wheelSet[1].checkData[4].state = 0;
				pData->carriages[1].wheelSet[1].checkData[4].axisArount = 0;
				pData->carriages[1].wheelSet[1].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[1].wheelSet[1].checkData[4].value = electricalDataL[0][0];
				pData->carriages[1].wheelSet[1].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[1], &pData->carriages[1].wheelSet[1].checkData[4]);

			}
			//2 for (int i = 0; i < 4; i++)
			{
				pData->carriages[1].wheelSet[2].pos = 3;
				pData->carriages[1].wheelSet[2].checkDataNum = 5;
				pData->carriages[1].wheelSet[2].checkData[0].img = "";
				pData->carriages[1].wheelSet[2].checkData[0].state = 0;
				pData->carriages[1].wheelSet[2].checkData[0].axisArount = 0;
				pData->carriages[1].wheelSet[2].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[2].checkData[0].value = zdzhz_R[6];
				pData->carriages[1].wheelSet[2].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[2], &pData->carriages[1].wheelSet[2].checkData[0]);
				pData->carriages[1].wheelSet[2].checkData[1].img = "";
				pData->carriages[1].wheelSet[2].checkData[1].state = 0;
				pData->carriages[1].wheelSet[2].checkData[1].axisArount = 1;
				pData->carriages[1].wheelSet[2].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[2].checkData[1].value = zdzhz_L[6];
				pData->carriages[1].wheelSet[2].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[2], &pData->carriages[1].wheelSet[2].checkData[1]);
				pData->carriages[1].wheelSet[2].checkData[2].img = "";
				pData->carriages[1].wheelSet[2].checkData[2].state = 0;
				pData->carriages[1].wheelSet[2].checkData[2].axisArount = 0;
				pData->carriages[1].wheelSet[2].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[2].checkData[2].value = axleDataR[0][6];
				pData->carriages[1].wheelSet[2].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[2], &pData->carriages[1].wheelSet[2].checkData[2]);
				pData->carriages[1].wheelSet[2].checkData[3].img = "";
				pData->carriages[1].wheelSet[2].checkData[3].state = 0;
				pData->carriages[1].wheelSet[2].checkData[3].axisArount = 1;
				pData->carriages[1].wheelSet[2].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[2].checkData[3].value = axleDataL[0][6];
				pData->carriages[1].wheelSet[2].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[2], &pData->carriages[1].wheelSet[2].checkData[3]);
				pData->carriages[1].wheelSet[2].checkData[4].img = "";
				pData->carriages[1].wheelSet[2].checkData[4].state = 0;
				pData->carriages[1].wheelSet[2].checkData[4].axisArount = 0;
				pData->carriages[1].wheelSet[2].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[1].wheelSet[2].checkData[4].value = electricalDataR[0][1];
				pData->carriages[1].wheelSet[2].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[2], &pData->carriages[1].wheelSet[2].checkData[4]);
			}
			//3 for (int i = 0; i < 4; i++)
			{
				pData->carriages[1].wheelSet[3].pos = 4;
				pData->carriages[1].wheelSet[3].checkDataNum = 5;
				pData->carriages[1].wheelSet[3].checkData[0].img = "";
				pData->carriages[1].wheelSet[3].checkData[0].state = 0;
				pData->carriages[1].wheelSet[3].checkData[0].axisArount = 0;
				pData->carriages[1].wheelSet[3].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[3].checkData[0].value = zdzhz_R[7];
				pData->carriages[1].wheelSet[3].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[3], &pData->carriages[1].wheelSet[3].checkData[0]);  //yxw modify,������������Ϊ&pData->carriages[1].wheelSet[3]
				pData->carriages[1].wheelSet[3].checkData[1].img = "";
				pData->carriages[1].wheelSet[3].checkData[1].state = 0;
				pData->carriages[1].wheelSet[3].checkData[1].axisArount = 1;
				pData->carriages[1].wheelSet[3].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[1].wheelSet[3].checkData[1].value = zdzhz_L[7];
				pData->carriages[1].wheelSet[3].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[3], &pData->carriages[1].wheelSet[3].checkData[1]);//yxw modify,������������Ϊ&pData->carriages[1].wheelSet[3]
				pData->carriages[1].wheelSet[3].checkData[2].img = "";
				pData->carriages[1].wheelSet[3].checkData[2].state = 0;
				pData->carriages[1].wheelSet[3].checkData[2].axisArount = 0;
				pData->carriages[1].wheelSet[3].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[3].checkData[2].value = axleDataR[0][7];
				pData->carriages[1].wheelSet[3].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[3], &pData->carriages[1].wheelSet[3].checkData[2]);//yxw modify,������������Ϊ&pData->carriages[1].wheelSet[3]
				pData->carriages[1].wheelSet[3].checkData[3].img = "";
				pData->carriages[1].wheelSet[3].checkData[3].state = 0;
				pData->carriages[1].wheelSet[3].checkData[3].axisArount = 1;
				pData->carriages[1].wheelSet[3].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[1].wheelSet[3].checkData[3].value = axleDataL[0][7];
				pData->carriages[1].wheelSet[3].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[3], &pData->carriages[1].wheelSet[3].checkData[3]);//yxw modify,������������Ϊ&pData->carriages[1].wheelSet[3]
				pData->carriages[1].wheelSet[3].checkData[4].img = "";
				pData->carriages[1].wheelSet[3].checkData[4].state = 0;
				pData->carriages[1].wheelSet[3].checkData[4].axisArount = 0;
				pData->carriages[1].wheelSet[3].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[1].wheelSet[3].checkData[4].value = electricalDataL[0][1];
				pData->carriages[1].wheelSet[3].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[1], &pData->carriages[1].wheelSet[3], &pData->carriages[1].wheelSet[3].checkData[4]);
			}

			pData->carriages[2].carriagePos = 3;
			pData->carriages[2].wheelSetCount = 4;
			//0 for (int i = 0; i < 4; i++)
			{
				pData->carriages[2].wheelSet[0].pos = 1;
				pData->carriages[2].wheelSet[0].checkDataNum = 5;
				pData->carriages[2].wheelSet[0].checkData[0].img = "";
				pData->carriages[2].wheelSet[0].checkData[0].state = 0;
				pData->carriages[2].wheelSet[0].checkData[0].axisArount = 0;
				pData->carriages[2].wheelSet[0].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[0].checkData[0].value = zdzhz_R[8];
				pData->carriages[2].wheelSet[0].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[0], &pData->carriages[2].wheelSet[0].checkData[0]);
				pData->carriages[2].wheelSet[0].checkData[1].img = "";
				pData->carriages[2].wheelSet[0].checkData[1].state = 0;
				pData->carriages[2].wheelSet[0].checkData[1].axisArount = 1;
				pData->carriages[2].wheelSet[0].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[0].checkData[1].value = zdzhz_L[8];
				pData->carriages[2].wheelSet[0].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[0], &pData->carriages[2].wheelSet[0].checkData[1]);
				pData->carriages[2].wheelSet[0].checkData[2].img = "";
				pData->carriages[2].wheelSet[0].checkData[2].state = 0;
				pData->carriages[2].wheelSet[0].checkData[2].axisArount = 0;
				pData->carriages[2].wheelSet[0].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[0].checkData[2].value = axleDataR[0][8];
				pData->carriages[2].wheelSet[0].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[0], &pData->carriages[2].wheelSet[0].checkData[2]);
				pData->carriages[2].wheelSet[0].checkData[3].img = "";
				pData->carriages[2].wheelSet[0].checkData[3].state = 0;
				pData->carriages[2].wheelSet[0].checkData[3].axisArount = 1;
				pData->carriages[2].wheelSet[0].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[0].checkData[3].value = axleDataL[0][8];
				pData->carriages[2].wheelSet[0].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[0], &pData->carriages[2].wheelSet[0].checkData[3]);
				pData->carriages[2].wheelSet[0].checkData[4].img = "";
				pData->carriages[2].wheelSet[0].checkData[4].state = 0;
				pData->carriages[2].wheelSet[0].checkData[4].axisArount = 0;
				pData->carriages[2].wheelSet[0].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[2].wheelSet[0].checkData[4].value = electricalDataR[0][2];
				pData->carriages[2].wheelSet[0].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[0], &pData->carriages[2].wheelSet[0].checkData[4]);
			}
			//1 for (int i = 0; i < 4; i++)
			{
				pData->carriages[2].wheelSet[1].pos = 2;
				pData->carriages[2].wheelSet[1].checkDataNum = 5;
				pData->carriages[2].wheelSet[1].checkData[0].img = "";
				pData->carriages[2].wheelSet[1].checkData[0].state = 0;
				pData->carriages[2].wheelSet[1].checkData[0].axisArount = 0;
				pData->carriages[2].wheelSet[1].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[1].checkData[0].value = zdzhz_R[9];
				pData->carriages[2].wheelSet[1].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[1], &pData->carriages[2].wheelSet[1].checkData[0]);
				pData->carriages[2].wheelSet[1].checkData[1].img = "";
				pData->carriages[2].wheelSet[1].checkData[1].state = 0;
				pData->carriages[2].wheelSet[1].checkData[1].axisArount = 1;
				pData->carriages[2].wheelSet[1].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[1].checkData[1].value = zdzhz_L[9];
				pData->carriages[2].wheelSet[1].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[1], &pData->carriages[2].wheelSet[1].checkData[1]);
				pData->carriages[2].wheelSet[1].checkData[2].img = "";
				pData->carriages[2].wheelSet[1].checkData[2].state = 0;
				pData->carriages[2].wheelSet[1].checkData[2].axisArount = 0;
				pData->carriages[2].wheelSet[1].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[1].checkData[2].value = axleDataR[0][9];
				pData->carriages[2].wheelSet[1].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[1], &pData->carriages[2].wheelSet[1].checkData[2]);
				pData->carriages[2].wheelSet[1].checkData[3].img = "";
				pData->carriages[2].wheelSet[1].checkData[3].state = 0;
				pData->carriages[2].wheelSet[1].checkData[3].axisArount = 1;
				pData->carriages[2].wheelSet[1].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[1].checkData[3].value = axleDataL[0][9];
				pData->carriages[2].wheelSet[1].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[1], &pData->carriages[2].wheelSet[1].checkData[3]);
				pData->carriages[2].wheelSet[1].checkData[4].img = "";
				pData->carriages[2].wheelSet[1].checkData[4].state = 0;
				pData->carriages[2].wheelSet[1].checkData[4].axisArount = 0;
				pData->carriages[2].wheelSet[1].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[2].wheelSet[1].checkData[4].value = electricalDataL[0][2];
				pData->carriages[2].wheelSet[1].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[1], &pData->carriages[2].wheelSet[1].checkData[4]);
			}
			//2 for (int i = 0; i < 4; i++)
			{
				pData->carriages[2].wheelSet[2].pos = 3;  //yxw modify, 4 -> 3
				pData->carriages[2].wheelSet[2].checkDataNum = 5;
				pData->carriages[2].wheelSet[2].checkData[0].img = "";
				pData->carriages[2].wheelSet[2].checkData[0].state = 0;
				pData->carriages[2].wheelSet[2].checkData[0].axisArount = 0;
				pData->carriages[2].wheelSet[2].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[2].checkData[0].value = zdzhz_R[10];
				pData->carriages[2].wheelSet[2].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[2], &pData->carriages[2].wheelSet[2].checkData[0]);
				pData->carriages[2].wheelSet[2].checkData[1].img = "";
				pData->carriages[2].wheelSet[2].checkData[1].state = 0;
				pData->carriages[2].wheelSet[2].checkData[1].axisArount = 1;
				pData->carriages[2].wheelSet[2].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[2].checkData[1].value = zdzhz_L[10];
				pData->carriages[2].wheelSet[2].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[2], &pData->carriages[2].wheelSet[2].checkData[1]);
				pData->carriages[2].wheelSet[2].checkData[2].img = "";
				pData->carriages[2].wheelSet[2].checkData[2].state = 0;
				pData->carriages[2].wheelSet[2].checkData[2].axisArount = 0;
				pData->carriages[2].wheelSet[2].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[2].checkData[2].value = axleDataR[0][10];
				pData->carriages[2].wheelSet[2].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[2], &pData->carriages[2].wheelSet[2].checkData[2]);
				pData->carriages[2].wheelSet[2].checkData[3].img = "";
				pData->carriages[2].wheelSet[2].checkData[3].state = 0;
				pData->carriages[2].wheelSet[2].checkData[3].axisArount = 1;
				pData->carriages[2].wheelSet[2].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[2].checkData[3].value = axleDataL[0][10];
				pData->carriages[2].wheelSet[2].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[2], &pData->carriages[2].wheelSet[2].checkData[3]);
				pData->carriages[2].wheelSet[2].checkData[4].img = "";
				pData->carriages[2].wheelSet[2].checkData[4].state = 0;
				pData->carriages[2].wheelSet[2].checkData[4].axisArount = 0;
				pData->carriages[2].wheelSet[2].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[2].wheelSet[2].checkData[4].value = electricalDataR[0][3];
				pData->carriages[2].wheelSet[2].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[2], &pData->carriages[2].wheelSet[2].checkData[4]);
			}
			//3 for (int i = 0; i < 4; i++)
			{
				pData->carriages[2].wheelSet[3].pos = 4;
				pData->carriages[2].wheelSet[3].checkDataNum = 5;
				pData->carriages[2].wheelSet[3].checkData[0].img = "";
				pData->carriages[2].wheelSet[3].checkData[0].state = 0;
				pData->carriages[2].wheelSet[3].checkData[0].axisArount = 0;
				pData->carriages[2].wheelSet[3].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[3].checkData[0].value = zdzhz_R[11];
				pData->carriages[2].wheelSet[3].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[3], &pData->carriages[2].wheelSet[3].checkData[0]);
				pData->carriages[2].wheelSet[3].checkData[1].img = "";
				pData->carriages[2].wheelSet[3].checkData[1].state = 0;
				pData->carriages[2].wheelSet[3].checkData[1].axisArount = 1;
				pData->carriages[2].wheelSet[3].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[2].wheelSet[3].checkData[1].value = zdzhz_L[11];
				pData->carriages[2].wheelSet[3].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[3], &pData->carriages[2].wheelSet[3].checkData[1]);
				pData->carriages[2].wheelSet[3].checkData[2].img = "";
				pData->carriages[2].wheelSet[3].checkData[2].state = 0;
				pData->carriages[2].wheelSet[3].checkData[2].axisArount = 0;
				pData->carriages[2].wheelSet[3].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[3].checkData[2].value = axleDataR[0][11];
				pData->carriages[2].wheelSet[3].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[3], &pData->carriages[2].wheelSet[3].checkData[2]);
				pData->carriages[2].wheelSet[3].checkData[3].img = "";
				pData->carriages[2].wheelSet[3].checkData[3].state = 0;
				pData->carriages[2].wheelSet[3].checkData[3].axisArount = 1;
				pData->carriages[2].wheelSet[3].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[2].wheelSet[3].checkData[3].value = axleDataL[0][11];
				pData->carriages[2].wheelSet[3].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[3], &pData->carriages[2].wheelSet[3].checkData[3]);
				pData->carriages[2].wheelSet[3].checkData[4].img = "";
				pData->carriages[2].wheelSet[3].checkData[4].state = 0;
				pData->carriages[2].wheelSet[3].checkData[4].axisArount = 0;
				pData->carriages[2].wheelSet[3].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[2].wheelSet[3].checkData[4].value = electricalDataL[0][3];
				pData->carriages[2].wheelSet[3].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[2], &pData->carriages[2].wheelSet[3], &pData->carriages[2].wheelSet[3].checkData[4]);
			}


			pData->carriages[3].carriagePos = 4;
			pData->carriages[3].wheelSetCount = 4;
			//3 for (int i = 0; i < 4; i++)
			{
				pData->carriages[3].wheelSet[3].pos = 4;
				pData->carriages[3].wheelSet[3].checkDataNum = 5;
				pData->carriages[3].wheelSet[3].checkData[0].img = "";
				pData->carriages[3].wheelSet[3].checkData[0].state = 0;
				pData->carriages[3].wheelSet[3].checkData[0].axisArount = 1;
				pData->carriages[3].wheelSet[3].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[3].wheelSet[3].checkData[0].value = zdzhz_R[12];
				pData->carriages[3].wheelSet[3].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[3], &pData->carriages[3].wheelSet[3].checkData[0]); // yxw modify,�����������ĸ�������ΪwheelSet[3]
				pData->carriages[3].wheelSet[3].checkData[1].img = "";
				pData->carriages[3].wheelSet[3].checkData[1].state = 0;
				pData->carriages[3].wheelSet[3].checkData[1].axisArount = 0;
				pData->carriages[3].wheelSet[3].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[3].wheelSet[3].checkData[1].value = zdzhz_L[12];
				pData->carriages[3].wheelSet[3].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[3], &pData->carriages[3].wheelSet[3].checkData[1]);// yxw modify,�����������ĸ�������ΪwheelSet[3]
				pData->carriages[3].wheelSet[3].checkData[2].img = "";
				pData->carriages[3].wheelSet[3].checkData[2].state = 0;
				pData->carriages[3].wheelSet[3].checkData[2].axisArount = 1;
				pData->carriages[3].wheelSet[3].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[3].wheelSet[3].checkData[2].value = axleDataR[0][12];
				pData->carriages[3].wheelSet[3].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[3], &pData->carriages[3].wheelSet[3].checkData[2]);
				pData->carriages[3].wheelSet[3].checkData[3].img = "";
				pData->carriages[3].wheelSet[3].checkData[3].state = 0;
				pData->carriages[3].wheelSet[3].checkData[3].axisArount = 0;
				pData->carriages[3].wheelSet[3].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[3].wheelSet[3].checkData[3].value = axleDataL[0][12];
				pData->carriages[3].wheelSet[3].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[3], &pData->carriages[3].wheelSet[3].checkData[3]);
				pData->carriages[3].wheelSet[3].checkData[4].img = "";
				pData->carriages[3].wheelSet[3].checkData[4].state = 0;
				pData->carriages[3].wheelSet[3].checkData[4].axisArount = 0;
				pData->carriages[3].wheelSet[3].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[3].wheelSet[3].checkData[4].value = electricalDataR[0][4];  //yxw modify, �������ڳ���ͬ���޸�
				pData->carriages[3].wheelSet[3].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[3], &pData->carriages[3].wheelSet[3].checkData[4]);
			}
			//2for (int i = 0; i < 4; i++)
			{
				pData->carriages[3].wheelSet[2].pos = 3;
				pData->carriages[3].wheelSet[2].checkDataNum = 5;
				pData->carriages[3].wheelSet[2].checkData[0].img = "";
				pData->carriages[3].wheelSet[2].checkData[0].state = 0;
				pData->carriages[3].wheelSet[2].checkData[0].axisArount = 1;
				pData->carriages[3].wheelSet[2].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[3].wheelSet[2].checkData[0].value = zdzhz_R[13];
				pData->carriages[3].wheelSet[2].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[2], &pData->carriages[3].wheelSet[2].checkData[0]);
				pData->carriages[3].wheelSet[2].checkData[1].img = "";
				pData->carriages[3].wheelSet[2].checkData[1].state = 0;
				pData->carriages[3].wheelSet[2].checkData[1].axisArount = 0;
				pData->carriages[3].wheelSet[2].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[3].wheelSet[2].checkData[1].value = zdzhz_L[13];
				pData->carriages[3].wheelSet[2].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[2], &pData->carriages[3].wheelSet[2].checkData[1]);
				pData->carriages[3].wheelSet[2].checkData[2].img = "";
				pData->carriages[3].wheelSet[2].checkData[2].state = 0;
				pData->carriages[3].wheelSet[2].checkData[2].axisArount = 1;
				pData->carriages[3].wheelSet[2].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[3].wheelSet[2].checkData[2].value = axleDataR[0][13];
				pData->carriages[3].wheelSet[2].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[2], &pData->carriages[3].wheelSet[2].checkData[2]);
				pData->carriages[3].wheelSet[2].checkData[3].img = "";
				pData->carriages[3].wheelSet[2].checkData[3].state = 0;
				pData->carriages[3].wheelSet[2].checkData[3].axisArount = 0;
				pData->carriages[3].wheelSet[2].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[3].wheelSet[2].checkData[3].value = axleDataL[0][13];
				pData->carriages[3].wheelSet[2].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[2], &pData->carriages[3].wheelSet[2].checkData[3]);
				pData->carriages[3].wheelSet[2].checkData[4].img = "";
				pData->carriages[3].wheelSet[2].checkData[4].state = 0;
				pData->carriages[3].wheelSet[2].checkData[4].axisArount = 0;
				pData->carriages[3].wheelSet[2].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[3].wheelSet[2].checkData[4].value = electricalDataL[0][4];  //yxw modify, �������ڳ���ͬ���޸�
				pData->carriages[3].wheelSet[2].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[2], &pData->carriages[3].wheelSet[2].checkData[4]);
			}
			//1 for (int i = 0; i < 4; i++)
			{
				pData->carriages[3].wheelSet[1].pos = 2;
				pData->carriages[3].wheelSet[1].checkDataNum = 5;
				pData->carriages[3].wheelSet[1].checkData[0].img = "";
				pData->carriages[3].wheelSet[1].checkData[0].state = 0;
				pData->carriages[3].wheelSet[1].checkData[0].axisArount = 1;
				pData->carriages[3].wheelSet[1].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[3].wheelSet[1].checkData[0].value = zdzhz_R[14];
				pData->carriages[3].wheelSet[1].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[1], &pData->carriages[3].wheelSet[1].checkData[0]);
				pData->carriages[3].wheelSet[1].checkData[1].img = "";
				pData->carriages[3].wheelSet[1].checkData[1].state = 0;
				pData->carriages[3].wheelSet[1].checkData[1].axisArount = 0;
				pData->carriages[3].wheelSet[1].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[3].wheelSet[1].checkData[1].value = zdzhz_L[14];
				pData->carriages[3].wheelSet[1].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[1], &pData->carriages[3].wheelSet[1].checkData[1]);
				pData->carriages[3].wheelSet[1].checkData[2].img = "";
				pData->carriages[3].wheelSet[1].checkData[2].state = 0;
				pData->carriages[3].wheelSet[1].checkData[2].axisArount = 1;
				pData->carriages[3].wheelSet[1].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[3].wheelSet[1].checkData[2].value = axleDataR[0][14];
				pData->carriages[3].wheelSet[1].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[1], &pData->carriages[3].wheelSet[1].checkData[2]);
				pData->carriages[3].wheelSet[1].checkData[3].img = "";
				pData->carriages[3].wheelSet[1].checkData[3].state = 0;
				pData->carriages[3].wheelSet[1].checkData[3].axisArount = 0;
				pData->carriages[3].wheelSet[1].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[3].wheelSet[1].checkData[3].value = axleDataL[0][14];
				pData->carriages[3].wheelSet[1].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[1], &pData->carriages[3].wheelSet[1].checkData[3]);
				pData->carriages[3].wheelSet[1].checkData[4].img = "";
				pData->carriages[3].wheelSet[1].checkData[4].state = 0;
				pData->carriages[3].wheelSet[1].checkData[4].axisArount = 0;
				pData->carriages[3].wheelSet[1].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[3].wheelSet[1].checkData[4].value = electricalDataR[0][5];
				pData->carriages[3].wheelSet[1].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[1], &pData->carriages[3].wheelSet[1].checkData[4]);
			}

			//0 for (int i = 0; i < 4; i++)
				{
					pData->carriages[3].wheelSet[0].pos = 1;
					pData->carriages[3].wheelSet[0].checkDataNum = 5;
					pData->carriages[3].wheelSet[0].checkData[0].img = "";
					pData->carriages[3].wheelSet[0].checkData[0].state = 0;
					pData->carriages[3].wheelSet[0].checkData[0].axisArount = 1;
					pData->carriages[3].wheelSet[0].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
					pData->carriages[3].wheelSet[0].checkData[0].value = zdzhz_R[15];
					pData->carriages[3].wheelSet[0].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
					zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[0], &pData->carriages[3].wheelSet[0].checkData[0]);
					pData->carriages[3].wheelSet[0].checkData[1].img = "";
					pData->carriages[3].wheelSet[0].checkData[1].state = 0;
					pData->carriages[3].wheelSet[0].checkData[1].axisArount = 0;
					pData->carriages[3].wheelSet[0].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
					pData->carriages[3].wheelSet[0].checkData[1].value = zdzhz_L[15];
					pData->carriages[3].wheelSet[0].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
					zhzAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[0], &pData->carriages[3].wheelSet[0].checkData[1]);
					pData->carriages[3].wheelSet[0].checkData[2].img = "";
					pData->carriages[3].wheelSet[0].checkData[2].state = 0;
					pData->carriages[3].wheelSet[0].checkData[2].axisArount = 1;
					pData->carriages[3].wheelSet[0].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
					pData->carriages[3].wheelSet[0].checkData[2].value = axleDataR[0][15];
					pData->carriages[3].wheelSet[0].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
					zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[0], &pData->carriages[3].wheelSet[0].checkData[2]);
					pData->carriages[3].wheelSet[0].checkData[3].img = "";
					pData->carriages[3].wheelSet[0].checkData[3].state = 0;
					pData->carriages[3].wheelSet[0].checkData[3].axisArount = 0;
					pData->carriages[3].wheelSet[0].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
					pData->carriages[3].wheelSet[0].checkData[3].value = axleDataL[0][15];
					pData->carriages[3].wheelSet[0].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
					zxwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[0], &pData->carriages[3].wheelSet[0].checkData[3]);
					pData->carriages[3].wheelSet[0].checkData[4].img = "";
					pData->carriages[3].wheelSet[0].checkData[4].state = 0;
					pData->carriages[3].wheelSet[0].checkData[4].axisArount = 0;
					pData->carriages[3].wheelSet[0].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
					pData->carriages[3].wheelSet[0].checkData[4].value = electricalDataL[0][5];
					pData->carriages[3].wheelSet[0].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
					djwdAlarmSet(pData, &pData->carriages[3], &pData->carriages[3].wheelSet[0], &pData->carriages[3].wheelSet[0].checkData[4]);
				}

				pData->carriages[4].carriagePos = 5;
				pData->carriages[4].wheelSetCount = 4;
				//3 for (int i = 0; i < 4; i++)
				{
					pData->carriages[4].wheelSet[3].pos = 4;
					pData->carriages[4].wheelSet[3].checkDataNum = 5;
					pData->carriages[4].wheelSet[3].checkData[0].img = "";
					pData->carriages[4].wheelSet[3].checkData[0].state = 0;
					pData->carriages[4].wheelSet[3].checkData[0].axisArount = 1;
					pData->carriages[4].wheelSet[3].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
					pData->carriages[4].wheelSet[3].checkData[0].value = zdzhz_R[16];
					pData->carriages[4].wheelSet[3].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
					zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[3], &pData->carriages[4].wheelSet[3].checkData[0]);
					pData->carriages[4].wheelSet[3].checkData[1].img = "";
					pData->carriages[4].wheelSet[3].checkData[1].state = 0;
					pData->carriages[4].wheelSet[3].checkData[1].axisArount = 0;
					pData->carriages[4].wheelSet[3].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
					pData->carriages[4].wheelSet[3].checkData[1].value = zdzhz_L[16];
					pData->carriages[4].wheelSet[3].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
					zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[3], &pData->carriages[4].wheelSet[3].checkData[1]);
					pData->carriages[4].wheelSet[3].checkData[2].img = "";
					pData->carriages[4].wheelSet[3].checkData[2].state = 0;
					pData->carriages[4].wheelSet[3].checkData[2].axisArount = 1;
					pData->carriages[4].wheelSet[3].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
					pData->carriages[4].wheelSet[3].checkData[2].value = axleDataR[0][16];
					pData->carriages[4].wheelSet[3].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
					zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[3], &pData->carriages[4].wheelSet[3].checkData[2]);
					pData->carriages[4].wheelSet[3].checkData[3].img = "";
					pData->carriages[4].wheelSet[3].checkData[3].state = 0;
					pData->carriages[4].wheelSet[3].checkData[3].axisArount = 0;
					pData->carriages[4].wheelSet[3].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
					pData->carriages[4].wheelSet[3].checkData[3].value = axleDataL[0][16];
					pData->carriages[4].wheelSet[3].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
					zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[3], &pData->carriages[4].wheelSet[3].checkData[3]);
					pData->carriages[4].wheelSet[3].checkData[4].img = "";
					pData->carriages[4].wheelSet[3].checkData[4].state = 0;
					pData->carriages[4].wheelSet[3].checkData[4].axisArount = 0;
					pData->carriages[4].wheelSet[3].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
					pData->carriages[4].wheelSet[3].checkData[4].value = electricalDataR[0][6];
					pData->carriages[4].wheelSet[3].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
					djwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[3], &pData->carriages[4].wheelSet[3].checkData[4]);
				}
				//2 for (int i = 0; i < 4; i++)
			{
				pData->carriages[4].wheelSet[2].pos = 3;
				pData->carriages[4].wheelSet[2].checkDataNum = 5;
				pData->carriages[4].wheelSet[2].checkData[0].img = "";
				pData->carriages[4].wheelSet[2].checkData[0].state = 0;
				pData->carriages[4].wheelSet[2].checkData[0].axisArount = 1;
				pData->carriages[4].wheelSet[2].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[4].wheelSet[2].checkData[0].value = zdzhz_R[17];
				pData->carriages[4].wheelSet[2].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[2], &pData->carriages[4].wheelSet[2].checkData[0]);
				pData->carriages[4].wheelSet[2].checkData[1].img = "";
				pData->carriages[4].wheelSet[2].checkData[1].state = 0;
				pData->carriages[4].wheelSet[2].checkData[1].axisArount = 0;
				pData->carriages[4].wheelSet[2].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[4].wheelSet[2].checkData[1].value = zdzhz_L[17];
				pData->carriages[4].wheelSet[2].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[2], &pData->carriages[4].wheelSet[2].checkData[1]);
				pData->carriages[4].wheelSet[2].checkData[2].img = "";
				pData->carriages[4].wheelSet[2].checkData[2].state = 0;
				pData->carriages[4].wheelSet[2].checkData[2].axisArount = 1;
				pData->carriages[4].wheelSet[2].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[4].wheelSet[2].checkData[2].value = axleDataR[0][17];
				pData->carriages[4].wheelSet[2].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[2], &pData->carriages[4].wheelSet[2].checkData[2]);
				pData->carriages[4].wheelSet[2].checkData[3].img = "";
				pData->carriages[4].wheelSet[2].checkData[3].state = 0;
				pData->carriages[4].wheelSet[2].checkData[3].axisArount = 0;
				pData->carriages[4].wheelSet[2].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[4].wheelSet[2].checkData[3].value = axleDataL[0][17];
				pData->carriages[4].wheelSet[2].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[2], &pData->carriages[4].wheelSet[2].checkData[3]);
				pData->carriages[4].wheelSet[2].checkData[4].img = "";
				pData->carriages[4].wheelSet[2].checkData[4].state = 0;
				pData->carriages[4].wheelSet[2].checkData[4].axisArount = 0;
				pData->carriages[4].wheelSet[2].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[4].wheelSet[2].checkData[4].value = electricalDataL[0][6];
				pData->carriages[4].wheelSet[2].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[2], &pData->carriages[4].wheelSet[2].checkData[4]);
			}
			//1 for (int i = 0; i < 4; i++)
			{
				pData->carriages[4].wheelSet[1].pos = 2;
				pData->carriages[4].wheelSet[1].checkDataNum = 5;
				pData->carriages[4].wheelSet[1].checkData[0].img = "";
				pData->carriages[4].wheelSet[1].checkData[0].state = 0;
				pData->carriages[4].wheelSet[1].checkData[0].axisArount = 1;
				pData->carriages[4].wheelSet[1].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[4].wheelSet[1].checkData[0].value = zdzhz_R[18];
				pData->carriages[4].wheelSet[1].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[1], &pData->carriages[4].wheelSet[1].checkData[0]);
				pData->carriages[4].wheelSet[1].checkData[1].img = "";
				pData->carriages[4].wheelSet[1].checkData[1].state = 0;
				pData->carriages[4].wheelSet[1].checkData[1].axisArount = 0;
				pData->carriages[4].wheelSet[1].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[4].wheelSet[1].checkData[1].value = zdzhz_L[18];
				pData->carriages[4].wheelSet[1].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[1], &pData->carriages[4].wheelSet[1].checkData[1]);
				pData->carriages[4].wheelSet[1].checkData[2].img = "";
				pData->carriages[4].wheelSet[1].checkData[2].state = 0;
				pData->carriages[4].wheelSet[1].checkData[2].axisArount = 1;
				pData->carriages[4].wheelSet[1].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[4].wheelSet[1].checkData[2].value = axleDataR[0][18];
				pData->carriages[4].wheelSet[1].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[1], &pData->carriages[4].wheelSet[1].checkData[2]);
				pData->carriages[4].wheelSet[1].checkData[3].img = "";
				pData->carriages[4].wheelSet[1].checkData[3].state = 0;
				pData->carriages[4].wheelSet[1].checkData[3].axisArount = 0;
				pData->carriages[4].wheelSet[1].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[4].wheelSet[1].checkData[3].value = axleDataL[0][18];
				pData->carriages[4].wheelSet[1].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[1], &pData->carriages[4].wheelSet[1].checkData[3]);
				pData->carriages[4].wheelSet[1].checkData[4].img = "";
				pData->carriages[4].wheelSet[1].checkData[4].state = 0;
				pData->carriages[4].wheelSet[1].checkData[4].axisArount = 0;
				pData->carriages[4].wheelSet[1].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[4].wheelSet[1].checkData[4].value = electricalDataR[0][7];
				pData->carriages[4].wheelSet[1].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[1], &pData->carriages[4].wheelSet[1].checkData[4]);
			}
			//0 for (int i = 0; i < 4; i++)
			{
				pData->carriages[4].wheelSet[0].pos = 1;
				pData->carriages[4].wheelSet[0].checkDataNum = 5;
				pData->carriages[4].wheelSet[0].checkData[0].img = "";
				pData->carriages[4].wheelSet[0].checkData[0].state = 0;
				pData->carriages[4].wheelSet[0].checkData[0].axisArount = 1;
				pData->carriages[4].wheelSet[0].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[4].wheelSet[0].checkData[0].value = zdzhz_R[19];
				pData->carriages[4].wheelSet[0].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[0], &pData->carriages[4].wheelSet[0].checkData[0]);
				pData->carriages[4].wheelSet[0].checkData[1].img = "";
				pData->carriages[4].wheelSet[0].checkData[1].state = 0;
				pData->carriages[4].wheelSet[0].checkData[1].axisArount = 0;
				pData->carriages[4].wheelSet[0].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[4].wheelSet[0].checkData[1].value = zdzhz_L[19];
				pData->carriages[4].wheelSet[0].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[0], &pData->carriages[4].wheelSet[0].checkData[1]);
				pData->carriages[4].wheelSet[0].checkData[2].img = "";
				pData->carriages[4].wheelSet[0].checkData[2].state = 0;
				pData->carriages[4].wheelSet[0].checkData[2].axisArount = 1;
				pData->carriages[4].wheelSet[0].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[4].wheelSet[0].checkData[2].value = axleDataR[0][19];
				pData->carriages[4].wheelSet[0].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[0], &pData->carriages[4].wheelSet[0].checkData[2]);
				pData->carriages[4].wheelSet[0].checkData[3].img = "";
				pData->carriages[4].wheelSet[0].checkData[3].state = 0;
				pData->carriages[4].wheelSet[0].checkData[3].axisArount = 0;
				pData->carriages[4].wheelSet[0].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[4].wheelSet[0].checkData[3].value = axleDataL[0][19];
				pData->carriages[4].wheelSet[0].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[0], &pData->carriages[4].wheelSet[0].checkData[3]);
				pData->carriages[4].wheelSet[0].checkData[4].img = "";
				pData->carriages[4].wheelSet[0].checkData[4].state = 0;
				pData->carriages[4].wheelSet[0].checkData[4].axisArount = 0;
				pData->carriages[4].wheelSet[0].checkData[4].checkItemCode = CHECKITEMCODE_LVW_DJWD;
				pData->carriages[4].wheelSet[0].checkData[4].value = electricalDataL[0][7];
				pData->carriages[4].wheelSet[0].checkData[4].checkItemName = CHECKITEMNAME_LVW_DJWD;
				djwdAlarmSet(pData, &pData->carriages[4], &pData->carriages[4].wheelSet[0], &pData->carriages[4].wheelSet[0].checkData[4]);
			}

			pData->carriages[5].carriagePos = 6;
			pData->carriages[5].wheelSetCount = 4;
			//3 for (int i = 0; i < 4; i++)
			{
				pData->carriages[5].wheelSet[3].pos = 4;
				pData->carriages[5].wheelSet[3].checkDataNum = 4;
				pData->carriages[5].wheelSet[3].checkData[0].img = "";
				pData->carriages[5].wheelSet[3].checkData[0].state = 0;
				pData->carriages[5].wheelSet[3].checkData[0].axisArount = 1;
				pData->carriages[5].wheelSet[3].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[3].checkData[0].value = zdzhz_R[20];
				pData->carriages[5].wheelSet[3].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[3], &pData->carriages[5].wheelSet[3].checkData[0]);
				pData->carriages[5].wheelSet[3].checkData[1].img = "";
				pData->carriages[5].wheelSet[3].checkData[1].state = 0;
				pData->carriages[5].wheelSet[3].checkData[1].axisArount = 0;
				pData->carriages[5].wheelSet[3].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[3].checkData[1].value = zdzhz_L[20];
				pData->carriages[5].wheelSet[3].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[3], &pData->carriages[5].wheelSet[3].checkData[1]);
				pData->carriages[5].wheelSet[3].checkData[2].img = "";
				pData->carriages[5].wheelSet[3].checkData[2].state = 0;
				pData->carriages[5].wheelSet[3].checkData[2].axisArount = 1;
				pData->carriages[5].wheelSet[3].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[3].checkData[2].value = axleDataR[0][20];
				pData->carriages[5].wheelSet[3].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[3], &pData->carriages[5].wheelSet[3].checkData[2]);
				pData->carriages[5].wheelSet[3].checkData[3].img = "";
				pData->carriages[5].wheelSet[3].checkData[3].state = 0;
				pData->carriages[5].wheelSet[3].checkData[3].axisArount = 0;
				pData->carriages[5].wheelSet[3].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[3].checkData[3].value = axleDataL[0][20];
				pData->carriages[5].wheelSet[3].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[3], &pData->carriages[5].wheelSet[3].checkData[3]);
			}
			//2 for (int i = 0; i < 4; i++)
			{
				pData->carriages[5].wheelSet[2].pos = 3;
				pData->carriages[5].wheelSet[2].checkDataNum = 4;
				pData->carriages[5].wheelSet[2].checkData[0].img = "";
				pData->carriages[5].wheelSet[2].checkData[0].state = 0;
				pData->carriages[5].wheelSet[2].checkData[0].axisArount = 1;
				pData->carriages[5].wheelSet[2].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[2].checkData[0].value = zdzhz_R[21];
				pData->carriages[5].wheelSet[2].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[2], &pData->carriages[5].wheelSet[2].checkData[0]);
				pData->carriages[5].wheelSet[2].checkData[1].img = "";
				pData->carriages[5].wheelSet[2].checkData[1].state = 0;
				pData->carriages[5].wheelSet[2].checkData[1].axisArount = 0;
				pData->carriages[5].wheelSet[2].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[2].checkData[1].value = zdzhz_L[21];
				pData->carriages[5].wheelSet[2].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[2], &pData->carriages[5].wheelSet[2].checkData[1]);
				pData->carriages[5].wheelSet[2].checkData[2].img = "";
				pData->carriages[5].wheelSet[2].checkData[2].state = 0;
				pData->carriages[5].wheelSet[2].checkData[2].axisArount = 1;
				pData->carriages[5].wheelSet[2].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[2].checkData[2].value = axleDataR[0][21];
				pData->carriages[5].wheelSet[2].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[2], &pData->carriages[5].wheelSet[2].checkData[2]);
				pData->carriages[5].wheelSet[2].checkData[3].img = "";
				pData->carriages[5].wheelSet[2].checkData[3].state = 0;
				pData->carriages[5].wheelSet[2].checkData[3].axisArount = 0;
				pData->carriages[5].wheelSet[2].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[2].checkData[3].value = axleDataL[0][21];
				pData->carriages[5].wheelSet[2].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[2], &pData->carriages[5].wheelSet[2].checkData[3]);
			}
			//1 for (int i = 0; i < 4; i++)
			{
				pData->carriages[5].wheelSet[1].pos = 2;
				pData->carriages[5].wheelSet[1].checkDataNum = 4;
				pData->carriages[5].wheelSet[1].checkData[0].img = "";
				pData->carriages[5].wheelSet[1].checkData[0].state = 0;
				pData->carriages[5].wheelSet[1].checkData[0].axisArount = 1;
				pData->carriages[5].wheelSet[1].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[1].checkData[0].value = zdzhz_R[22];
				pData->carriages[5].wheelSet[1].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[1], &pData->carriages[5].wheelSet[1].checkData[0]);
				pData->carriages[5].wheelSet[1].checkData[1].img = "";
				pData->carriages[5].wheelSet[1].checkData[1].state = 0;
				pData->carriages[5].wheelSet[1].checkData[1].axisArount = 0;
				pData->carriages[5].wheelSet[1].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[1].checkData[1].value = zdzhz_L[22];
				pData->carriages[5].wheelSet[1].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[1], &pData->carriages[5].wheelSet[1].checkData[1]);
				pData->carriages[5].wheelSet[1].checkData[2].img = "";
				pData->carriages[5].wheelSet[1].checkData[2].state = 0;
				pData->carriages[5].wheelSet[1].checkData[2].axisArount = 1;
				pData->carriages[5].wheelSet[1].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[1].checkData[2].value = axleDataR[0][22];
				pData->carriages[5].wheelSet[1].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[1], &pData->carriages[5].wheelSet[1].checkData[2]);
				pData->carriages[5].wheelSet[1].checkData[3].img = "";
				pData->carriages[5].wheelSet[1].checkData[3].state = 0;
				pData->carriages[5].wheelSet[1].checkData[3].axisArount = 0;
				pData->carriages[5].wheelSet[1].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[1].checkData[3].value = axleDataL[0][22];
				pData->carriages[5].wheelSet[1].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[1], &pData->carriages[5].wheelSet[1].checkData[3]);
			}
			//0 for (int i = 0; i < 4; i++)
			{
				pData->carriages[5].wheelSet[0].pos = 1;
				pData->carriages[5].wheelSet[0].checkDataNum = 4;
				pData->carriages[5].wheelSet[0].checkData[0].img = "";
				pData->carriages[5].wheelSet[0].checkData[0].state = 0;
				pData->carriages[5].wheelSet[0].checkData[0].axisArount = 1;  //yxw modify
				pData->carriages[5].wheelSet[0].checkData[0].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[0].checkData[0].value = zdzhz_R[23];
				pData->carriages[5].wheelSet[0].checkData[0].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[0], &pData->carriages[5].wheelSet[0].checkData[0]);
				pData->carriages[5].wheelSet[0].checkData[1].img = "";
				pData->carriages[5].wheelSet[0].checkData[1].state = 0;
				pData->carriages[5].wheelSet[0].checkData[1].axisArount = 0;  //yxw modify
				pData->carriages[5].wheelSet[0].checkData[1].checkItemCode = CHECKITEMCODE_LVW_ZDZHZ;
				pData->carriages[5].wheelSet[0].checkData[1].value = zdzhz_L[23];
				pData->carriages[5].wheelSet[0].checkData[1].checkItemName = CHECKITEMNAME_LVW_ZDZHZ;
				zhzAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[0], &pData->carriages[5].wheelSet[0].checkData[1]);
				pData->carriages[5].wheelSet[0].checkData[2].img = "";
				pData->carriages[5].wheelSet[0].checkData[2].state = 0;
				pData->carriages[5].wheelSet[0].checkData[2].axisArount = 1;  //yxw modify
				pData->carriages[5].wheelSet[0].checkData[2].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[0].checkData[2].value = axleDataR[0][23];
				pData->carriages[5].wheelSet[0].checkData[2].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[0], &pData->carriages[5].wheelSet[0].checkData[2]);
				pData->carriages[5].wheelSet[0].checkData[3].img = "";
				pData->carriages[5].wheelSet[0].checkData[3].state = 0;
				pData->carriages[5].wheelSet[0].checkData[3].axisArount = 0;  //yxw modify
				pData->carriages[5].wheelSet[0].checkData[3].checkItemCode = CHECKITEMCODE_LVW_ZXWD;
				pData->carriages[5].wheelSet[0].checkData[3].value = axleDataL[0][23];
				pData->carriages[5].wheelSet[0].checkData[3].checkItemName = CHECKITEMNAME_LVW_ZXWD;
				zxwdAlarmSet(pData, &pData->carriages[5], &pData->carriages[5].wheelSet[0], &pData->carriages[5].wheelSet[0].checkData[3]);
			}
}

void AlgorithmBase::vibrateDataToJson(struct VIBRATE_DATA *pData)
{
	if (NULL == pData)
	{
		return;
	}

	//m_Semaphore.acquire(16);
	QJsonObject  totalData;

	qDebug() << "vibrateDataToJson start";
	//algorithmSendMsg(QStringLiteral("vibrateDataToJson start"), 0);

	totalData.insert("record_id", pData->record_id);
	totalData.insert("groupName", pData->groupName);
	totalData.insert("checkTime", pData->checkTime);
	totalData["passDw"] = pData->passDw;
	totalData["checkDir"] = pData->checkDir;
	totalData.insert("jcpCode", pData->jcpCode);
	totalData["speed"] = pData->speed;
	totalData["ambientTemperature"] = pData->ambientTemperature;

	QJsonArray carriage_list;

	for (int i = 0; i<pData->carriageNum; i++)
	{
		QJsonObject carriage;
		carriage["carriageNumber"] = pData->carriages[i].carriageNumber;
		carriage["carriagePos"] = pData->carriages[i].carriagePos;
		carriage["wheelSetCount"] = pData->carriages[i].wheelSetCount;
		QJsonArray wheelSet_list;
		for (int j = 0; j<pData->carriages[i].wheelSetCount; j++)
		{

			QJsonObject wheelSet;
			wheelSet["pos"] = pData->carriages[i].wheelSet[j].pos;

			QJsonArray checkData_list;
			for (int k = 0; k<pData->carriages[i].wheelSet[j].checkDataNum; k++)
			{
				QJsonObject checkData;
				checkData["img"] = pData->carriages[i].wheelSet[j].checkData[k].img;
				checkData["state"] = pData->carriages[i].wheelSet[j].checkData[k].state;
				checkData["axisArount"] = pData->carriages[i].wheelSet[j].checkData[k].axisArount;
				checkData["checkItemCode"] = pData->carriages[i].wheelSet[j].checkData[k].checkItemCode;
				checkData["value"] = pData->carriages[i].wheelSet[j].checkData[k].value;
				checkData["checkItemName"] = pData->carriages[i].wheelSet[j].checkData[k].checkItemName;
				checkData_list.append(checkData);
			}
			wheelSet["checkData"] = checkData_list;
			wheelSet_list.append(wheelSet);

		}
		carriage["wheelSet"] = wheelSet_list;
		carriage_list.append(carriage);
	}
	totalData["carriages"] = carriage_list;

	QJsonArray alarm_list;

	for (int i = 0; i<pData->alarmNum; i++)
	{
		QJsonObject alarm;
		alarm["groupName"] = pData->alarmDatas[i].groupName;
		alarm["alarmTime"] = pData->alarmDatas[i].alarmTime;
		alarm["carriagePos"] = pData->alarmDatas[i].carriagePos;
		alarm["carriageNumber"] = pData->alarmDatas[i].carriageNumber;
		alarm["modelId"] = pData->alarmDatas[i].modelId;
		alarm["modelCode"] = pData->alarmDatas[i].modelCode;
		alarm["checkItemId"] = pData->alarmDatas[i].checkItemId;
		alarm["checkItemCode"] = pData->alarmDatas[i].checkItemCode;

		alarm["alarmPos1"] = pData->alarmDatas[i].alarmPos1;
		alarm["alarmPos2"] = pData->alarmDatas[i].alarmPos2;
		alarm["alarmValue1"] = pData->alarmDatas[i].alarmValue1;
		alarm["alarmLevel"] = pData->alarmDatas[i].alarmLevel;

		alarm_list.append(alarm);

	}
	totalData.insert("alarmDatas", alarm_list);

	//��ʾ��
	QJsonDocument doc(totalData);
	QByteArray byteArrayUft8 = doc.toJson();

	boolean isutf8 = IsTextUTF8((char*)byteArrayUft8.data(), byteArrayUft8.length());
	qDebug() << "vibrateDataToJson end" << isutf8;
	qDebug() << "JSON�洢" ;
	{
#if 1
		bool exist;
		QString fileName;

		QDir *folder = new QDir;
		exist = folder->exists("D:/Vabrition/Resources/Json");//�鿴Ŀ¼�Ƿ���ڣ������Ǳ��浽���棩

		if (!exist){//�����ھʹ���
			bool ok = folder->mkdir("D:/Vabrition/Resources/Json");
		}
		fileName = tr("D:/Vabrition/Resources/Json/%1.txt").arg(pData->record_id);

		QFile f(fileName);
		if (f.open(QIODevice::ReadWrite | QIODevice::Text)){//chongxinд�� ��ӽ�����\r\n
			f.write(byteArrayUft8);
		}
		f.close();
#endif
	}


	{
		NetworkLogic hpptPost = NetworkLogic();
		hpptPost.postHttps("http://192.168.172.99:8080/collection/checkData/lvwRecord/", byteArrayUft8);
		//hpptPost.postHttps("https://api.apiopen.top/api/getTime/", byteArrayUft8);
		//hpptPost.postHttps("https://www.sojson.com/httpRequest/", byteArrayUft8);
	}
}

/************************************/

AlgorithmBase::AlgorithmBase(QObject *parent) : QObject(parent)
{
	m_pMySqlHelper = ImportDllClass::GetInstance()->m_pMySqlHelper;

}

AlgorithmBase::~AlgorithmBase()
{
	if (NULL != ETHDLLHandel)
	{
		FreeLibrary(ETHDLLHandel);
	}
	ETHDLLHandel = NULL;

	if (NULL != m_pMySqlHelper)
	{
		delete m_pMySqlHelper;
	}
	m_pMySqlHelper = NULL;
}

void AlgorithmBase::initialETHDLL()
{

	ETHDLLHandel = LoadLibrary(TEXT("ETHDLL.dll"));
	if (NULL != ETHDLLHandel)
	{
		SysInit = (int(__stdcall *)(unsigned int *ipbuff, unsigned int num))GetProcAddress(ETHDLLHandel, "SysInit");
		if (!SysInit)
		{
			algorithmSendMsg(QStringLiteral("SysInit������ʼ���쳣"), 1);
		}

		IP_StrToInt = (unsigned int(__stdcall *)(char *strIp))GetProcAddress(ETHDLLHandel, "IP_StrToInt");
		if (!IP_StrToInt)
		{
			algorithmSendMsg(QStringLiteral("IP_StrToInt������ʼ���쳣"), 1);
		}

		IP_IntToStr = (int(__stdcall *)(char *strIp, unsigned int ip))GetProcAddress(ETHDLLHandel, "IP_IntToStr");
		if (!IP_IntToStr)
		{
			algorithmSendMsg(QStringLiteral("IP_IntToStr������ʼ���쳣"), 1);
		}

		ConnectCreate = (int(__stdcall *)(unsigned int inst_ip, unsigned short inst_port, unsigned short local_port,
			unsigned int type, unsigned int outtime, unsigned int num))GetProcAddress(ETHDLLHandel, "ConnectCreate");
		if (!ConnectCreate)
		{
			algorithmSendMsg(QStringLiteral("ConnectCreate������ʼ���쳣"), 1);
		}

		ConnectDel = (int(__stdcall *)(unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ConnectDel");
		if (!ConnectDel)
		{
			algorithmSendMsg(QStringLiteral("ConnectDel������ʼ���쳣"), 1);
		}

		ADSyncParaWrite = (int(__stdcall *)(unsigned int  ad_frequency, unsigned int  ad_range, unsigned int  ain_select,
			unsigned short ch_enabled, unsigned short masterflag, unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ADSyncParaWrite");
		if (!ADSyncParaWrite)
		{
			algorithmSendMsg(QStringLiteral("ADSyncParaWrite������ʼ���쳣"), 1);
		}

		ADGainWrite = (int(__stdcall *)(unsigned short gain_ch0, unsigned short gain_ch1, unsigned short gain_ch2,
			unsigned short gain_ch3, unsigned short gain_ch4, unsigned short gain_ch5, unsigned short gain_ch6,
			unsigned short gain_ch7, unsigned short gain_ch8, unsigned short gain_ch9, unsigned short gain_ch10,
			unsigned short gain_ch11, unsigned short gain_ch12, unsigned short gain_ch13, unsigned short gain_ch14,
			unsigned short gain_ch15, unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ADGainWrite");
		if (!ADGainWrite)
		{
			algorithmSendMsg(QStringLiteral("ADGainWrite������ʼ���쳣"), 1);
		}

		ADTriggerExWrite = (int(__stdcall *)(unsigned short enabled, unsigned short A0_trig_type, unsigned short A1_trig_type,
			unsigned short D0_trig_type, unsigned short D1_trig_type, unsigned short A0_trig_ch_sel, unsigned short A1_trig_ch_sel,
			unsigned int   trig_num, int   A0_trig_vol, int   A1_trig_vol, unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ADTriggerExWrite");
		if (!ADTriggerExWrite)
		{
			algorithmSendMsg(QStringLiteral("ADTriggerExWrite������ʼ���쳣"), 1);
		}

		ADTriggerWrite = (int(__stdcall *) (unsigned char enabled, unsigned char type, unsigned short spring_clk,
			unsigned short spring_num, unsigned short spring_vol, unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ADTriggerWrite");
		if (!ADTriggerWrite)
		{
			algorithmSendMsg(QStringLiteral("ADTriggerWrite������ʼ���쳣"), 1);
		}

		ADStart = (int(__stdcall *)(unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ADStart");
		if (!ADStart)
		{
			algorithmSendMsg(QStringLiteral("ADStart������ʼ���쳣"), 1);
		}

		ADDataRead = (int(__stdcall *)(short *pData, unsigned int nCount, unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ADDataRead");
		if (!ADDataRead)
		{
			algorithmSendMsg(QStringLiteral("ADDataRead������ʼ���쳣"), 1);
		}

		ADStop = (int(__stdcall *)(unsigned int socket_num))GetProcAddress(ETHDLLHandel, "ADStop");
		if (!ADStop)
		{
			algorithmSendMsg(QStringLiteral("ADStop������ʼ���쳣"), 1);
		}

		AdStatusRead = (int(__stdcall *)(unsigned int *ad_status, unsigned int *fifo_status, unsigned int *fifoff_num,
			unsigned int *cherr_num, unsigned int *fifodata_num, unsigned int *ad_real_fre, unsigned int *ad_num,
			unsigned int *ch_num, unsigned int socket_num))GetProcAddress(ETHDLLHandel, "AdStatusRead");
		if (!AdStatusRead)
		{
			algorithmSendMsg(QStringLiteral("ADStatusRead������ʼ���쳣"), 1);
		}

	}
	else
	{
		algorithmSendMsg(QStringLiteral("DLL��������쳣"), 1);
	}

	m_pLogHelper = ImportDllClass::GetInstance()->m_pLogHelper;
	IniHelper *iniHelper = ImportDllClass::GetInstance()->m_pIniHelper;
	m_strConnectIP = iniHelper->readIniStr("system", "data_box_ip", m_strAllSoftGrabParamPath + "LogicParam\\SystemParam.ini");
	algorithmSendMsg(QStringLiteral("�ۺ����ݺ�����IP��") + m_strConnectIP, 0);
	m_strFileSavePathRoot = iniHelper->readIniStr("system", "file_root_path", m_strAllSoftGrabParamPath + "LogicParam\\SystemParam.ini");
	m_strStation = iniHelper->readIniStr("system", "station", m_strAllSoftGrabParamPath + "LogicParam\\SystemParam.ini");

	//ʵ�����㷨����
	algotrithOperateRight = new AlgorithmOperate();
	algotrithOperateLeft = new AlgorithmOperate();

	//��ʼ���ź���������
	m_Semaphore.release(16);

}

void AlgorithmBase::openDevice()
{
	m_bIsOpen = true;
	algorithmSendMsg(QStringLiteral("���ڴ��豸..."), 0);
	QtConcurrent::run([=](){
		QByteArray byte_ip = m_strConnectIP.toLatin1();
		int_ip = IP_StrToInt(byte_ip.data());
		int ret = SysInit(&int_ip, 1);
		if (-1 == ret)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("���ӳ�ʼ���쳣"), 1);
		}

		socket_index = ConnectCreate(int_ip, 1600, 0, 1, 500, 3);
		if (socket_index < 0)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("�������ӳ����쳣���������Ϊ��") + QString::number(socket_index), 1);
		}

		ret = ADSyncParaWrite(10000, 1, 0, 0xFFFF, 0, socket_index);
		if (-1 == ret)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("ͬ���ɼ��������쳣"), 1);
		}

		ret = ADGainWrite(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, socket_index);
		if (-1 == ret)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("�ɼ������������쳣"), 1);
		}

		if (m_bIsOpen)
		{
			algorithmSendMsg(QStringLiteral("�豸�򿪳ɹ�"), 0);
		}

		ret = ConnectDel(socket_index);
		if (ret)
		{
			algorithmSendMsg(QStringLiteral("�豸�Ͽ�����"), 0);
		}
	});
}

void AlgorithmBase::startGrab(QString strTrainRunNumber)
{
	initialData();
	m_strTrainRunNumber = strTrainRunNumber;

	QtConcurrent::run([=](){
		short* arrayData = NULL;
		m_bIsOpen = true;

		QByteArray byte_ip = m_strConnectIP.toLatin1();
		int_ip = IP_StrToInt(byte_ip.data());
		int ret = SysInit(&int_ip, 1);
		if (-1 == ret)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("���ӳ�ʼ���쳣"), 1);
		}

		socket_index = ConnectCreate(int_ip, 1600, 0, 1, 500, 3);
		if (socket_index < 0)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("�������ӳ����쳣���������Ϊ��") + QString::number(socket_index), 1);
		}

		ret = ADSyncParaWrite(10000, 1, 0, 0xFFFF, 0, socket_index);
		if (-1 == ret)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("ͬ���ɼ��������쳣"), 1);
		}

		ret = ADGainWrite(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, socket_index);
		if (-1 == ret)
		{
			m_bIsOpen = false;
			algorithmSendMsg(QStringLiteral("�ɼ������������쳣"), 1);
		}

		if (m_bIsOpen)
		{
			try
			{
				int ret = ADStart(socket_index);
				if (-1 == ret)
				{
					algorithmSendMsg(QStringLiteral("����A/D�ɼ�ʧ��"), 1);
					m_bIsGrab = false;
					return;
				}
				m_bIsGrab = true;
				algorithmSendMsg(QStringLiteral("����A/D�ɼ�"), 0);
				QThread::msleep(100);  //��ֹ��ȡ����ʱ����

				while (m_bIsGrab)
				{
					arrayData = new short[720];
					int ret = ADDataRead(arrayData, 720, socket_index);
					for (int i = 0; i < 720; i++)
					{
						allGrabData.push_back((*(arrayData + i))*10.0 / 0x7fff);
					}
					delete[] arrayData;
					arrayData = NULL;
				}
			}
			catch (...)
			{
				algorithmSendMsg(QStringLiteral("���ݲɼ��г��ִ���"), 1);
				int ret = ADStop(socket_index);
				if (ret > 0)
				{
					algorithmSendMsg(QStringLiteral("ֹͣ�ɼ�����"), 0);
				}
				else
				{
					algorithmSendMsg(QStringLiteral("ֹͣ�ɼ������쳣"), 1);
				}
				return;
			}
			if (NULL != arrayData)
			{
				delete[] arrayData;
			}
			arrayData = NULL;
			int ret = ADStop(socket_index);
			if (ret > 0)
			{
				algorithmSendMsg(QStringLiteral("ֹͣ�ɼ�����"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("ֹͣ�ɼ������쳣"), 1);
			}
			ret = ConnectDel(socket_index);
			if (ret > 0)
			{
				algorithmSendMsg(QStringLiteral("�ɼ������ѶϿ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("�ɼ������ѶϿ��쳣"), 1);
			}

			//algorithmSendMsg(QStringLiteral("��ʼ���ݷ���������"), 0);
			//analysisAndSaveData();
		}
		else
		{
			algorithmSendMsg(QStringLiteral("�豸δ�򿪣��޷������ɼ�"), 1);
		}
	});

}

void AlgorithmBase::stopGrab(QString strTrainNumber, QString strLineNumber, QString strControlPort, int flag)
{
	algorithmSendMsg(QStringLiteral("�յ�ֹͣ�ɼ������ʼֹͣ�ɼ�"), 0);
	m_bIsGrab = false;
	switch (flag)
	{
	case 0:
		//********************************//
		break;
	case 1:
		m_strTrainNumber = strTrainNumber;
		m_strLineNumber = strLineNumber;
		m_strMainControlPort = strControlPort;
		QThread::sleep(1);    //��ͣ1s��ֹͣ�ɼ�
		algorithmSendMsg(QStringLiteral("��ʼ���ݷ���������"), 0);
		analysisAndSaveData();
		break;
	}
}


void AlgorithmBase::closeDevice()
{
	m_bIsOpen = false;
	int ret = ConnectDel(socket_index);
	if (-1 == ret)
	{
		algorithmSendMsg(QStringLiteral("ֹͣ����ʧ��"), 1);
	}

}

void AlgorithmBase::initialData()
{
	m_bIsGetRightData = false;
	m_bIsGetLeftData = false;
	std::vector<double>().swap(allGrabData);
	std::vector<std::vector<double>>().swap(vibrationDataRight);
	std::vector<std::vector<double>>().swap(axleDataRight);
	std::vector<std::vector<double>>().swap(electricalDataRight);
	std::vector<std::vector<double>>().swap(vibrationDataLeft);
	std::vector<std::vector<double>>().swap(axleDataLeft);
	std::vector<std::vector<double>>().swap(electricalDataLeft);
	std::vector<double>().swap(v1);       //8·���ź�
	std::vector<double>().swap(v2);
	std::vector<double>().swap(v3);
	std::vector<double>().swap(v4);
	std::vector<double>().swap(v5);
	std::vector<double>().swap(v6);
	std::vector<double>().swap(v7);
	std::vector<double>().swap(v8);
	std::vector<double>().swap(g1);
	std::vector<double>().swap(g2);
	std::vector<double>().swap(g3);
	std::vector<double>().swap(g4);
	std::vector<double>().swap(t1);
	std::vector<double>().swap(t2);
	std::vector<double>().swap(t3);
	std::vector<double>().swap(t4);

}

void AlgorithmBase::analysisAndSaveData()
{
	if (allGrabData.size() == 0)
	{
		algorithmSendMsg(QStringLiteral("ϵͳδ�ɼ����κ����ݣ�ֹͣ�������ݴ���"), 1);
		return;
	}
	if (allGrabData.size() > 8000000)
	{
		algorithmSendMsg(QStringLiteral("���ݲɼ�������8�����������ݲɼ��쳣��ֹͣ���ݴ���"), 1);
		return;
	}
	//���ݹ���
	for (size_t i = 0; i < allGrabData.size() / 16; i++)
	{
		v1.push_back(allGrabData[i * 16 + 0]); v2.push_back(allGrabData[i * 16 + 1]); v3.push_back(allGrabData[i * 16 + 2]);
		v4.push_back(allGrabData[i * 16 + 3]); v5.push_back(allGrabData[i * 16 + 4]); v6.push_back(allGrabData[i * 16 + 5]);
		v7.push_back(allGrabData[i * 16 + 6]); v8.push_back(allGrabData[i * 16 + 7]);

		g2.push_back(allGrabData[i * 16 + 8]); g1.push_back(allGrabData[i * 16 + 9]); g3.push_back(allGrabData[i * 16 + 10]);
		g4.push_back(allGrabData[i * 16 + 11]);

		t1.push_back(allGrabData[i * 16 + 12]); t2.push_back(allGrabData[i * 16 + 13]); t3.push_back(allGrabData[i * 16 + 14]);
		t4.push_back(allGrabData[i * 16 + 15]);
	}

	

	//���ݴ洢
	QString strSavePath = m_strFileSavePathRoot + m_strTrainRunNumber + "\\VAIS\\";
	strSavePath.replace("/", "\\");

	//���汾�����ݲ����ö��̴߳�����ֹ���������쳣�����ݶ�ʧ���޷����ֳ������ԭ��
	//д��������
	saveDataToTxt(v1, "vibration1", strSavePath);  //������1������
	algorithmSendMsg(QStringLiteral("vibration1�������"), 0);
	saveDataToTxt(v2, "vibration2", strSavePath);  //������2������
	algorithmSendMsg(QStringLiteral("vibration2�������"), 0);
	saveDataToTxt(v3, "vibration3", strSavePath);  //������3������
	algorithmSendMsg(QStringLiteral("vibration3�������"), 0);
	saveDataToTxt(v4, "vibration4", strSavePath);  //������4������
	algorithmSendMsg(QStringLiteral("vibration4�������"), 0);
	saveDataToTxt(v5, "vibration5", strSavePath);  //������5������
	algorithmSendMsg(QStringLiteral("vibration5�������"), 0);
	saveDataToTxt(v6, "vibration6", strSavePath);  //������6������
	algorithmSendMsg(QStringLiteral("vibration6�������"), 0);
	saveDataToTxt(v7, "vibration7", strSavePath);  //������7������
	algorithmSendMsg(QStringLiteral("vibration7�������"), 0);
	saveDataToTxt(v8, "vibration8", strSavePath);  //������8������
	algorithmSendMsg(QStringLiteral("vibration8�������"), 0);

	//д��Ÿ�����
	saveDataToTxt(g1, "magnetic1", strSavePath);   //����Ÿ�1������
	algorithmSendMsg(QStringLiteral("magnetic1�������"), 0);
	saveDataToTxt(g2, "magnetic2", strSavePath);   //����Ÿ�2������
	algorithmSendMsg(QStringLiteral("magnetic2�������"), 0);
	saveDataToTxt(g3, "magnetic3", strSavePath);   //����Ÿ�3������
	algorithmSendMsg(QStringLiteral("magnetic3�������"), 0);
	saveDataToTxt(g4, "magnetic4", strSavePath);   //����Ÿ�4������
	algorithmSendMsg(QStringLiteral("magnetic4�������"), 0);

	//д���¶�����
	saveDataToTxt(t1, "temperate1", strSavePath);   //�����¶�1������
	algorithmSendMsg(QStringLiteral("temperate1�������"), 0);
	saveDataToTxt(t2, "temperate2", strSavePath);   //�����¶�2������
	algorithmSendMsg(QStringLiteral("temperate2�������"), 0);
	saveDataToTxt(t3, "temperate3", strSavePath);   //�����¶�3������
	algorithmSendMsg(QStringLiteral("temperate3�������"), 0);
	saveDataToTxt(t4, "temperate4", strSavePath);   //�����¶�4������
	algorithmSendMsg(QStringLiteral("temperate4�������"), 0);

	//��������
	operateData(v1, v2, v3, v4, v5, v6, v7, v8, g1, g2, g3, g4, t1, t2, t3, t4);
}

void AlgorithmBase::saveDataToTxt(std::vector<double> data, QString dataName, QString fileRootPath)
{
	try
	{
		QDir dir;
		if (!dir.exists(fileRootPath))
		{
			dir.mkpath(fileRootPath);
		}

		QFile file(fileRootPath + dataName + ".txt");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			algorithmSendMsg(dataName + QStringLiteral(".txt�ļ�����ʧ��"), 1);
		}
		QTextStream in(&file);
		for (int i = 0; i < data.size(); i++)
		{
			in << data[i] << endl;
		}
	}
	catch (...)
	{
		algorithmSendMsg(dataName + QStringLiteral("����д���������"), 1);
	}

}

void AlgorithmBase::operateData(std::vector<double> &v1, std::vector<double> &v2, std::vector<double> &v3, std::vector<double> &v4,
	std::vector<double> &v5, std::vector<double> &v6, std::vector<double> &v7, std::vector<double> &v8,
	std::vector<double> &g1, std::vector<double> &g2, std::vector<double> &g3, std::vector<double> &g4,
	std::vector<double> &t1, std::vector<double> &t2, std::vector<double> &t3, std::vector<double> &t4)
{
	algorithmSendMsg(QStringLiteral("��ʼ��������"), 0);
	//��������
	std::vector<double> sensitivity1 = { 0.0099, 0.0099, 0.010, 0.0101 };    //�񶯴�����������
	std::vector<double> sensitivity2 = { 0.0098, 0.010, 0.0097, 0.0098 };

	QtConcurrent::run([=](){
		if (!m_bIsRunAlg)
		{
			m_bIsRunAlg = true;
			algotrithOperateRight->initialData(v1, v2, v3, v4, g2, g3, g4, t1, t3, sensitivity1, 1);
			algotrithOperateRight->runSingle();

			vibrationDataRight = algotrithOperateRight->vibrationEigenValue;
			axleDataRight = algotrithOperateRight->bearPalteTemperatureDivided;
			electricalDataRight = algotrithOperateRight->electricalTemperatureDivided;
			m_dTrainSpeed = algotrithOperateRight->m_dTrainSpeed;
			m_dEnvironmentTemp = algotrithOperateRight->m_dEnvironmentTemp;
			algorithmSendMsg(QStringLiteral("�Ҳ�������"), 0);

			algotrithOperateLeft->initialData(v5, v6, v7, v8, g2, g3, g4, t2, t4, sensitivity2, 0);
			algotrithOperateLeft->runSingle();
			vibrationDataLeft = algotrithOperateLeft->vibrationEigenValue;
			axleDataLeft = algotrithOperateLeft->bearPalteTemperatureDivided;
			electricalDataLeft = algotrithOperateLeft->electricalTemperatureDivided;
			algorithmSendMsg(QStringLiteral("���������"), 0);

			dataCombination(); //���������ۺϴ�����ʽ������ȱʧ���³�����������쳣

		}
		else
		{
			algorithmSendMsg(QStringLiteral("�Ҳ�ǰһ�μ��㻹δ�����������ظ�����"), 1);
		}

	});
}

void AlgorithmBase::writeDataToSql(std::vector<std::vector<double>> vibrateDataL, std::vector<std::vector<double>> axleDataL,
	std::vector<std::vector<double>> electricalDataL, std::vector<std::vector<double>> vibrateDataR,
	std::vector<std::vector<double>> axleDataR, std::vector<std::vector<double>> electricalDataR)
{
	//�ӱ�"train_param"��ѯ7������ֵ�ı�����Ԥ��ֵ
	double jfg_warn; int jfg_warn_num = 0;
	double jfg_alarm; int jfg_alarm_num = 0;
	double fz_warn; int fz_warn_num = 0;
	double fz_alarm; int fz_alarm_num = 0;
	double fzyz_warn; int fzyz_warn_num = 0;
	double fzyz_alarm; int fzyz_alarm_num = 0;
	double qd_warn; int qd_warn_num = 0;
	double qd_alarm; int qd_alarm_num = 0;
	double qdyz_warn; int qdyz_warn_num = 0;
	double qdyz_alarm; int qdyz_alarm_num = 0;
	double zw_warn; int zw_warn_num = 0;
	double zw_alarm; int zw_alarm_num = 0;
	double zwinc_warn; int zwinc_warn_num = 0;
	double zwinc_alarm; int zwinc_alarm_num = 0;
	double zw_max;
	double zwinc_max;
	double motortemp_warn; int motortemp_warn_num = 0;
	double motortemp_alarm; int motortemp_alarm_num = 0;
	double motortempinc_warn; int motortempinc_warn_num = 0;
	double motortempinc_alarm; int motortempinc_alarm_num = 0;
	double motortemp_max;
	double motortempinc_max;
	std::vector<double> zdzhz_L;
	std::vector<double> zdzhz_R;
	double zdzhz_max;

	//20210302 yxw add:��ѯ���ۺ�ֵ�ı�����Ԥ��ֵ
	double zdzhz_warn; int zdzhz_warn_num = 0;
	double zdzhz_alarm; int zdzhz_alarm_num = 0;
	double zdzhz_ka = 0.5;//����ϵ��Ĭ��0.5
	double zdzhz_kb = 0.5;
	
	if (m_pMySqlHelper->openSql())
	{
#pragma region �����ݿ��ȡԤ���ͱ�������
		QString strSqlRead = "";
		m_pMySqlHelper->readSqlValue("train_param", "jfg_warn", strSqlRead);
		jfg_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "jfg_alarm", strSqlRead);
		jfg_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "fz_warn", strSqlRead);
		fz_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "fz_alarm", strSqlRead);
		fz_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "fzyz_warn", strSqlRead);
		fzyz_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "fzyz_alarm", strSqlRead);
		fzyz_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "qd_warn", strSqlRead);
		qd_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "qd_alarm", strSqlRead);
		qd_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "qdyz_warn", strSqlRead);
		qdyz_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "qdyz_alarm", strSqlRead);
		qdyz_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "zw_warn", strSqlRead);
		zw_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "zw_alarm", strSqlRead);
		zw_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "motortemp_warn", strSqlRead);
		motortemp_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "motortemp_alarm", strSqlRead);
		motortemp_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "zwinc_warn", strSqlRead);
		zwinc_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "zwinc_alarm", strSqlRead);
		zwinc_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "motortempinc_warn", strSqlRead);
		motortempinc_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "motortempinc_alarm", strSqlRead);
		motortempinc_alarm = strSqlRead.toFloat();

		//20210302 yxw add:start
		m_pMySqlHelper->readSqlValue("train_param", "zdzhz_warn", strSqlRead);
		zdzhz_warn = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "zdzhz_alarm", strSqlRead);
		zdzhz_alarm = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "xishuA", strSqlRead);
		zdzhz_ka = strSqlRead.toFloat();
		m_pMySqlHelper->readSqlValue("train_param", "xishuB", strSqlRead);
		zdzhz_kb = strSqlRead.toFloat();
		//20210302 yxw add:end

		algorithmSendMsg(QStringLiteral("Ԥ�������������������"), 0);

#pragma endregion

#pragma region ������౨����
		for (int i = 0; i < 5; i++)
		{
			for (std::vector<double>::iterator it = vibrateDataL[i].begin(); it != vibrateDataL[i].end(); it++)
			{
				switch (i)
				{
				case 0:
					if (*it >= jfg_alarm)
					{
						jfg_alarm_num++;
					}
					else if ((*it < jfg_alarm) && (*it >= jfg_warn))
					{
						jfg_warn_num++;
					}
					break;
				case 1:
					if (*it >= fz_alarm)
					{
						fz_alarm_num++;
					}
					else if ((*it < fz_alarm) && (*it >= fz_warn))
					{
						fz_warn_num++;
					}
				case 2:
					if (*it >= fzyz_alarm)
					{
						fzyz_alarm_num++;
					}
					else if ((*it < fzyz_alarm) && (*it >= fzyz_warn))
					{
						fzyz_warn_num++;
					}
					break;
				case 3:
					if (*it >= qd_alarm)
					{
						fzyz_alarm_num++;
					}
					else if ((*it < qd_alarm) && (*it >= qd_warn))
					{
						qd_warn_num++;
					}
					break;
				case 4:
					if (*it >= qdyz_alarm)
					{
						qdyz_alarm_num++;
					}
					else if ((*it < qdyz_alarm) && (*it >= qdyz_warn))
					{
						qdyz_warn_num++;
					}
				}
			}
		}
#pragma endregion

#pragma region �����Ҳ౨����
		for (int i = 0; i < 5; i++)
		{
			for (std::vector<double>::iterator it = vibrateDataR[i].begin(); it != vibrateDataR[i].end(); it++)
			{
				switch (i)
				{
				case 0:
					if (*it >= jfg_alarm)
					{
						jfg_alarm_num++;
					}
					else if ((*it < jfg_alarm) && (*it >= jfg_warn))
					{
						jfg_warn_num++;
					}
					break;
				case 1:
					if (*it >= fz_alarm)
					{
						fz_alarm_num++;
					}
					else if ((*it < fz_alarm) && (*it >= fzyz_warn))
					{
						fz_warn_num++;
					}
					break;
				case 2:
					if (*it >= fzyz_alarm)
					{
						fzyz_alarm_num++;
					}
					else if ((*it < fzyz_alarm) && (*it >= fzyz_warn))
					{
						fzyz_warn_num++;
					}
					break;
				case 3:
					if (*it >= qd_alarm)
					{
						qd_alarm_num++;
					}
					else if ((*it < qd_alarm) && (*it >= qd_warn))
					{
						qd_warn_num++;
					}
					break;
				case 4:
					if (*it >= qdyz_alarm)
					{
						qdyz_alarm_num++;
					}
					else if ((*it < qdyz_alarm) && (*it >= qdyz_warn))
					{
						qdyz_warn_num++;
					}
					break;
				}
			}

		}

#pragma endregion

// 20210302 yxw add:start
#pragma region ����������ۺ�ֵ������Ԥ����
		int lengthL = vibrateDataL[0].size();
		for (int i = 0; i < lengthL;i++)
		{
			double zdzhzL = vibrateDataL[0][i] * zdzhz_ka + vibrateDataL[1][i] * zdzhz_kb;
			zdzhz_L.push_back(zdzhzL);
			if (zdzhzL >= zdzhz_alarm)
			{
				zdzhz_alarm_num++;
			}
			else if (zdzhzL < zdzhz_alarm && zdzhzL >= zdzhz_warn)
			{
				zdzhz_warn_num++;
			}
		}
#pragma endregion

#pragma region �����Ҳ����ۺ�ֵ������Ԥ����
		int lengthR = vibrateDataR[0].size();
		for (int i = 0; i < lengthR; i++)
		{
			double zdzhzR = vibrateDataR[0][i] * zdzhz_ka + vibrateDataR[1][i] * zdzhz_kb;
			zdzhz_R.push_back(zdzhzR);
			if (zdzhzR >= zdzhz_alarm)
			{
				zdzhz_alarm_num++;
			}
			else if (zdzhzR < zdzhz_alarm && zdzhzR >= zdzhz_warn)
			{
				zdzhz_warn_num++;
			}
		}
#pragma endregion

// 20210302 yxw add:end

#pragma region ����������±�����Ԥ����
		for (std::vector<double>::iterator it = axleDataL[1].begin(); it != axleDataL[1].end(); it++)
		{
			if (*it >= zw_alarm)
			{
				zw_alarm_num++;
			}
			else if ((*it < zw_alarm) && (*it >= zw_warn))
			{
				zw_warn_num++;
			}
		}

#pragma endregion 

#pragma region �������������������Ԥ����
		for (std::vector<double>::iterator it = axleDataL[2].begin(); it != axleDataL[2].end(); it++)
		{
			if (*it >= zwinc_alarm)
			{
				zwinc_alarm_num++;
			}
			else if ((*it < zwinc_alarm) && (*it >= zwinc_warn))
			{
				zwinc_warn_num++;
			}
		}

#pragma endregion 

#pragma region �����Ҳ����±�����Ԥ����
		for (std::vector<double>::iterator it = axleDataR[1].begin(); it != axleDataR[1].end(); it++)
		{
			if (*it >= zw_alarm)
			{
				zw_alarm_num++;
			}
			else if ((*it < zw_alarm) && (*it >= zw_warn))
			{
				zw_warn_num++;
			}
		}

#pragma endregion 

#pragma region �����Ҳ�������������Ԥ����
		for (std::vector<double>::iterator it = axleDataR[2].begin(); it != axleDataR[2].end(); it++)
		{
			if (*it >= zwinc_alarm)
			{
				zwinc_alarm_num++;
			}
			else if ((*it < zwinc_alarm) && (*it >= zwinc_warn))
			{
				zwinc_warn_num++;
			}
		}

#pragma endregion 

#pragma region ����������¶ȱ�����Ԥ����
		for (std::vector<double>::iterator it = electricalDataL[1].begin(); it != electricalDataL[1].end(); it++)
		{
			if (*it >= motortemp_alarm)
			{
				motortemp_alarm_num++;
			}
			else if ((*it < motortemp_alarm) && (*it >= motortemp_warn))
			{
				motortemp_warn_num++;
			}
		}

#pragma endregion

#pragma region ����������¶���������Ԥ����
		for (std::vector<double>::iterator it = electricalDataL[2].begin(); it != electricalDataL[2].end(); it++)
		{
			if (*it >= motortempinc_alarm)
			{
				motortempinc_alarm_num++;
			}
			else if ((*it < motortempinc_alarm) && (*it >= motortempinc_warn))
			{
				motortempinc_warn_num++;
			}
		}

#pragma endregion

#pragma region �����Ҳ����¶ȱ�����Ԥ����
		for (std::vector<double>::iterator it = electricalDataR[1].begin(); it != electricalDataR[1].end(); it++)
		{
			if (*it >= motortemp_alarm)
			{
				motortemp_alarm_num++;
			}
			else if ((*it < motortemp_alarm) && (*it >= motortemp_warn))
			{
				motortemp_warn_num++;
			}
		}

#pragma endregion

#pragma region �����Ҳ����¶���������Ԥ����
		for (std::vector<double>::iterator it = electricalDataR[2].begin(); it != electricalDataR[2].end(); it++)
		{
			if (*it >= motortempinc_alarm)
			{
				motortempinc_alarm_num++;
			}
			else if ((*it < motortempinc_alarm) && (*it >= motortempinc_warn))
			{
				motortempinc_warn_num++;
			}

		}

#pragma endregion

		//�������ֵ
		zw_max = (*std::max_element(axleDataL[1].begin(), axleDataL[1].end()) > *std::max_element(axleDataR[1].begin(), axleDataR[1].end()) ?
			*std::max_element(axleDataL[1].begin(), axleDataL[1].end()) : *std::max_element(axleDataR[1].begin(), axleDataR[1].end()));
		//z���������ֵ
		zwinc_max = (*std::max_element(axleDataL[2].begin(), axleDataL[2].end()) > *std::max_element(axleDataR[2].begin(), axleDataR[2].end()) ?
			*std::max_element(axleDataL[2].begin(), axleDataL[2].end()) : *std::max_element(axleDataR[2].begin(), axleDataR[2].end()));
		//����¶����ֵ
		motortemp_max = (*std::max_element(electricalDataL[1].begin(), electricalDataL[1].end()) > *std::max_element(electricalDataR[1].begin(), electricalDataR[1].end()) ?
			*std::max_element(electricalDataL[1].begin(), electricalDataL[1].end()) : *std::max_element(electricalDataR[1].begin(), electricalDataR[1].end()));
		//����������ֵ
		motortempinc_max = (*std::max_element(electricalDataL[2].begin(), electricalDataL[2].end()) > *std::max_element(electricalDataR[2].begin(), electricalDataR[2].end()) ?
			*std::max_element(electricalDataL[2].begin(), electricalDataL[2].end()) : *std::max_element(electricalDataR[2].begin(), electricalDataR[2].end()));
		//���ۺ�ֵ���ֵ
		zdzhz_max = (*std::max_element(zdzhz_L.begin(), zdzhz_L.end()) > *std::max_element(zdzhz_R.begin(), zdzhz_R.end()) ?
			*std::max_element(zdzhz_L.begin(), zdzhz_L.end()) : *std::max_element(zdzhz_R.begin(), zdzhz_R.end()));

		int state = 4;
		// 20210302 yxw modify:start
		if (zdzhz_alarm_num > 0 || zw_alarm_num > 0 || motortemp_alarm_num > 0 || zwinc_alarm_num > 0 || motortempinc_alarm_num > 0)
		{
			state = 1;
		}
		else if (zdzhz_warn_num > 0 || zw_warn_num > 0 || motortemp_warn_num > 0 || zwinc_warn_num > 0 || motortempinc_warn_num > 0)
		{
			state = 2;
		}
		// 20210302 yxw modify:end



		algorithmSendMsg(QStringLiteral("���ݿ�򿪳ɹ�"), 0);
		QStringList key, value;
		key << "line_id" << "train_id" << "train_station" << "train_direction" << "train_state" << "train_speed" << "train_date";
		value << m_strLineNumber << m_strTrainNumber << m_strStation << m_strMainControlPort << QString::number(state) << QString::number(m_dTrainSpeed) << m_strTrainRunNumber;
		bool ret = m_pMySqlHelper->writeSqlData("train_info", key, value);
		if (ret)
		{
			algorithmSendMsg(QStringLiteral("train_info���ݱ�д��ɹ�"), 0);
		}
		else
		{
			algorithmSendMsg(QStringLiteral("train_info���ݱ�д��ʧ��"), 1);
		}
		QString m_strOnlyID = "";
		ret = m_pMySqlHelper->readSqlValue("train_info", "train_onlyid", m_strOnlyID);
		if (ret)
		{
			algorithmSendMsg(QStringLiteral("train_onlyid��ȡ�ɹ�"), 0);
		}
		else
		{
			algorithmSendMsg(QStringLiteral("train_onlyid��ȡʧ��"), 1);
		}

		key.clear(); value.clear();
		key << "train_onlyid" << "fz_warn" << "fz_alarm" << "jfg_warn" << "jfg_alarm" << "fzyz_warn" <<
			"fzyz_alarm" << "qd_warn" << "qd_alarm" << "qdyz_warn" << "qdyz_alarm" << "zx_temp" <<
			"motor_temp" << "zx_tempinc" << "motor_tempinc" <<"train_date";
		value << m_strOnlyID << QString::number(fz_warn_num) << QString::number(fz_alarm_num) << QString::number(jfg_warn_num) <<
			QString::number(jfg_alarm_num) << QString::number(fzyz_warn_num) << QString::number(fzyz_alarm_num) << QString::number(qd_warn_num) <<
			QString::number(qd_alarm_num) << QString::number(qdyz_warn_num) << QString::number(qdyz_alarm_num) <<
			QString::number(zw_max) << QString::number(motortemp_max) << QString::number(zwinc_max) <<
			QString::number(motortempinc_max) << m_strTrainRunNumber;
		ret = m_pMySqlHelper->writeSqlData("train_data", key, value);
		if (ret)
		{
			algorithmSendMsg(QStringLiteral("train_data���ݱ�д��ɹ�"), 0);
		}
		else
		{
			algorithmSendMsg(QStringLiteral("train_data���ݱ�д��ʧ��"), 1);
		}


		if (zdzhz_alarm_num > 0)
		{
			key.clear(); value.clear();
			key << "train_onlyid" << "fault_type" << "fault_rank" << "train_date";
			value << m_strOnlyID << "1" << "1" << m_strTrainRunNumber;
			ret = m_pMySqlHelper->writeSqlData("train_fault", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��1��1�ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ��һ��д��1��1ʧ��"), 1);
			}

		}
		else if (zdzhz_warn_num > 0)
		{
			key.clear(); value.clear();
			key << "train_onlyid" << "fault_type" << "fault_rank" << "train_date";
			value << m_strOnlyID << "1" << "2" << m_strTrainRunNumber;
			ret = m_pMySqlHelper->writeSqlData("train_fault", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��1��2�ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��1��2ʧ��"), 1);
			}
		}

		if (zw_alarm_num > 0 || zwinc_alarm_num > 0)
		{
			key.clear(); value.clear();
			key << "train_onlyid" << "fault_type" << "fault_rank" << "train_date";
			value << m_strOnlyID << "2" << "1" << m_strTrainRunNumber;
			ret = m_pMySqlHelper->writeSqlData("train_fault", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��2��1�ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��2��1ʧ��"), 1);
			}
		}
		else if (zw_warn_num > 0 || zwinc_warn_num > 0)
		{
			key.clear(); value.clear();
			key << "train_onlyid" << "fault_type" << "fault_rank" << "train_date";
			value << m_strOnlyID << "2" << "2" << m_strTrainRunNumber;
			ret = m_pMySqlHelper->writeSqlData("train_fault", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��2��2�ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��2��2ʧ��"), 1);
			}
		}

		if (motortemp_alarm_num > 0 || motortempinc_alarm_num > 0)
		{
			key.clear(); value.clear();
			key << "train_onlyid" << "fault_type" << "fault_rank" << "train_date";
			value << m_strOnlyID << "3" << "1" << m_strTrainRunNumber;
			ret = m_pMySqlHelper->writeSqlData("train_fault", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��3��1�ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��3��1ʧ��"), 1);
			}

		}
		else if (motortemp_warn_num > 0 || motortempinc_warn_num > 0)
		{
			key.clear(); value.clear();
			key << "train_onlyid" << "fault_type" << "fault_rank" << "train_date";
			value << m_strOnlyID << "3" << "2" << m_strTrainRunNumber;
			ret = m_pMySqlHelper->writeSqlData("train_fault", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��3��2�ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("train_fault���ݱ�д��3��2ʧ��"), 1);
			}
		}

		QStringList qsl;
		QStringList qsl1;
		qsl << "IA" << "IB" << "IC" << "IIC" << "IIB" << "IIA";
		qsl1 << "IIA" << "IIB" << "IIC" << "IC" << "IB" << "IA";

		//����ż����ȡ�泵
		key.clear();
		key << "train_onlyid" << "carriage_num" << "wheel_num" << "jfg_value" << "fz_value" << "fzyz_value" <<
			"qd_value" << "qdyz_value" << "zx_temp" << "zx_tempinc" << "train_date" << "hj_temp";
		for (int i = 0; i < 24; i++)
		{
			value.clear();
			value << m_strOnlyID;
			int y = i / 8;
			int z = i / 2;
			int wheel_num = (i % 8) + 1;    // ���ֺ�

			if (i % 2 == 0)
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}

				value << QString::number(wheel_num) << QString::number(vibrateDataL[0][z]) <<
					QString::number(vibrateDataL[1][z]) << QString::number(vibrateDataL[2][z]) <<
					QString::number(vibrateDataL[3][z]) << QString::number(vibrateDataL[4][z]) <<
					QString::number(axleDataL[0][z]) << QString::number(axleDataL[2][z]) << m_strTrainRunNumber
					<< QString::number(m_dEnvironmentTemp);;
			}
			else
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}
				value << QString::number(wheel_num) << QString::number(vibrateDataR[0][z]) <<
					QString::number(vibrateDataR[1][z]) << QString::number(vibrateDataR[2][z]) <<
					QString::number(vibrateDataR[3][z]) << QString::number(vibrateDataR[4][z]) <<
					QString::number(axleDataR[0][z]) << QString::number(axleDataR[2][z]) << m_strTrainRunNumber
					<< QString::number(m_dEnvironmentTemp);
			}
			bool ret = m_pMySqlHelper->writeSqlData("vt_info", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("vt_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("vt_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ʧ��"), 1);
			}
		}

		//����ż����ȡż��
		for (int i = 24; i < 48; ++i)
		{
			value.clear();
			value << m_strOnlyID;
			int y = i / 8;
			int z = i / 2;
			int wheel_num = 8 - (i % 8);    // ���ֺ�
			if (i % 2 == 0)
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}

				value << QString::number(wheel_num) << QString::number(vibrateDataL[0][z]) <<
					QString::number(vibrateDataL[1][z]) << QString::number(vibrateDataL[2][z]) <<
					QString::number(vibrateDataL[3][z]) << QString::number(vibrateDataL[4][z]) <<
					QString::number(axleDataL[0][z]) << QString::number(axleDataL[2][z]) << m_strTrainRunNumber
					<< QString::number(m_dEnvironmentTemp);
			}
			else
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}
				value << QString::number(wheel_num) << QString::number(vibrateDataR[0][z]) <<
					QString::number(vibrateDataR[1][z]) << QString::number(vibrateDataR[2][z]) <<
					QString::number(vibrateDataR[3][z]) << QString::number(vibrateDataR[4][z]) <<
					QString::number(axleDataR[0][z]) << QString::number(axleDataR[2][z]) << m_strTrainRunNumber
					<< QString::number(m_dEnvironmentTemp);
			}

			bool ret = m_pMySqlHelper->writeSqlData("vt_info", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("vt_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("vt_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ʧ��"), 1);
			}
		}

		key.clear();
		key << "train_onlyid" << "carriage_num" << "motor_num" << "motor_temp" << "motor_tempinc" << "train_date";

		//����ż����ȡ�泵
		for (int i = 0; i < 8; ++i)
		{
			value.clear();
			value << m_strOnlyID;

			int y = i / 4 + 1;
			int z = i / 2;
			int motor_num = (i % 4) + 1;    // �����
			if (i % 2 == 0)
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}
				value << QString::number(motor_num) << QString::number(electricalDataR[1][z]) <<
					QString::number(electricalDataR[2][z]) << m_strTrainRunNumber;
			}
			else
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}
				value << QString::number(motor_num) << QString::number(electricalDataL[1][z]) <<
					QString::number(electricalDataL[2][z]) << m_strTrainRunNumber;
			}
			bool ret = m_pMySqlHelper->writeSqlData("motor_info", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("motor_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("motor_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ʧ��"), 1);
			}

		}

		//����ż����ȡż��
		for (int i = 8; i < 16; ++i)
		{
			value.clear();
			value << m_strOnlyID;

			int y = i / 4 + 1;
			int z = i / 2;
			int motor_num = 4 - (i % 4);    //�����

			if (i % 2 == 0)
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}
				value << QString::number(motor_num) << QString::number(electricalDataR[1][z]) <<
					QString::number(electricalDataR[2][z]) << m_strTrainRunNumber;
			}
			else
			{
				if (m_strMainControlPort.toInt() % 2 == 1)
				{
					value << qsl[y];
				}
				else
				{
					value << qsl1[y];
				}
				value << QString::number(motor_num) << QString::number(electricalDataL[1][z]) <<
					QString::number(electricalDataL[2][z]) << m_strTrainRunNumber;
			}
			bool ret = m_pMySqlHelper->writeSqlData("motor_info", key, value);
			if (ret)
			{
				algorithmSendMsg(QStringLiteral("motor_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ɹ�"), 0);
			}
			else
			{
				algorithmSendMsg(QStringLiteral("motor_info���ݱ���") + QString::number(i) + QStringLiteral("��д��ʧ��"), 1);
			}
		}
			

		if (m_pMySqlHelper->closeSql())
		{
			emit algorithmSendMsg(QStringLiteral("���ݿ�رգ�"), 0);
		}
		else
		{
			emit algorithmSendMsg(QStringLiteral("���ݿ�ر�ʧ�ܣ�"), 1);
		}

		/********************************************/
		{
			//-----����(json�ж���)��c++�ж�Ӧ�������------
			//���ۺ�ֵ��zdzhz_R��zdzhz_L
			//�����¶ȣ�axleDataR[0],axleDataL[0] ----0�����ֵ��1�����ֵ������ֵ��json
			//����¶ȣ�electricalDataR[0], electricalDataL[0] ----0�����ֵ��1�����ֵ������ֵ��json
			//�����¶�(ambientTemperature)��m_dEnvironmentTemp
			//�ٶ�(speed)��m_dTrainSpeed
			//���ʱ��(checkTime)��m_strTrainRunNumber  ----�г�������ˮ������ʱ�������ģ�checkTime;�衰20201121125815��ת����"2022-05-21 18:35:47"��ʽ
			//��¼��(record_id)��m_strTrainRunNumber  ----�г�������ˮ������ʱ�������ģ�record_id:ֱ��ʹ��
			//���ضˣ�m_strMainControlPort.toInt() ---- 0��ż���ˣ�1�������ˣ��ò�������json��ֻ���ڳ���ŵĴ���
			//����(groupName)��m_strTrainNumber -----��ʽ��"005006"������json�ĸ�ʽΪ"09005006"
			//�����(carriageNumber)����������ض˺ͳ��Ž��д�����ʽ"09A005"
			//��·�ţ�m_strLineNumber

			setAlarmData(motortemp_alarm, motortemp_warn, zdzhz_alarm, zdzhz_warn, zw_alarm, zw_warn);

			struct VIBRATE_DATA data;
			struct VIBRATE_DATA *pData = &data;
			setVibData(pData, zdzhz_L, zdzhz_R, axleDataR, axleDataL, electricalDataR, electricalDataL);

			vibrateDataToJson(&data);
		}
		/**********************************************/
		
	}
	else
	{
		algorithmSendMsg(QStringLiteral("���ݿ��ʧ��"), 1);
	}

}

void AlgorithmBase::readDataFromTxt(QString strPath, std::vector<double> &vec1)
{
	m_Semaphore.acquire(1);

	QFile file(strPath);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		algorithmSendMsg(strPath + QStringLiteral("�ļ����ݶ�ȡʧ��"), 1);
	}
	else
	{
		QTextStream in(&file);
		QString line = in.readLine();
		while (!line.isNull())
		{
			vec1.push_back(line.toFloat());
			line = in.readLine();
		}
	}
	m_Semaphore.release(1);
}

void AlgorithmBase::simulateTrigger(QString strTrainNumber)
{
	algorithmSendMsg(QStringLiteral("���ڶ�ȡ�������..."), 0);
	initialData();
	m_strFileSavePathRoot.replace("/", "\\");
	QString  strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration1.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v1); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration2.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v2); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration3.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v3); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration4.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v4); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration5.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v5); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration6.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v6); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration7.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v7); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\vibration8.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, v8); });

	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\magnetic1.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, g1); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\magnetic2.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, g2); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\magnetic3.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, g3); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\magnetic4.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, g4); });

	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\temperate1.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, t1); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\temperate2.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, t2); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\temperate3.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, t3); });
	strfialePath = m_strFileSavePathRoot + "simulate\\" + strTrainNumber + "\\VAIS\\temperate4.txt";
	QtConcurrent::run([=](){ readDataFromTxt(strfialePath, t4); });

	m_Semaphore.acquire(16);

	if (!v1.size() || !v2.size() || !v3.size() || !v4.size() || !v5.size() || !v6.size() || !v7.size() || !v8.size()
		|| !g1.size() || !g2.size() || !g3.size() || !g4.size() || !t1.size() || !t2.size() || !t3.size() || !t4.size())
	{
		algorithmSendMsg(QStringLiteral("��������Ϊ�գ���������Դ"), 1);
		m_Semaphore.release(16);
		return;
	}
	m_Semaphore.release(16);
	operateData(v1, v2, v3, v4, v5, v6, v7, v8, g1, g2, g3, g4, t1, t2, t3, t4);
}

void AlgorithmBase::dataCombination()
{
	algorithmSendMsg(QStringLiteral("���������"), 0);

#pragma region ������ݣ���ȱʧ���ݽ��в�ȫ
	for (int i = 0; i < vibrationDataLeft.size(); i++)
	{
		if (vibrationDataLeft[i].size() < 24)
		{
			int n = 24 - vibrationDataLeft[i].size();
			for (int j = 0; j < n; j++)
			{
				vibrationDataLeft[i].push_back(-1000);
			}
		}

		if (vibrationDataRight[i].size() < 24)
		{
			int n = 24 - vibrationDataRight[i].size();
			for (int j = 0; j < n; j++)
			{
				vibrationDataRight[i].push_back(-1000);
			}
		}
	}

	for (int i = 0; i < axleDataLeft.size(); i++)
	{
		if (axleDataLeft[i].size() < 24)
		{
			int n = 24 - axleDataLeft[i].size();
			for (int j = 0; j < n; j++)
			{
				axleDataLeft[i].push_back(-1000);
			}
		}

		if (axleDataRight[i].size() < 24)
		{
			int n = 24 - axleDataRight[i].size();
			for (int j = 0; j < n; j++)
			{
				axleDataRight[i].push_back(-1000);
			}
		}
	}

	for (int i = 0; i < electricalDataLeft.size(); i++)
	{
		if (electricalDataLeft[i].size() < 8)
		{
			int n = 8 - electricalDataLeft[i].size();
			for (int j = 0; j < n; j++)
			{
				electricalDataLeft[i].push_back(-1000);
			}
		}

		if (electricalDataRight[i].size() < 8)
		{
			int n = 8 - electricalDataRight[i].size();
			for (int j = 0; j < n; j++)
			{
				electricalDataRight[i].push_back(-1000);
			}
		}
	}
#pragma endregion

	algorithmSendMsg(QStringLiteral("֪ͨ������ʾ����"), 0);
	emit sendVibrationData();
	emit sendAxleData();
	emit sendElectricalData();
	algorithmSendMsg(QStringLiteral("��ʼд�����ݿ�"), 0);
	writeDataToSql(vibrationDataLeft, axleDataLeft, electricalDataLeft, vibrationDataRight, axleDataRight, electricalDataRight);
	m_bIsRunAlg = false;
}

AlgorithmBase* AlgorithmBase::instance = NULL;
DEALALGORITHM_EXPORT AlgorithmBase* getAlgorithmInstance()
{
	if (NULL == AlgorithmBase::instance)
	{
		AlgorithmBase::instance = new AlgorithmBase();
	}
	return AlgorithmBase::instance;
}
