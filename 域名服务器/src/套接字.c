/*
 * �׽���.c
 *
 *  Created on: 2016��6��9��
 *      Author: Lanky
 */
#include "DNSServer.h"

/**
 * �����׽���
 */
int cmStartSocket() {
	SOCKET SOCKETServer; //skLocalSocket���
	WSADATA WSADATAData; //�洢��WSAStartup�������ú󷵻ص�Windows �汾��Ϣ
	struct sockaddr_in Ssockaddr_inServer; //�׽��ֵ�ַ�ṹ�� �����
	struct sockaddr_in Ssockaddr_inClient; //�׽��ֵ�ַ�ṹ�� �ͻ���
	int nLength = sizeof(Ssockaddr_inClient); //�׽�����Ϣ�ṹ�� ��С

	/*�����׽���*/
	{
		if (WSAStartup(MAKEWORD(2, 1), &WSADATAData)) { //�����׽���(����˫�ֽ�(���汾��, ���汾��), ���ذ汾��Ϣ)
			printf("�׽�������ʧ��\n");
			WSACleanup(); //��ֹ�׽���
			return EXIT_FAILURE; //��������ʧ��
		}
		printf("������������...\n");
	}

	/*��ʼ���׽��ֵ�ַ*/
	{
		Ssockaddr_inServer.sin_family = AF_INET; //Э����
		Ssockaddr_inServer.sin_port = htons(12345); //��Ҫ�����Ķ˿�
		Ssockaddr_inServer.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ
		SOCKETServer = socket(AF_INET, SOCK_DGRAM, 0); //��ȡ�׽��־��( ��ַ����, �׽�������, �׽���Э��)
		bind(SOCKETServer, (struct sockaddr*) &Ssockaddr_inServer,
				sizeof(Ssockaddr_inServer)); //�׽��ֺͶ˿�����
	}

	/*ѭ��������Ϣ*/
	while (TRUE) {
		char pcBuffer[1024] = "/0"; //�ַ�����

		/*����UDP���ݰ�(�׽��־��, ���������ݻ�������ַ, ��������С, ������ʽ, Ŀ���׽��ֵ�ַ, �׽��ֵ�ַ����)
		 * �������Զ��������������ݱ�һֱ�ȴ�*/
		if (recvfrom(SOCKETServer, pcBuffer, sizeof(pcBuffer), 0,
				(struct sockaddr*) &Ssockaddr_inClient,
				&nLength) != SOCKET_ERROR) {
			/*inet_ntoa��ת���ַ���Ϊip��ַ
			 * printf(ip��ַ, ���ܵ�����Ϣ)*/
			printf("���յ�����Ϊ: [%s]---%s\n",
					inet_ntoa(Ssockaddr_inClient.sin_addr), pcBuffer);
			/*����UDP���ݰ�(�׽��־��, ���������ݻ�������ַ, ��������С, ������ʽ, Ŀ���׽��ֵ�ַ, �׽��ֵ�ַ����)*/
			sendto(SOCKETServer, pcBuffer, sizeof(pcBuffer), 0,
					(struct sockaddr*) &Ssockaddr_inClient, nLength);
		}
	}

	/*�رշ���*/
	{
		closesocket(SOCKETServer);			//�ͷ��׽��־��
		return EXIT_SUCCESS;			//��������
	}
}

