// DLLHook.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DLLHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma data_seg("SHARED") //不同Instance共享的该变量   
static HHOOK  hhk = NULL; //鼠标钩子句柄  
static HINSTANCE hinst = NULL; //本dll的实例句柄 (hook.dll)  
#pragma data_seg()  
#pragma comment(linker, "/section:SHARED,rws")  
//以上的变量为共享  

CString temp; //用于显示错误的临时变量  
bool bHook = false; //是否Hook了函数  
bool m_bInjected = false; //是否对API进行了Hook  
BYTE OldCode[5]; //原程序API入口代码  
BYTE NewCode[5]; //新跳转的API代码 (jmp xxxx)  
typedef int (WINAPI*AddProc)(int a, int b);//add.dll中的add函数定义  
AddProc add; //add.dll中的add函数  
HANDLE hProcess = NULL; //所处进程的句柄  
FARPROC pfadd;  //指向add函数的远指针  
DWORD dwPid;  //所处进程ID  
//end of 变量定义  
// CHookApp

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CDLLHookApp

BEGIN_MESSAGE_MAP(CDLLHookApp, CWinApp)
END_MESSAGE_MAP()


//鼠标钩子过程，什么事情也不做，目的是注入dll到程序中  
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(hhk, nCode, wParam, lParam);
}

//开启钩子的函数  
void HookOn()
{
	ASSERT(hProcess != NULL);

	DWORD dwTemp = 0;
	DWORD dwOldProtect;

	//将内存保护模式改为可写,老模式保存入dwOldProtect  
	VirtualProtectEx(hProcess, pfadd, 5, PAGE_READWRITE, &dwOldProtect);
	//将所属进程中add()的前5个字节改为Jmp Myadd   
	WriteProcessMemory(hProcess, pfadd, NewCode, 5, 0);
	//将内存保护模式改回为dwOldProtect  
	VirtualProtectEx(hProcess, pfadd, 5, dwOldProtect, &dwTemp);

	bHook = true;
}
//关闭钩子的函数  
void HookOff()//将所属进程中add()的入口代码恢复  
{
	ASSERT(hProcess != NULL);

	DWORD dwTemp = 0;
	DWORD dwOldProtect;

	VirtualProtectEx(hProcess, pfadd, 5, PAGE_READWRITE, &dwOldProtect);
	WriteProcessMemory(hProcess, pfadd, OldCode, 5, 0);
	VirtualProtectEx(hProcess, pfadd, 5, dwOldProtect, &dwTemp);
	bHook = false;
}
//然后，写我们自己的Myadd()函数  
int WINAPI Myadd(int a, int b)
{
	//截获了对add()的调用，我们给a,b都加上一定的数
	a = a + 1;
	b = b + 1;

	HookOff();//关掉Myadd()钩子防止死循环  

	int ret;
	ret = add(a, b);

	HookOn();//开启Myadd()钩子  

	return ret;
}
//好，最重要的HOOK函数： 
void Inject()
{

	if (m_bInjected == false)
	{ //保证只调用1次  
		m_bInjected = true;

		//获取add.dll中的add()函数  
		HMODULE hmod = ::LoadLibrary(_T("DLLTest.dll"));
		add = (AddProc)::GetProcAddress(hmod, "_add@8");
		pfadd = (FARPROC)add;

		if (pfadd == NULL)
		{
			AfxMessageBox(L"cannot locate add()");
		}

		// 将add()中的入口代码保存入OldCode[]  
		_asm
		{
			lea edi, OldCode
			mov esi, pfadd
			cld
			movsd
			movsb
		}

		NewCode[0] = 0xe9;//实际上0xe9就相当于jmp指令  
						  //获取Myadd()的相对地址  
		_asm
		{
			lea eax, Myadd
			mov ebx, pfadd
			sub eax, ebx
			sub eax, 5
			mov dword ptr[NewCode + 1], eax
		}
		//填充完毕，现在NewCode[]里的指令相当于Jmp Myadd  
		HookOn(); //可以开启钩子了  
	}
}


// CDLLHookApp 构造

CDLLHookApp::CDLLHookApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDLLHookApp 对象

CDLLHookApp theApp;

//鼠标钩子安装函数:  
BOOL InstallHook()
{

	hhk = ::SetWindowsHookEx(WH_MOUSE, MouseProc, hinst, 0);

	return true;
}

//卸载鼠标钩子函数  
void UninstallHook()
{
	::UnhookWindowsHookEx(hhk);
}


// CDLLHookApp 初始化

BOOL CDLLHookApp::InitInstance()
{
	CWinApp::InitInstance();

	//获得dll 实例，进程句柄  
	hinst = ::AfxGetInstanceHandle();
	DWORD dwPid = ::GetCurrentProcessId();
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid);
	//调用注射函数  
	Inject();

	return TRUE;
}
