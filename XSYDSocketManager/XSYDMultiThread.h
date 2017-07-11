//* Powered by www.xsyds.cn(C)2017, all rights reserved
#ifndef XSYDMT
	#define XSYDMT
	#if defined(linux) || defined(_UNIX) || defined(__LINUX__)
		#include <pthread.h>
		using namespace std;
		struct XSYDLinuxCallBack{
			bool Success;
			pthread_t ThreadID;
		};
		typedef void (*XSYDLinuxMTTask)(void*);
		class clsXSYDLinuxMT{
			private:
				
			public:
				XSYDLinuxCallBack AddThread(XSYDLinuxMTTask);
				XSYDLinuxCallBack AddThreadEx(XSYDLinuxMTTask, void* Parameters);
				void WaitForThread(pthread_t MyThread);
		};
	#endif
	#if defined(__WINDOWS_) || defined(_WIN32)
		#include <stdio.h>   
		#include <process.h>   
		#include <windows.h>
		using namespace std;
		typedef unsigned int(__stdcall *XSYDWinMTTask)(PVOID);
		class clsXSYDWinMT {
		private:

		public:
			HANDLE AddThread(XSYDWinMTTask ExecTask);
			HANDLE AddThreadEx(XSYDWinMTTask ExecTask, void* Parameters);
			void WaitForThread(HANDLE MyThread);
		};
	#endif
#endif
