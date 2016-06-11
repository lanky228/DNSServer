/*
 * 套接字.c
 *
 *  Created on: 2016年6月9日
 *      Author: Lanky
 */
#include "DNSServer.h"

/**
 * 开启套接字
 */
int cmStartSocket() {
	SOCKET SOCKETServer; //skLocalSocket句柄
	WSADATA WSADATAData; //存储被WSAStartup函数调用后返回的Windows 版本信息
	struct sockaddr_in Ssockaddr_inServer; //套接字地址结构体 服务端
	struct sockaddr_in Ssockaddr_inClient; //套接字地址结构体 客户端
	int nLength = sizeof(Ssockaddr_inClient); //套接字信息结构体 大小

	/*启动套接字*/
	{
		if (WSAStartup(MAKEWORD(2, 1), &WSADATAData)) { //启动套接字(返回双字节(副版本号, 主版本号), 返回版本信息)
			printf("套接字启动失败\n");
			WSACleanup(); //终止套接字
			return EXIT_FAILURE; //服务启动失败
		}
		printf("启动域名服务...\n");
	}

	/*初始化套接字地址*/
	{
		Ssockaddr_inServer.sin_family = AF_INET; //协议族
		Ssockaddr_inServer.sin_port = htons(12345); //需要监听的端口
		Ssockaddr_inServer.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY就是指定地址为0.0.0.0的地址
		SOCKETServer = socket(AF_INET, SOCK_DGRAM, 0); //获取套接字句柄( 地址描述, 套接字类型, 套接字协议)
		bind(SOCKETServer, (struct sockaddr*) &Ssockaddr_inServer,
				sizeof(Ssockaddr_inServer)); //套接字和端口捆绑
	}

	/*循环接受消息*/
	while (TRUE) {
		char pcBuffer[1024] = "/0"; //字符数组

		/*接收UDP数据包(套接字句柄, 待发送数据缓冲区地址, 缓冲区大小, 接收形式, 目的套接字地址, 套接字地址长度)
		 * 函数会自动阻塞：若无数据便一直等待*/
		if (recvfrom(SOCKETServer, pcBuffer, sizeof(pcBuffer), 0,
				(struct sockaddr*) &Ssockaddr_inClient,
				&nLength) != SOCKET_ERROR) {
			/*inet_ntoa：转化字符串为ip地址
			 * printf(ip地址, 接受到的消息)*/
			printf("接收的数据为: [%s]---%s\n",
					inet_ntoa(Ssockaddr_inClient.sin_addr), pcBuffer);
			/*发送UDP数据包(套接字句柄, 待发送数据缓冲区地址, 缓冲区大小, 发送形式, 目的套接字地址, 套接字地址长度)*/
			sendto(SOCKETServer, pcBuffer, sizeof(pcBuffer), 0,
					(struct sockaddr*) &Ssockaddr_inClient, nLength);
		}
	}

	/*关闭服务*/
	{
		closesocket(SOCKETServer);			//释放套接字句柄
		return EXIT_SUCCESS;			//函数结束
	}
}

