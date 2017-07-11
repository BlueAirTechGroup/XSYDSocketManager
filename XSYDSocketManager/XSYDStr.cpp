#include "XSYDStr.h"
int clsXSYDString::FindStr(char* FatherStr,char* ChildStr, unsigned int StartPos){
	//我们需要快速的找到位置
	//第一步: 找到ChildStr第一个字符的出现的所有位置
	if(strlen(ChildStr)>strlen(FatherStr)){
		return -1;
	}
	bool AllFind; 
	for(unsigned int i=StartPos;i<strlen(FatherStr);i++){
		if(FatherStr[i]==ChildStr[0]){
			//查看第二到最后一个字符
			AllFind=true;
			for(unsigned int j=1;j<strlen(ChildStr);j++){
				if(FatherStr[i+j]!=ChildStr[j]){
					AllFind=false;
				}
			}
			if(AllFind==true){
				return i;
			}
		}
	}
	return -1;
}
int clsXSYDString::FindStr_s(char* FatherStr, unsigned int FatherStrLength, char* ChildStr, unsigned int StartPos) {
	//我们需要快速的找到位置
	//第一步: 找到ChildStr第一个字符的出现的所有位置
	if (strlen(ChildStr)>FatherStrLength) {
		return -1;
	}
	bool AllFind;
	for (unsigned int i = StartPos;i<FatherStrLength;i++) {
		if (FatherStr[i] == ChildStr[0]) {
			//查看第二到最后一个字符
			AllFind = true;
			for (unsigned int j = 1;j<strlen(ChildStr);j++) {
				if (FatherStr[i + j] != ChildStr[j]) {
					AllFind = false;
				}
			}
			if (AllFind == true) {
				return i;
			}
		}
	}
	return -1;
}
int clsXSYDString::FindStrReverse(char* FatherStr, char* ChildStr, unsigned int StartPos) {
	if (strlen(ChildStr) > strlen(FatherStr)) {
		return -1;
	}
	bool AllFind;
	for (unsigned int i = strlen(FatherStr) - StartPos - strlen(ChildStr); i >= 0; i--) {
		if (FatherStr[i] == ChildStr[0]) {
			//查看
			AllFind = true;
			for (unsigned int j = 1;j < strlen(ChildStr);j++) {
				if (FatherStr[i + j] != ChildStr[j]) {
					AllFind = false;
				}
			}
			if (AllFind == true) {
				return i;
			}
		}
	}
	return -1;
}
int clsXSYDString::FindStrReverse_s(char* FatherStr, unsigned int FatherStrLength, char* ChildStr, unsigned int StartPos) {
	if (strlen(ChildStr) > FatherStrLength) {
		return -1;
	}
	bool AllFind;
	for (unsigned int i = FatherStrLength - StartPos - strlen(ChildStr); i >= 0; i--) {
		if (FatherStr[i] == ChildStr[0]) {
			//查看
			AllFind = true;
			for (unsigned int j = 1;j < strlen(ChildStr);j++) {
				if (FatherStr[i + j] != ChildStr[j]) {
					AllFind = false;
				}
			}
			if (AllFind == true) {
				return i;
			}
		}
	}
	return -1;
}
char* clsXSYDString::SubStr(char* FatherStr, unsigned int StartPos, unsigned int Length){
	if(StartPos+Length > strlen(FatherStr)){
		return NULL;
	}
	char* TempRST = (char*) malloc((Length+1+1)*sizeof(char));
	strncpy_s(TempRST,(Length+1)*sizeof(char),FatherStr+StartPos,Length);
	TempRST[Length+1] = '\0';
	return TempRST;
}
char* clsXSYDString::SubStr_s(char* FatherStr, unsigned int FatherStrLength, unsigned int StartPos, unsigned int Length) {
	if (StartPos + Length > FatherStrLength) {
		return NULL;
	}
	char* TempRST = (char*)malloc((Length + 1 + 1) * sizeof(char));
	strncpy_s(TempRST, (Length + 1) * sizeof(char), FatherStr + StartPos, Length);
	TempRST[Length + 1] = '\0';
	return TempRST;
}
char* clsXSYDString::MiddleText(char* FatherStr,char* StartStr, char* EndStr, unsigned int StartSearchingPos){
	int FirstFind = this->FindStr(FatherStr,StartStr,StartSearchingPos);
	if(FirstFind == -1){
		return NULL;
	}
	int LastFind = this->FindStr(FatherStr,EndStr,FirstFind+strlen(EndStr));
	if(LastFind == -1){
		return NULL;
	}
	return this->SubStr(FatherStr,FirstFind+strlen(StartStr),LastFind-FirstFind-strlen(StartStr));
}
char* clsXSYDString::MiddleText_s(char* FatherStr, unsigned int FatherStrLength, char* StartStr, char* EndStr, unsigned int StartSearchingPos) {
	int FirstFind = this->FindStr_s(FatherStr, FatherStrLength, StartStr, StartSearchingPos);
	if (FirstFind == -1) {
		return NULL;
	}
	int LastFind = this->FindStr_s(FatherStr, FatherStrLength, EndStr, FirstFind + strlen(EndStr));
	if (LastFind == -1) {
		return NULL;
	}
	return this->SubStr_s(FatherStr, FatherStrLength, FirstFind + strlen(StartStr), LastFind - FirstFind - strlen(StartStr));
}
void clsXSYDString::ReleaseSubStr(char* Result) {
	free(Result);
}

