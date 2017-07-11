#include "XSYDSocketManager.h"
#if defined(_WIN32) || defined(__WINDOWS_)
bool Listening = false;
bool Connected = false;
clsXSYDWinMT *mymt;

XSYDIPAddr* BannedIps;
XSYDIPAddr* ConnectedLists;
unsigned int *ConnectedListNums;
unsigned int ConnectListNum;
int BannedNumbers = 0;
int ConnectedNum = 0;

using namespace std;
unsigned int __stdcall DealWithXSYDSingleSocket(PVOID MySockets) {
	ConnectedNum++;
	time_t starttime;
	time_t nowtime;
	time_t lastmsgtime;
	time_t currentmsgtime;
	int MsgTime = 0;
	DWORD MyThreadID = GetCurrentThreadId();
	XSYDSS *TempSSS = (XSYDSS*)MySockets;
	XSYDSS TempSS = *TempSSS;
	SOCKET MySocket = TempSS.ChildSocket;
	SOCKET FatherSocket = TempSS.FatherSocket;
	SOCKADDR_IN ClientAddr = TempSS.Clientaddr;
	XSYDGetMsgCB MsgCallBack = TempSS.MsgCB;
	XSYDConnectedCB ConnectCallBack = TempSS.ConnectCB;
	XSYDDisconnectCB DCCallBack = TempSS.DisconnectCB;
	int TempAreaSize = TempSS.TempAreaSize;
	int MaxRatePerSec = TempSS.MaxRate;
	unsigned int MaxWaitTime = TempSS.MaxWaitTime;
	unsigned int MaxSingleConnect = TempSS.MaxSingleConnect;
	//mymt->AddThreadEx(ConnectCallBack,(void*)&MySocket);
	char sendBuf[20] = { '\0' };
	char *MyRemoteAddr = (char*)inet_ntop(AF_INET, (void*)&ClientAddr.sin_addr, sendBuf, 16);
	unsigned short MyRemotePort = ntohs(ClientAddr.sin_port);
	//char *MyRemoteAddr = inet_ntoa(ClientAddr.sin_addr);
	ConnectCallBack(MySocket, MyRemoteAddr, MyRemotePort);
	char *tempch = (char*)malloc((TempAreaSize + 1 + 1) * sizeof(char));
	char* tempmsg = NULL;
	char* temptempmsg = NULL;
	char* tempmsgcb = NULL;
	bool inmsgquote = false;
	int msgpos;
	int msg2pos;
	unsigned int temptempmsgl = 0;
	unsigned int tempmsglength = 0;
	clsXSYDString *MyStringEncoder = new clsXSYDString();
	int RecvRST = 1;
	int HeartBeatRST = 0;
	int LastCheckBanNum = 0;
	bool NeedUpdateBL = false;
	time(&starttime);
	time(&lastmsgtime);

	if (MaxWaitTime != 0) {
		int timeout = 1000 * MaxWaitTime;
		int SetTO = setsockopt(MySocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	}
	//*************************DDOS*************************
	if (MaxRatePerSec != 0) {
		XSYDIPAddr* TempIps;
		for (int i = 0;i < BannedNumbers;i++) {
			if (strcmp(BannedIps[i], MyRemoteAddr) == 0) {
				free(tempch);
				closesocket(MySocket);
				DCCallBack(MySocket);
				ConnectedNum--;
				return 0;
			}
		}
		LastCheckBanNum = BannedNumbers;
	}
	//*********************************************************

	//***********************MaxSingleConnect***********************
	if (MaxSingleConnect != 0) {
		XSYDIPAddr* MyTempConnectedLists = (XSYDIPAddr*)malloc(sizeof(XSYDIPAddr)*(ConnectListNum + 1));
		unsigned int *MyTempConnectedListNums = (unsigned int *)malloc(sizeof(unsigned int)*(ConnectListNum + 1));
		bool FindedMyIPInConnectedList = false;
		//添加到ConnectList中
		for (unsigned int i = 0;i < ConnectListNum;i++) {
			MyTempConnectedLists[i] = ConnectedLists[i]; //赋值
			MyTempConnectedListNums[i] = ConnectedListNums[i];
			if (strcmp(ConnectedLists[i], MyRemoteAddr) == 0) {
				if (ConnectedListNums[i] == MaxSingleConnect && MaxSingleConnect != 0) {
					free(tempch);
					closesocket(MySocket);
					DCCallBack(MySocket);
					ConnectedNum--;
					return 0;
				}
				(ConnectedListNums[i])++;
				FindedMyIPInConnectedList = true;
				break;
			}
		}
		if (!FindedMyIPInConnectedList) {
			XSYDIPAddr MyTempListRemoteAddr = (XSYDIPAddr)malloc(sizeof(XSYDIPAddr));
			strcpy_s((char*)MyTempListRemoteAddr, 16, MyRemoteAddr);
			MyTempConnectedLists[ConnectListNum] = MyTempListRemoteAddr;
			MyTempConnectedListNums[ConnectListNum] = 1;
			if (ConnectListNum != 0) {
				free(ConnectedLists);
			}
			ConnectedLists = MyTempConnectedLists;
			ConnectedListNums = MyTempConnectedListNums;
			ConnectListNum++;
		}
	}
	//************************************************************


	while (Listening = true && RecvRST != INVALID_SOCKET && RecvRST != 0) {
		RecvRST = recv(MySocket, tempch, TempAreaSize, 0);
		//HeartBeatRST = send(MySocket, "XSYDHEARTB~{", 12, 0);
		//mymt->AddThreadEx(MsgCallBack,(void*)&TempStruct);
		if (MaxRatePerSec != 0) {
			if (LastCheckBanNum != BannedNumbers) {
				for (int i = 0;i<BannedNumbers;i++) {
					if (strcmp(BannedIps[i], MyRemoteAddr) == 0) {
						//减少一个ConnectListNum
						if (MaxSingleConnect != 0) {
							for (unsigned int i = 0;i < ConnectListNum;i++) {
								if (strcmp(ConnectedLists[i], MyRemoteAddr) == 0) {
									(ConnectedListNums[i])--;
									break;
								}
							}
						}
						//减少完毕
						free(tempch);
						closesocket(MySocket);
						DCCallBack(MySocket);
						ConnectedNum--;
						return 0;
					}
				}
				LastCheckBanNum = BannedNumbers;
			}
		}
		if (RecvRST != 0 && RecvRST != INVALID_SOCKET) {
		CheckMsgXML:
			tempch[RecvRST] = '\0';
			if (tempmsg == NULL) {
				tempmsglength = 0;
			}
			else {
				tempmsglength = strlen(tempmsg);
			}
			temptempmsg = (char*)malloc(sizeof(char)*(tempmsglength + strlen(tempch) + 1 + 1));
			if (tempmsglength != 0) {
				strcpy_s(temptempmsg, tempmsglength + 1, tempmsg);
			}
			strcpy_s(temptempmsg + tempmsglength, strlen(tempch) + 1, tempch);
			temptempmsg[tempmsglength + strlen(tempch) + 1] = '\0';
			if (tempmsg != NULL) {
				free(tempmsg);
			}

			tempmsg = temptempmsg;
			if (!inmsgquote) {
				msgpos = MyStringEncoder->FindStr(temptempmsg, "<Msg>", 0);
				if (msgpos == -1) {
					//找不到
					if (tempmsglength > 400) {
						//太大了,扔
						free(tempmsg);
						tempmsg = 0;
					}
				}
				else {
					inmsgquote = true;
				}

			}
			if (inmsgquote) {
				msg2pos = MyStringEncoder->FindStr(temptempmsg, "</Msg>", 0);
				if (msg2pos == -1) {
					//没找到,等下一个消息合并
					if (tempmsglength > 400) {
						//太大了,扔
						free(tempmsg);
						tempmsg = 0;
					}
				}
				else {
					tempmsgcb = MyStringEncoder->MiddleText(temptempmsg, "<Msg>", "</Msg>", 0);
					MsgCallBack(MySocket, tempmsgcb, (unsigned int)RecvRST, MyRemoteAddr, MyRemotePort);
					MyStringEncoder->ReleaseSubStr(tempmsgcb);
					temptempmsgl = strlen(temptempmsg) - msg2pos - strlen("</Msg>");
					temptempmsg = MyStringEncoder->SubStr(temptempmsg, msg2pos + strlen("</Msg>"), temptempmsgl);
					if (temptempmsg != NULL) {
						temptempmsg[temptempmsgl + 1] = '\0';
						free(tempmsg);
						tempmsg = temptempmsg;
						tempch[0] = '\0';
						goto CheckMsgXML;
					}
					else {
						free(tempmsg);
						tempmsg = NULL;
					}
					inmsgquote = false;
				}
			}

			MsgTime++;
			time(&nowtime);
			time(&currentmsgtime);
			if (difftime(currentmsgtime, lastmsgtime) >= MaxWaitTime && MaxWaitTime != 0) {
				//超时,退出
				//减少一个ConnectListNum
				if (MaxSingleConnect != 0) {
					for (unsigned int i = 0;i < ConnectListNum;i++) {
						if (strcmp(ConnectedLists[i], MyRemoteAddr) == 0) {
							(ConnectedListNums[i])--;
							break;
						}
					}
				}
				//减少完毕
				free(tempch);
				shutdown(MySocket, 2); //0 = 读取, 1=写入, 2=读写 
				DCCallBack(MySocket);
				ConnectedNum--;
				return 0;
			}
			time(&lastmsgtime);
			if (MsgTime >= (MaxRatePerSec * 2) && MaxRatePerSec != 0) {
				//一秒最多20个请求
				XSYDIPAddr* TempIps = (XSYDIPAddr*)malloc(sizeof(XSYDIPAddr)*(BannedNumbers + 1 + 1));
				NeedUpdateBL = true;
				for (int i = 0;i<BannedNumbers;i++) {
					TempIps[i] = BannedIps[i];
					if (strcmp(MyRemoteAddr, BannedIps[i]) == 0) {
						NeedUpdateBL = false;
					}
					//strcpy_s(TempIps[i],16,BannedIps[i]);
				}
				if (NeedUpdateBL == true) {
					BannedNumbers++;
					TempIps[BannedNumbers - 1] = (char*)malloc(sizeof(char) * (16 + 1));
					strcpy_s((char*)TempIps[BannedNumbers - 1], 16, MyRemoteAddr);
					if (BannedNumbers != 1) {
						free(BannedIps);
					}
					BannedIps = TempIps;
				}
				//减少一个ConnectListNum
				if (MaxSingleConnect != 0) {
					for (unsigned int i = 0;i < ConnectListNum;i++) {
						if (strcmp(ConnectedLists[i], MyRemoteAddr) == 0) {
							(ConnectedListNums[i])--;
							break;
						}
					}
				}
				//减少完毕
				free(tempch);
				closesocket(MySocket); //0 = 读取, 1=写入, 2=读写 
				DCCallBack(MySocket);
				ConnectedNum--;
				return 0;
			}
			if (difftime(nowtime, starttime) >= 2) {
				time(&starttime);
				MsgTime = 0;
			}
		}
	}
	//减少一个ConnectListNum
	if (MaxSingleConnect != 0) {
		for (unsigned int i = 0;i < ConnectListNum;i++) {
			if (strcmp(ConnectedLists[i], MyRemoteAddr) == 0) {
				(ConnectedListNums[i])--;
				break;
			}
		}
	}
	//减少完毕
	shutdown(MySocket, 2); //0 = 读取, 1=写入, 2=读写 
	DCCallBack(MySocket);

	delete MyStringEncoder;
	free(tempch);
	free(tempmsg);
	ConnectedNum--;
	return 0;
}
unsigned int __stdcall WaitForXSYDSocket(PVOID LSInfo) {
	Listening = true;
	ListenInfo *TempInfo = (ListenInfo*)LSInfo;
	u_short Port = TempInfo->Port;
	int MaxConnect = TempInfo->MaxConnect;
	XSYDGetMsgCB MsgCallBack = TempInfo->MsgCB;
	XSYDConnectedCB ConnectCallBack = TempInfo->ConnectCB;
	XSYDDisconnectCB DisconnectCallBack = TempInfo->DisconnectCB;
	int TempAreaSize = TempInfo->TempAreaSize;
	int MaxRatePerSec = TempInfo->MaxRate;
	unsigned int MaxWaitTime = TempInfo->MaxWaitTime;
	unsigned int SingleMaxConnect = TempInfo->MaxSingleConnect;
	free(TempInfo);
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1); //1.1 阻塞传输 2.2 异步传输
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) { //如果高八位或第八位与设定值不同(1.1) {
		WSACleanup();
		return -2;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0); //创建Socket实例 
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET; //inet_addr("127.0.0.1")
	addrSrv.sin_port = htons(Port);
	//getsocketname
	int BindStatus = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)); //绑定套接字
	if (BindStatus == SOCKET_ERROR) {
		return -3;
	}
	int ListenStatus = listen(sockSrv, MaxConnect);
	if (ListenStatus == SOCKET_ERROR) {
		return -4;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	SOCKET TempSocket;
	SOCKET sockConn;
	HANDLE* MyThreads = NULL;
	HANDLE* TempThreads;
	int HowManyThread = 0;
	XSYDSS TempSS;
	TempSS.FatherSocket = sockSrv;
	TempSS.MsgCB = MsgCallBack;
	TempSS.ConnectCB = ConnectCallBack;
	TempSS.DisconnectCB = DisconnectCallBack;
	TempSS.TempAreaSize = TempAreaSize;
	TempSS.MaxRate = MaxRatePerSec;
	TempSS.MaxWaitTime = MaxWaitTime;
	TempSS.MaxSingleConnect = SingleMaxConnect;
	while (Listening = true) {
		if (ConnectedNum < MaxConnect) {
			sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
			if (sockConn != INVALID_SOCKET) {
				HowManyThread = HowManyThread + 1;
				TempThreads = (HANDLE*)malloc(sizeof(HANDLE)*(HowManyThread + 1));
				for (int i = 0;i<(HowManyThread - 1);i++) {
					TempThreads[i] = MyThreads[i];
				}
				if (HowManyThread != 1) {
					free(MyThreads);
				}
				MyThreads = TempThreads;
				TempSocket = sockConn;
				TempSS.ChildSocket = TempSocket;
				TempSS.Clientaddr = addrClient;
				MyThreads[HowManyThread] = mymt->AddThreadEx(DealWithXSYDSingleSocket, (void*)&TempSS);
			}
		}

	}
	WaitForMultipleObjects(HowManyThread, MyThreads, TRUE, INFINITE); //等待子线程全部退出 
	free(MyThreads);
	//释放黑名单
	for (int banipxh = 0;banipxh < BannedNumbers;banipxh++) {
		free(BannedIps[banipxh]);
	}
	free(BannedIps);
	//释放完毕
	//释放MaxSingleConnect
	if (SingleMaxConnect != 0) {
		for (unsigned int mscxh = 0;mscxh < ConnectListNum;mscxh++) {
			free(ConnectedLists[mscxh]);
		}
		free(ConnectedLists);
		free(ConnectedListNums);
	}
	//释放完毕

	closesocket(sockSrv);
	WSACleanup();
	return 0;
}

unsigned int __stdcall XSYDWaitForMsg(PVOID Para) {
	XSYDWaitMsg *MyTEMPMTCur = (XSYDWaitMsg*)Para;
	XSYDWaitMsg MyParas = *MyTEMPMTCur;
	free(Para);
	XSYDGetMsgCB MsgCB = MyParas.MsgCB;
	XSYDConnectedCB ConnectCB = MyParas.ConnectCB;
	XSYDDisconnectCB DisConnectCB = MyParas.DisconnectCB;
	clsXSYDString *MyStringEncoder = new clsXSYDString();
	SOCKADDR_IN MyRemote = MyParas.remoteAddr;
	int MyTempAreaSize = MyParas.TempAreaSize;
	SOCKET MySocket = MyParas.MySocket;
	char sendBuf[20] = { '\0' };
	ConnectCB(MySocket, (char*)inet_ntop(AF_INET, &MyRemote.sin_addr, sendBuf, 16), ntohs(MyRemote.sin_port));
	char* temptempmsg = NULL;
	char* tempmsg = NULL;
	char* tempmsgcb = NULL;
	unsigned int tempmsglen = 0;
	int msgpos = 0;
	int msg2pos = 0;
	bool inmsgquote = false;
	unsigned int temptempmsgl = 0;
	int MyLastResult = 1;
	char *MyTemp = (char*)malloc((MyTempAreaSize + 1) * sizeof(char));
	while (MyLastResult != INVALID_SOCKET && MyLastResult != 0 && Connected) {
		MyLastResult = recv(MySocket, MyTemp, MyTempAreaSize, 0);
		if (MyLastResult != 0 && MyLastResult != INVALID_SOCKET) {
		CheckMsgXML:
			MyTemp[MyLastResult] = '\0';
			if (tempmsg == NULL) {
				tempmsglen = 0;
			}
			else {
				tempmsglen = strlen(tempmsg);
			}
			temptempmsg = (char*)malloc(sizeof(char)*(tempmsglen + strlen(MyTemp) + 1 + 1));
			if (tempmsglen != 0) {
				strcpy_s(temptempmsg, tempmsglen + 1, tempmsg);
			}
			strcpy_s(temptempmsg + tempmsglen, strlen(MyTemp) + 1, MyTemp);
			temptempmsg[tempmsglen + strlen(MyTemp) + 1] = '\0';
			if (tempmsg != NULL) {
				free(tempmsg);
			}
			tempmsg = temptempmsg;
			if (!inmsgquote) {
				msgpos = MyStringEncoder->FindStr(temptempmsg, "<Msg>", 0);
				if (msgpos == -1) {
					//没找到
					if (tempmsglen >= 400) {
						//太大了,扔
						free(temptempmsg);
						tempmsg = NULL;
					}
				}
				else {
					inmsgquote = true;
				}
			}
			if (inmsgquote) {
				msg2pos = MyStringEncoder->FindStr(temptempmsg, "</Msg>", 0);
				if (msg2pos == -1) {
					//没找到,等下一波
					if (tempmsglen >= 400) {
						//太大了,扔
						free(temptempmsg);
						tempmsg = NULL;
					}
				}
				else {
					tempmsgcb = MyStringEncoder->MiddleText(temptempmsg, "<Msg>", "</Msg>", 0);
					MsgCB(MySocket, tempmsgcb, strlen(tempmsgcb), (char*)inet_ntop(AF_INET, &MyRemote.sin_addr, sendBuf, 16), ntohs(MyRemote.sin_port));
					MyStringEncoder->ReleaseSubStr(tempmsgcb);
					temptempmsgl = strlen(temptempmsg) - msg2pos - strlen("</Msg>");
					temptempmsg = MyStringEncoder->SubStr(temptempmsg, msg2pos + strlen("</Msg>"), temptempmsgl);
					if (temptempmsg != NULL) {
						temptempmsg[temptempmsgl + 1] = '\0';
						free(tempmsg);
						tempmsg = temptempmsg;
						MyTemp[0] = '\0';
						goto CheckMsgXML;
					}
					else {
						free(tempmsg);
						tempmsg = NULL;
					}
					inmsgquote = false;
				}
			}

			memset(MyTemp, 0, MyTempAreaSize);
		}
	}
	free((void*)MyTemp);
	delete MyStringEncoder;
	shutdown(MySocket, 2);
	DisConnectCB(MySocket);
	WSACleanup();
	return 0;
}
clsXSYDSocketManager::clsXSYDSocketManager() {
	Listening = false;
	Connected = false;
	mymt = new clsXSYDWinMT();
}
void clsXSYDSocketManager::ShutdownConnection() {
	Connected = false;
}

void clsXSYDSocketManager::CloseListenConnection(SOCKET MySocket) {
	shutdown(MySocket, 2);
}
void clsXSYDSocketManager::ForceCloseListenConnection(SOCKET MySocket) {
	closesocket(MySocket);
}
XSYDConnectCBDT clsXSYDSocketManager::Connect(char* Addr, u_short Port, int TempAreaSize, XSYDGetMsgCB GetMsgCallBack, XSYDConnectedCB ConnectCallBack, XSYDDisconnectCB DisconnectCallBack) {
	WORD wVersionRequested;
	WSADATA wsaData;
	XSYDConnectCBDT TempCB;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		TempCB.Status = -1;
		return TempCB;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		TempCB.Status = -2;
		return TempCB;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	//addrSrv.sin_addr.S_un.S_addr=inet_addr(Addr);
	inet_pton(AF_INET, (PCSTR)Addr, (void*)&addrSrv.sin_addr.S_un.S_addr);
	addrSrv.sin_family = (ADDRESS_FAMILY)AF_INET;
	addrSrv.sin_port = (USHORT)htons(Port);
	int ConnectNum = (int)connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(addrSrv));
	if (ConnectNum == SOCKET_ERROR) {
		TempCB.Status = -3;
		return TempCB;
	}
	XSYDWaitMsg *MyWait = (XSYDWaitMsg*)malloc(sizeof(XSYDWaitMsg));
	(*MyWait).MySocket = sockClient;
	(*MyWait).TempAreaSize = TempAreaSize;
	(*MyWait).MsgCB = GetMsgCallBack;
	(*MyWait).DisconnectCB = DisconnectCallBack;
	(*MyWait).ConnectCB = ConnectCallBack;
	(*MyWait).remoteAddr = addrSrv;
	mymt->AddThreadEx(XSYDWaitForMsg, (void*)MyWait);
	Connected = true;
	TempCB.ConnectedSocket = sockClient;
	TempCB.Status = 0;
	return TempCB;
}
int clsXSYDSocketManager::StartListen(u_short Port, int MaxConnect, int MaxMsgPerSec, unsigned int SingleMaxConnect, unsigned int MaxWaitTime, int TempAreaSize, XSYDGetMsgCB MsgCallBack, XSYDConnectedCB ConnectedCallBack, XSYDDisconnectCB DisconnectedCallBack) {
	ListenInfo *TempInfo = (ListenInfo*)malloc(sizeof(ListenInfo));
	TempInfo->MaxConnect = MaxConnect;
	TempInfo->Port = Port;
	TempInfo->ConnectCB = ConnectedCallBack;
	TempInfo->MsgCB = MsgCallBack;
	TempInfo->DisconnectCB = DisconnectedCallBack;
	TempInfo->TempAreaSize = TempAreaSize;
	TempInfo->MaxRate = MaxMsgPerSec;
	TempInfo->MaxWaitTime = MaxWaitTime;
	TempInfo->MaxSingleConnect = SingleMaxConnect;
	mymt->AddThreadEx(WaitForXSYDSocket, (void *)TempInfo);
	return 0;
}
unsigned int __stdcall AjaxSendData(PVOID Data) {
	XSYDSendMsgInfo* MyData = (XSYDSendMsgInfo*)Data;

	unsigned int DataLength = strlen(MyData->Data) + strlen("<Msg>") + strlen("</Msg>");
	char* MyDatas = (char*)malloc(sizeof(char)*(DataLength + 1 + 1));
	strcpy_s(MyDatas, DataLength + 1, "<Msg>");
	strcpy_s(MyDatas + strlen("<Msg>"), strlen(MyData->Data) + 1, MyData->Data);
	strcpy_s(MyDatas + strlen("<Msg>") + strlen(MyData->Data), strlen("</Msg>") + 1, "</Msg>");
	MyDatas[DataLength + 1] = '\0';
	int SendState = send(MyData->MySocket, MyDatas, DataLength, 0);
	if (SendState == INVALID_SOCKET) {
		shutdown(MyData->MySocket, 2);
	}
	free(MyData->Data);
	free(MyDatas);
	return SendState;
}
int clsXSYDSocketManager::SendData_Ajax(SOCKET MySocket, char* DATA) {
	XSYDSendMsgInfo *MyInfo = (XSYDSendMsgInfo*)malloc(sizeof(XSYDSendMsgInfo));
	char* MyData = (char*)malloc(sizeof(char)*(strlen(DATA) + 1 + 1));
	strcpy_s(MyData, strlen(DATA) + 1, DATA);
	MyInfo->MySocket = MySocket;
	MyInfo->Data = MyData;
	mymt->AddThreadEx((XSYDWinMTTask)AjaxSendData, MyInfo);
	return 0;
}
int clsXSYDSocketManager::SendData(SOCKET MySocket, char* DATA) {
	XSYDSendMsgInfo *MyInfo = (XSYDSendMsgInfo*)malloc(sizeof(XSYDSendMsgInfo));
	unsigned int DataLength = strlen(DATA) + strlen("<Msg>") + strlen("</Msg>");
	char* MyData = (char*)malloc(sizeof(char)*(DataLength + 1 + 1));
	strcpy_s(MyData, DataLength + 1, "<Msg>");
	strcpy_s(MyData + strlen("<Msg>"), strlen(DATA) + 1, DATA);
	strcpy_s(MyData + strlen("<Msg>") + strlen(DATA), strlen("</Msg>") + 1, "</Msg>");
	MyData[DataLength + 1] = '\0';

	int SendState = send(MySocket, MyData, DataLength, 0);
	if (SendState == INVALID_SOCKET) {
		shutdown(MySocket, 2);
	}
	free(MyData);
	return SendState;
}


void clsXSYDSocketManager::StopListen() {
	Listening = false;
}
#elif defined(linux) || defined(_UNIX) || defined(__LINUX__)
	bool Listening = false;
	bool Connected = false;
	clsXSYDLinuxMT *mymt;
	XSYDIPAddr* BannedIps;
	XSYDIPAddr* ConnectedLists;
	unsigned int *ConnectedListNums;
	unsigned int ConnectListNum;
	int BannedNumbers = 0;
	int ConnectedNum = 0;
	using namespace std;

	
#endif