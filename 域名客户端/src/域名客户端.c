/*
 * �����ͻ���.c
 *
 *  Created on: 2016��6��9��
 *      Author: Lanky
 */
#include<stdio.h>//IO����
#include<stdlib.h>///������������
#include<Winsock2.h>//�׽��ֺ���

/**
 * �����׽���
 */
int cmStartSocket();

/**
 * ������
 * argc����������
 * argv�����������ַ
 */
int main(int agrc, char*argv[]) {
	cmStartSocket();
	return EXIT_SUCCESS;		//��������
}

/**
 * �����׽���
 */
int cmStartSocket() {
	SOCKET SOCKETClient; //SOCKET���
	WSADATA WSADATAData; //�洢��WSAStartup�������ú󷵻ص�Windows Sockets����
	const char* c_pcIp = "127.0.0.1"; //��Ҫ�󶨵����ص��ĸ�IP��ַ
	struct sockaddr_in Ssockaddr_inClient; //�׽��ֵ�ַ�ṹ��
	int nLength = sizeof(Ssockaddr_inClient); //�׽�����Ϣ�ṹ�� ��С

	/*�����׽���*/
	{
		if (WSAStartup(MAKEWORD(2, 1), &WSADATAData)) { //�����׽���(����˫�ֽ�(���汾��, ���汾��), ���ذ汾��Ϣ)
			printf("�׽�������ʧ��\n");
			WSACleanup(); //��ֹ�׽���
			return EXIT_FAILURE; //��������ʧ��
		}
		printf("���������ͻ���...\n");
	}

	/*��ʼ���׽��ֵ�ַ*/
	{
		Ssockaddr_inClient.sin_family = AF_INET; //Э����
		Ssockaddr_inClient.sin_port = htons(12345); //��Ҫ�����Ķ˿�
		Ssockaddr_inClient.sin_addr.s_addr = inet_addr(c_pcIp); //inet_addr:���ַ�����Ч���ַ���ת��Ϊ32λ�����������ֽ����IPV4��ַ
		SOCKETClient = socket(AF_INET, SOCK_DGRAM, 0); //��ȡ�׽��־��( ��ַ����, �׽�������, �׽���Э��)
	}

	/*����ѭ��*/
	while (TRUE) {
		char pcBuffer[1024] = "/0"; //�ַ�����

		/*���뷢����Ϣ*/
		{
			printf("������Ϣ:");
			scanf("%s", pcBuffer); //���� �� �ַ�����
		}
		/*����UDP���ݰ�(�׽��־��, ���������ݻ�������ַ, ��������С, ������ʽ, Ŀ���׽��ֵ�ַ, �׽��ֵ�ַ����)*/
		if (sendto(SOCKETClient, pcBuffer, sizeof(pcBuffer), 0,
				(struct sockaddr*) &Ssockaddr_inClient, nLength) != SOCKET_ERROR) {
			/*����UDP���ݰ�(�׽��־��, ���������ݻ�������ַ, ��������С, ������ʽ, Ŀ���׽��ֵ�ַ, �׽��ֵ�ַ����)
			 * �������Զ��������������ݱ�һֱ�ȴ�*/
			if (recvfrom(SOCKETClient, pcBuffer, sizeof(pcBuffer), 0,
					(struct sockaddr*) &Ssockaddr_inClient,
					&nLength) != SOCKET_ERROR) {
				printf("������Ϣ: %s\n\n", pcBuffer);
			}
		}
	}

	/*�رտͻ���*/
	{
		closesocket(SOCKETClient);		//�ͷ��׽��־��
		return EXIT_SUCCESS;		//��������
	}
}
