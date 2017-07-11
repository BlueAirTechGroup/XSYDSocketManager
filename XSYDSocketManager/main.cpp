#include "XSYDSocketManager.h"
#include <iostream>
clsXSYDSocketManager MySockets;
using namespace std;
void __stdcall SC(SOCKET MySocket, char* RemoteAddr, unsigned int RemotePort){
	std::cout << "Connected:" << RemoteAddr << std::endl;
}
void __stdcall SM(SOCKET MySocket, char* Msg,unsigned int TotalNum, char* RemoteAddr, unsigned int RemotePort){
	std::cout << "Message:" << Msg << std::endl;
}
void __stdcall SDC(SOCKET MySocket){
	std::cout << "Disconnected" << std::endl;
}
/*
//服务端测试代码 
int main(int argc, char** args){
	MySocket = new clsXSYDSocketManager();
	MySocket->StartListen(24000,5,10,1024*30,SM,SC,SDC);
	std::cout << "Started Listen:" << std::endl;
	while(1){
		
	} 
}
*/

//客户端测试代码
int main(int agrc, char** args){
	std::cout << "开始连接.." << std::endl;
	XSYDConnectCBDT MyCCB = MySockets.Connect((char*)"127.0.0.1", 24000, 3 * 1024, (XSYDGetMsgCB)SM, (XSYDConnectedCB)SC, SDC);
	MySockets.SendData(MyCCB.ConnectedSocket, "Login admin,fuck");
	MySockets.SendData(MyCCB.ConnectedSocket, "GetMapInfo 0");
	MySockets.SendData(MyCCB.ConnectedSocket, "Move 0,2,2");
	MySockets.SendData(MyCCB.ConnectedSocket, "Talk Nearby,FUCKYOU");

	char MyCommand[20];
	while(strcmp(MyCommand,"exit")!=0){
		std::cin >> MyCommand;
	}
	
	return 0;
}
