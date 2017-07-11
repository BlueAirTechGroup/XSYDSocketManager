/* 
 * Powered by www.xsyds.cn(c)2017, all rights reserved
 * Only available on WINDOWS currently
*/
#ifndef XSYDSM
	#if defined(__WINDOWS_) || defined(_WIN32)
		#define XSYDSM
		#include <stdio.h>
	
		#include <Winsock2.h>
		#include <WS2tcpip.h>
	
		#include <windows.h>
		#include "XSYDMultiThread.h"
		#include <stdlib.h>
		#include <time.h>
		#include <string.h>
		#include "XSYDStr.h"
		#pragma comment(lib,"ws2_32.lib")
		//#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
		typedef void (__stdcall *XSYDGetMsgCB)(SOCKET,char*,unsigned int,char*,unsigned short);
		typedef void (__stdcall *XSYDConnectedCB)(SOCKET,char*,unsigned short);
		typedef void (__stdcall *XSYDDisconnectCB)(SOCKET);
		typedef char* XSYDIPAddr;
		using namespace std;

		struct XSYDSS{
			SOCKET FatherSocket;
			SOCKET ChildSocket;
			XSYDGetMsgCB MsgCB;
			XSYDConnectedCB ConnectCB;
			XSYDDisconnectCB DisconnectCB;
			SOCKADDR_IN Clientaddr;
			int TempAreaSize;
			int MaxRate;
			unsigned int MaxSingleConnect;
			unsigned int MaxWaitTime;
		};
		struct ListenInfo{
			u_short Port;
			int MaxConnect;
			XSYDGetMsgCB MsgCB;
			XSYDConnectedCB ConnectCB;
			XSYDDisconnectCB DisconnectCB;
			int TempAreaSize;
			int MaxRate;
			unsigned int MaxSingleConnect;
			unsigned int MaxWaitTime;
		};
		struct XSYDGetMsg{
			SOCKET MySocket;
			char* TempMsg;
			int MsgLength;
		};
		struct XSYDWaitMsg{
			SOCKET MySocket;
			int TempAreaSize;
			XSYDGetMsgCB MsgCB;
			XSYDConnectedCB ConnectCB;
			XSYDDisconnectCB DisconnectCB;
			SOCKADDR_IN remoteAddr;
		};
		struct XSYDSendMsgInfo {
			SOCKET MySocket;
			char* Data;
		};
		struct XSYDConnectCBDT {
			int Status;
			SOCKET ConnectedSocket;
		};
		class clsXSYDSocketManager{
			private:
			
			protected:
			
			public:
				clsXSYDSocketManager();
				void ShutdownConnection();
				int StartListen(u_short Port,int MaxConnect,int MaxMsgPerSec, unsigned int SingleMaxConnect, unsigned int MaxWaitTime,int TempAreaSize,XSYDGetMsgCB MsgCallBack,XSYDConnectedCB ConnectedCallBack, XSYDDisconnectCB DisconnectedCallBack);
				void CloseListenConnection(SOCKET MySocket);
				void StopListen();
				XSYDConnectCBDT Connect(char * Addr, u_short Port, int TempAreaSize, XSYDGetMsgCB GetMsgCallBack, XSYDConnectedCB ConnectCallBack, XSYDDisconnectCB DisconnectCallBack);
				int SendData_Ajax(SOCKET MySocket,char* DATA);
				int SendData(SOCKET MySocket, char* DATA);
				void ForceCloseListenConnection(SOCKET MySocket);
		};
	#elif defined(linux) || defined(_UNIX) || defined(__LINUX__)
		#include <stdio.h>
		#include <sys/types.h>
		#include <netinet/in.h>
		#include <arpa/inet.h>
		#include <stdlib.h>
		typedef void(__stdcall *XSYDGetMsgCB)(int, char*, unsigned int, char*, unsigned short);
		typedef void(__stdcall *XSYDConnectedCB)(int, char*, unsigned short);
		typedef void(__stdcall *XSYDDisconnectCB)(int);
		typedef char* XSYDIPAddr;
		using namespace std;

		struct XSYDSS {
			int FatherSocket;
			int ChildSocket;
			XSYDGetMsgCB MsgCB;
			XSYDConnectedCB ConnectCB;
			XSYDDisconnectCB DisconnectCB;
			sockaddr_in Clientaddr;
			int TempAreaSize;
			int MaxRate;
			unsigned int MaxSingleConnect;
			unsigned int MaxWaitTime;
		};
		struct ListenInfo {
			unsigned short Port;
			int MaxConnect;
			XSYDGetMsgCB MsgCB;
			XSYDConnectedCB ConnectCB;
			XSYDDisconnectCB DisconnectCB;
			int TempAreaSize;
			int MaxRate;
			unsigned int MaxSingleConnect;
			unsigned int MaxWaitTime;
		};
		struct XSYDGetMsg {
			int MySocket;
			char* TempMsg;
			int MsgLength;
		};
		struct XSYDWaitMsg {
			int MySocket;
			int TempAreaSize;
			XSYDGetMsgCB MsgCB;
			XSYDConnectedCB ConnectCB;
			XSYDDisconnectCB DisconnectCB;
			sockaddr_in remoteAddr;
		};
		struct XSYDSendMsgInfo {
			SOCKET MySocket;
			char* Data;
		};
		struct XSYDConnectCBDT {
			int Status;
			SOCKET ConnectedSocket;
		};
		class clsXSYDSocketManager {
		private:

		protected:

		public:
			clsXSYDSocketManager();
			void ShutdownConnection();
			int StartListen(u_short Port, int MaxConnect, int MaxMsgPerSec, unsigned int SingleMaxConnect, unsigned int MaxWaitTime, int TempAreaSize, XSYDGetMsgCB MsgCallBack, XSYDConnectedCB ConnectedCallBack, XSYDDisconnectCB DisconnectedCallBack);
			void CloseListenConnection(SOCKET MySocket);
			void StopListen();
			XSYDConnectCBDT Connect(char * Addr, u_short Port, int TempAreaSize, XSYDGetMsgCB GetMsgCallBack, XSYDConnectedCB ConnectCallBack, XSYDDisconnectCB DisconnectCallBack);
			int SendData_Ajax(SOCKET MySocket, char* DATA);
			int SendData(SOCKET MySocket, char* DATA);
			void ForceCloseListenConnection(SOCKET MySocket);
		};
	#endif
#endif
