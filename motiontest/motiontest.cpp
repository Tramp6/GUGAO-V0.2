                                                                                                                                         // motiontest.cpp : �������̨Ӧ�ó������ڵ㡣
//
#pragma comment(lib,"gts.lib")
#include "stdafx.h"
#include "conio.h"
#include "gts.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


#define DLT_FAILURE					-1
#define DLT_SUCCESS					0
HANDLE hCom;  //��������ڳ�ʼ������

DWORD WINAPI ThreadWrite(LPVOID lpParameter)//����1
{
	char outputData[100] = { 0x00 };//������ݻ���

	if (hCom == INVALID_HANDLE_VALUE)
	{
		puts("�򿪴���ʧ��\n");
		return 0;
	}
	unsigned char s_str0[1] = { 0x05 };
	unsigned char s_str1[3][4] = { { 0x00,0x00,0xD2,0x2E },{ 0x00, 0x01, 0xD2, 0x2D },{ 0x00, 0x02, 0xD2, 0x2C } };
	unsigned char s_str2[1] = { 0x04 };
	unsigned char s_str3[1] = { 0x06 };
	DWORD strLength_1 = 1;
	DWORD strLength_2 = 4;
	int sleeptime = 100;
	int k = 0;

	while (k <= 15)
	{
		for (int i = 0; i <= 2; i++)
		{
			WriteFile(hCom, s_str0, strLength_1, &strLength_1, NULL); // ���ڷ����ַ���
			Sleep(sleeptime);
			WriteFile(hCom, s_str1[i], strLength_2, &strLength_2, NULL); // ���ڷ����ַ���
																		 //printf("�������� %02x %02x %02x %02x\n", s_str1[0], s_str1[1], s_str1[2], s_str1[3]);	
			Sleep(sleeptime);
			WriteFile(hCom, s_str2, strLength_1, &strLength_1, NULL); // ���ڷ����ַ���
			Sleep(sleeptime);
			WriteFile(hCom, s_str3, strLength_1, &strLength_1, NULL); // ���ڷ����ַ���
			Sleep(sleeptime);
		}

		fflush(stdout);
		PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);            //��ջ�����
																   //Sleep(500);
		k = k + 1;
	}
	return 0;
}

DWORD WINAPI ThreadRead(LPVOID lpParameter)
{
	if (hCom == INVALID_HANDLE_VALUE)   //INVALID_HANDLE_VALUE��ʾ����������GetLastError
	{
		puts("�򿪴���ʧ��");
		return 0;
	}

	unsigned char getputData[100] = { 0x00 };//�������ݻ���
											 // ���ô�����Ϣ����ȡ���봮�ڻ��������ݵ��ֽ���
	DWORD dwErrors;     // ������Ϣ
	COMSTAT Rcs;        // COMSTAT�ṹͨ���豸�ĵ�ǰ��Ϣ
	DWORD length = 15;               //�������ն�ȡ���ֽ���
	int Len = 15;
	int sleeptime1 = 50;
	int axis_data[3][3][5] = { 0x00 };
	int axis_1, axis_2, axis_3;
	axis_1 = axis_2 = axis_3 = 0;
	//(axis_1<=2)&& (axis_2 <= 2) &&(axis_1 <= 4) && (axis_2 <= 4) && (axis_3 <= 4)
	while (1)
	{	

		if ((axis_data[0][2][2]==0x00) && (axis_data[1][2][2] == 0x00)&& (axis_data[2][2][2] == 0x00))
		{
			for (int i = 0; i<15; i++)
			{
				getputData[i] = 0;
			}

			ClearCommError(hCom, &dwErrors, &Rcs);                                // ��ȡ�����������ݳ���
			Len = Rcs.cbInQue;
			ReadFile(hCom, getputData, Len, &length, NULL);   //��ȡ�ַ���
			PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);  //��ջ�����

			if (getputData[0] == 0x0B && getputData[2] == 0xD2 && getputData[3] == 0x0B && (getputData[9] != 0x00) && (getputData[8] != 0x00) && (getputData[7] != 0x00))
			{
				if (getputData[1] == 0x00)
				{
					printf("��1��Ȧ���� %02x  %02x %02x ", getputData[9], getputData[8], getputData[7]);
					printf("��1��Ȧ���� %02x %02x \n", getputData[11], getputData[10]);
					for (int i = 0; i <= 2; i++)
					{
						axis_data[0][axis_1][0] = getputData[9];
						axis_data[0][axis_1][1] = getputData[8];
						axis_data[0][axis_1][2] = getputData[7];
						axis_data[0][axis_1][3] = getputData[11];
						axis_data[0][axis_1][4] = getputData[10];
					}
					axis_1 = axis_1 + 1;
				}
				else if (getputData[1] == 0x01)
				{
					printf("��2��Ȧ���� %02x  %02x %02x ", getputData[9], getputData[8], getputData[7]);
					printf("��2��Ȧ���� %02x %02x \n", getputData[11], getputData[10]);
					for (int i = 0; i <= 2; i++)
					{
						axis_data[1][axis_2][0] = getputData[9];
						axis_data[1][axis_2][1] = getputData[8];
						axis_data[1][axis_2][2] = getputData[7];
						axis_data[1][axis_2][3] = getputData[11];
						axis_data[1][axis_2][4] = getputData[10];
					}
					axis_2 = axis_2 + 1;
				}
				else if (getputData[1] == 0x02)
				{
					printf("��3��Ȧ���� %02x  %02x %02x ", getputData[9], getputData[8], getputData[7]);
					printf("��3��Ȧ���� %02x %02x \n", getputData[11], getputData[10]);
					for (int i = 0; i <= 2; i++)
					{
						axis_data[2][axis_3][0] = getputData[9];
						axis_data[2][axis_3][1] = getputData[8];
						axis_data[2][axis_3][2] = getputData[7];
						axis_data[2][axis_3][3] = getputData[11];
						axis_data[2][axis_3][4] = getputData[10];
					}
					axis_3 = axis_3 + 1;
				}
			}

			fflush(stdout);
			Sleep(sleeptime1);
		}
		else
		{
			break;
		}



	}

	FILE *fp;
	fopen_s(&fp, "axisdata.txt", "w");//���ļ��Ա�д������
	for (int i = 0; i <= 2; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			axis_data[i][j][0] = (axis_data[i][j][0] << 16) + (axis_data[i][j][1] << 8) + axis_data[i][j][2];
			axis_data[i][j][1] = (axis_data[i][j][3] << 8) + axis_data[i][j][4];
		}
		axis_data[i][0][0] = int((axis_data[i][0][0] + axis_data[i][1][0] + axis_data[i][2][0]) / 3);
		axis_data[i][0][1] = int((axis_data[i][0][1] + axis_data[i][1][1] + axis_data[i][2][1]) / 3);
		fprintf(fp, "%06x ", axis_data[i][0][0]);
		fprintf(fp, "%04x ", axis_data[i][0][1]);
	}
	fclose(fp);
	return 0;
}

int Serial_port()
{
	// ��ʼ������
	hCom = CreateFile(TEXT("COM3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// ��ȡ�����ô��ڲ���
	DCB myDCB;
	GetCommState(hCom, &myDCB);  // ��ȡ���ڲ���
	printf("Baud rate is %d\n", (int)myDCB.BaudRate);
	fflush(stdout);
	myDCB.BaudRate = 9600;       // ������
	myDCB.Parity = NOPARITY;   // У��λ
	myDCB.ByteSize = 8;          // ����λ
	myDCB.StopBits = ONESTOPBIT; // ֹͣλ
	SetCommState(hCom, &myDCB);  // ���ô��ڲ���

								 // �̴߳���
	HANDLE HRead, HWrite;
	HWrite = CreateThread(NULL, 0, ThreadWrite, NULL, 0, NULL);
	HRead = CreateThread(NULL, 0, ThreadRead, NULL, 0, NULL);

	int sleep_sec = 5;
	//while (1) { ; }

	Sleep(sleep_sec * 1000);
	CloseHandle(HRead);
	CloseHandle(HWrite);

	//�ļ���ȡ����
	int data[6];
	int i = 0;
	FILE *fp;
	fopen_s(&fp, "axisdata.txt", "r");//���ļ�
	if (fp == NULL) //���ļ�����
		return -1;
	while (fscanf_s(fp, "%x", &data[i]) != EOF) //��ȡ���ݵ����飬ֱ���ļ���β(����EOF)
		i++;
	fclose(fp);//�ر��ļ�

	printf("��1��Ȧ����%d\t,��Ȧ����%d\n��2��Ȧ����%d\t,��Ȧ����%d\n��3��Ȧ����%d\t,��Ȧ����%d\n ", data[0], data[1], data[2], data[3], data[4], data[5]);
	return data[0], data[1], data[2], data[3], data[4], data[5];
}


int main(int argc, char* argv[])
{
	printf("��ʼ�������Ե�\n");
	int axis_single_data[3];
	int axis_multiple_data[3];

	axis_single_data[0], axis_multiple_data[0], axis_single_data[1], axis_multiple_data[1],
		axis_single_data[2], axis_multiple_data[2] = Serial_port();

	printf("��ʼ�����\n");
	


	const int AxisNumber[3] = { 1, 2, 3 };
	long sts[3];
	double enc[3];
	int m_LastRtn;
	int m_LastDltErr;
	m_LastRtn =  GT_GetEncPos( 1, &enc[0], 8);
	for (int i = 0; i<3; ++i)
	{
		printf("%8.0lf", enc[i]);
	}
	printf("\r");
	//Open GT
	m_LastRtn = GT_Open();//GTN_OpenRingNet?
	if (m_LastRtn)
		return -5;

	//Reset Controller
	m_LastRtn = GT_Reset();
	if (m_LastRtn)
		return -2;

	//DownLoad Configuration
	m_LastRtn = GT_LoadConfig("999.cfg");//FILENAME IS CHANGED
	if (m_LastRtn)
		return -3;


	//Clear All Axis Alarm and Limit
	m_LastRtn = GT_ClrSts(1, 8);//���3����״̬(�ܶ������8)
	if (m_LastRtn)
		return -4;

	//Servo pid--Open Brake--Servo On

	TPid pid; // pid struct
	TTrapPrm Trap;// acc-vel struct
	for (int i = 0; i < 3; i++)//����������
	{
		//if(BrakeIO[i] != -1)//ԭ������ͨ����բ��Ϣ�ж��Ƿ�Ϊ�ŷ���� 
		//{
		//read PID
		m_LastRtn = GT_GetPid(AxisNumber[i], 1, &pid);//��ȡpid���� short GTN_GetPid(short core, short control, short index, TPid *pPid) indexΪpid�����������ţ�Ĭ��Ϊ1��
		if (m_LastRtn)
			return -6;

		if (i == 1)//PID������ֵ
		{
			pid.kp = 0.7;
			pid.kd = 35;
			//pid.kvff=97;
		}
		else if (i == 2)
		{
			pid.kp = 0.7;
			pid.kd = 35;
			//pid.kvff=97;
		}
		else
		{
			pid.kp = 0.7;
			pid.kd = 35;
			//pid.kvff=97;
		}

		//update PID   setpid
		m_LastRtn = GT_SetPid(AxisNumber[i], 3, &pid);//short GTN_SetPid(short core, short control, short index, TPid *pPid)
		if (m_LastRtn)
			return -7;

		//set error band  ����������
		m_LastRtn = GT_SetAxisBand(AxisNumber[i], 1000, 1);//����δ��
		if (m_LastRtn)
			return -8;

		//set axis "dianwei" mode  
		m_LastRtn = GT_PrfTrap(AxisNumber[i]);
		if (m_LastRtn)
			return -9;
		// read  dianwei mode parameter
		m_LastRtn = GT_GetTrapPrm(AxisNumber[i], &Trap);
		if (m_LastRtn)
			return -10;

		Trap.acc = 0.1;
		Trap.dec = 0.1;
		Trap.velStart = 3;

		m_LastRtn = GT_SetTrapPrm(AxisNumber[i], &Trap);
		if (m_LastRtn)
			return -11;

		m_LastRtn = GT_SetVel(AxisNumber[i], 5);
		if (m_LastRtn)
			return -12;

		//set error band
		m_LastRtn = GT_SetAxisBand(AxisNumber[i], 20, 1);
		if (m_LastRtn)
			return -13;

		//open axis motor sever on
		m_LastRtn = GT_AxisOn(AxisNumber[i]);
		if (m_LastRtn)
			return -14;




	}
	// clean planning position and actual position

	m_LastRtn = GT_ZeroPos(AxisNumber[0], 3);
	if (m_LastRtn)
		return -15;


	//����Ŀ��λ���Լ��ٶ�
	for (int i = 0; i < 3; i++)
	{
		m_LastRtn = GT_SetPos(AxisNumber[i], 30000);//set position 
		if (m_LastRtn)
			return -16;
		m_LastRtn = GT_SetVel(AxisNumber[i], 10);
		if (m_LastRtn)
			return -17;

		//������λģʽ
		m_LastRtn = GT_Update(
			((1 << (AxisNumber[i] - 1))));
		if (m_LastRtn)
			return -18;
	}
		//�ȴ���λģʽ����
		do
		{
			m_LastRtn = GT_GetSts(AxisNumber[0], &sts[0], 3);
			if (m_LastRtn)
				return -19;
		} while (!(sts[0] & 0x400));
		return DLT_SUCCESS;
		do
		{
			m_LastRtn = GT_GetSts(AxisNumber[1], &sts[1], 3);
			if (m_LastRtn)
				return -19;
		} while (!(sts[1] & 0x400));
		return DLT_SUCCESS;
		do
		{
			m_LastRtn = GT_GetSts(AxisNumber[2], &sts[2], 3);
			if (m_LastRtn)
				return -19;
		} while (!(sts[2] & 0x400));
		return DLT_SUCCESS;

	for (int i = 0; i < 3; i++)
	{
		m_LastRtn = GT_AxisOff(AxisNumber[i]);
		if (m_LastRtn)
			return -20;
	}
	system("pause");
	//Close GT
	m_LastRtn = GT_Close();
	if (m_LastRtn)
		return -21;



	return DLT_SUCCESS;
	
}