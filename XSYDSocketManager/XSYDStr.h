#ifndef XSYDString
	#define XSYDString
	#include <string.h>
	//#include <stdio.h>
	#include <stdlib.h>
	class clsXSYDString{
		private:
			
		protected:
			
		public:
			int FindStr(char* FatherStr,char* ChildStr, unsigned int StartPos=0);
			int FindStr_s(char* FatherStr, unsigned  int FatherStrLength, char* ChildStr, unsigned int StartPos);
			int FindStrReverse(char* FatherStr, char* ChildStr, unsigned int StartPos = 0);
			int FindStrReverse_s(char* FatherStr, unsigned  int FatherStrLength, char* ChildStr, unsigned int StartPos = 0);
			char* SubStr(char* FatherStr,unsigned int StartPos,unsigned int Length);
			char* SubStr_s(char* FatherStr, unsigned int FatherStrLength, unsigned int StartPos, unsigned int Length);
			char* MiddleText(char* FatherStr,char* StartStr, char* EndStr, unsigned int StartPos=0);
			char* MiddleText_s(char* FatherStr, unsigned int FatherStrLength, char* StartStr, char* EndStr, unsigned int StartPos = 0);
			void ReleaseSubStr(char* Result);
	};
#endif
