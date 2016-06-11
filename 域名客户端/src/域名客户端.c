/*
 * 域名客户端.c
 *
 *  Created on: 2016年6月9日
 *      Author: Lanky
 */
#include<stdio.h>//IO函数
#include<stdlib.h>///基本数据类型
#include<Winsock2.h>//套接字函数

/**
 * 开启套接字
 */
int cmStartSocket();

/**
 * 主函数
 * argc：参数个数
 * argv：参数数组地址
 */
int main(int agrc, char*argv[]) {
	cmStartSocket();
	return EXIT_SUCCESS;		//函数结束
}

/**
 * 开启套接字
 */
int cmStartSocket() {
	SOCKET SOCKETClient; //SOCKET句柄
	WSADATA WSADATAData; //存储被WSAStartup函数调用后返回的Windows Sockets数据
	const char* c_pcIp = "127.0.0.1"; //需要绑定到本地的哪个IP地址
	struct sockaddr_in Ssockaddr_inClient; //套接字地址结构体
	int nLength = sizeof(Ssockaddr_inClient); //套接字信息结构体 大小

	/*启动套接字*/
	{
		if (WSAStartup(MAKEWORD(2, 1), &WSADATAData)) { //启动套接字(返回双字节(副版本号, 主版本号), 返回版本信息)
			printf("套接字启动失败\n");
			WSACleanup(); //终止套接字
			return EXIT_FAILURE; //服务启动失败
		}
		printf("启动域名客户端...\n");
	}

	/*初始化套接字地址*/
	{
		Ssockaddr_inClient.sin_family = AF_INET; //协议族
		Ssockaddr_inClient.sin_port = htons(12345); //需要监听的端口
		Ssockaddr_inClient.sin_addr.s_addr = inet_addr(c_pcIp); //inet_addr:若字符串有效则将字符串转换为32位二进制网络字节序的IPV4地址
		SOCKETClient = socket(AF_INET, SOCK_DGRAM, 0); //获取套接字句柄( 地址描述, 套接字类型, 套接字协议)
	}

	/*发送循环*/
	while (TRUE) {
		char pcBuffer[1024] = "/0"; //字符数组

		/*输入发送消息*/
		{
			printf("发送信息:");
			scanf("%s", pcBuffer); //输入 到 字符数组
		}
		/*发送UDP数据包(套接字句柄, 待发送数据缓冲区地址, 缓冲区大小, 发送形式, 目的套接字地址, 套接字地址长度)*/
		if (sendto(SOCKETClient, pcBuffer, sizeof(pcBuffer), 0,
				(struct sockaddr*) &Ssockaddr_inClient, nLength) != SOCKET_ERROR) {
			/*接收UDP数据包(套接字句柄, 待发送数据缓冲区地址, 缓冲区大小, 接收形式, 目的套接字地址, 套接字地址长度)
			 * 函数会自动阻塞：若无数据便一直等待*/
			if (recvfrom(SOCKETClient, pcBuffer, sizeof(pcBuffer), 0,
					(struct sockaddr*) &Ssockaddr_inClient,
					&nLength) != SOCKET_ERROR) {
				printf("返回消息: %s\n\n", pcBuffer);
			}
		}
	}

	/*关闭客户端*/
	{
		closesocket(SOCKETClient);		//释放套接字句柄
		return EXIT_SUCCESS;		//函数结束
	}
}
