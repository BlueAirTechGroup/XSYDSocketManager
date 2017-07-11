#include "XSYDMultiThread.h"
using namespace std;
	#if defined(linux) || defined(_UNIX) || defined(__LINUX__)
		XSYDLinuxCallBack clsXSYDLinuxMT::AddThread(XSYDLinuxMTTask ExecTask){
			pthread_t id;
			XSYDLinuxCallBack MyTemp;
			int ret = pthread_create(&id, NULL, ExecTask, NULL);
			MyTemp.Success = ret == 0;
			MyTemp.ThreadID = id;
			return MyTemp;
		}
		XSYDLinuxCallBack clsXSYDLinuxMT::AddThreadEx(XSYDLinuxMTTask, void* Parameters){
			pthread_t id;
			XSYDLinuxCallBack MyTemp;
			int ret = pthread_create(&id, NULL, ExecTask, Parameters);
			MyTemp.Success = ret == 0;
			MyTemp.ThreadID = id;
			return MyTemp;
		}
		void clsXSYDLinuxMT::WaitForThread(pthread_t MyThread){
			pthread_join(MyThread,NULL);
		}
	#endif
	#if defined(__WINDOWS_) || defined(_WIN32)
		HANDLE clsXSYDWinMT::AddThread(XSYDWinMTTask ExecTask){
			HANDLE TempHD = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ExecTask, NULL, 0, NULL);
			return TempHD;
		}
		HANDLE clsXSYDWinMT::AddThreadEx(XSYDWinMTTask ExecTask, void* Parameters){
			HANDLE TempHD = (HANDLE)_beginthreadex(NULL, 0,(_beginthreadex_proc_type) ExecTask, Parameters, 0, NULL);
			return TempHD;
		}
		void clsXSYDWinMT::WaitForThread(HANDLE MyThread){
			WaitForSingleObject(MyThread,INFINITE);
		}
	#endif
