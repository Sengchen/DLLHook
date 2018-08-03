// DLLHook.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DLLHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma data_seg("SHARED") //��ͬInstance����ĸñ���   
static HHOOK  hhk = NULL; //��깳�Ӿ��  
static HINSTANCE hinst = NULL; //��dll��ʵ����� (hook.dll)  
#pragma data_seg()  
#pragma comment(linker, "/section:SHARED,rws")  
//���ϵı���Ϊ����  

CString temp; //������ʾ�������ʱ����  
bool bHook = false; //�Ƿ�Hook�˺���  
bool m_bInjected = false; //�Ƿ��API������Hook  
BYTE OldCode[5]; //ԭ����API��ڴ���  
BYTE NewCode[5]; //����ת��API���� (jmp xxxx)  
typedef int (WINAPI*AddProc)(int a, int b);//add.dll�е�add��������  
AddProc add; //add.dll�е�add����  
HANDLE hProcess = NULL; //�������̵ľ��  
FARPROC pfadd;  //ָ��add������Զָ��  
DWORD dwPid;  //��������ID  
//end of ��������  
// CHookApp

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CDLLHookApp

BEGIN_MESSAGE_MAP(CDLLHookApp, CWinApp)
END_MESSAGE_MAP()


//��깳�ӹ��̣�ʲô����Ҳ������Ŀ����ע��dll��������  
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(hhk, nCode, wParam, lParam);
}

//�������ӵĺ���  
void HookOn()
{
	ASSERT(hProcess != NULL);

	DWORD dwTemp = 0;
	DWORD dwOldProtect;

	//���ڴ汣��ģʽ��Ϊ��д,��ģʽ������dwOldProtect  
	VirtualProtectEx(hProcess, pfadd, 5, PAGE_READWRITE, &dwOldProtect);
	//������������add()��ǰ5���ֽڸ�ΪJmp Myadd   
	WriteProcessMemory(hProcess, pfadd, NewCode, 5, 0);
	//���ڴ汣��ģʽ�Ļ�ΪdwOldProtect  
	VirtualProtectEx(hProcess, pfadd, 5, dwOldProtect, &dwTemp);

	bHook = true;
}
//�رչ��ӵĺ���  
void HookOff()//������������add()����ڴ���ָ�  
{
	ASSERT(hProcess != NULL);

	DWORD dwTemp = 0;
	DWORD dwOldProtect;

	VirtualProtectEx(hProcess, pfadd, 5, PAGE_READWRITE, &dwOldProtect);
	WriteProcessMemory(hProcess, pfadd, OldCode, 5, 0);
	VirtualProtectEx(hProcess, pfadd, 5, dwOldProtect, &dwTemp);
	bHook = false;
}
//Ȼ��д�����Լ���Myadd()����  
int WINAPI Myadd(int a, int b)
{
	//�ػ��˶�add()�ĵ��ã����Ǹ�a,b������һ������
	a = a + 1;
	b = b + 1;

	HookOff();//�ص�Myadd()���ӷ�ֹ��ѭ��  

	int ret;
	ret = add(a, b);

	HookOn();//����Myadd()����  

	return ret;
}
//�ã�����Ҫ��HOOK������ 
void Inject()
{

	if (m_bInjected == false)
	{ //��ֻ֤����1��  
		m_bInjected = true;

		//��ȡadd.dll�е�add()����  
		HMODULE hmod = ::LoadLibrary(_T("DLLTest.dll"));
		add = (AddProc)::GetProcAddress(hmod, "_add@8");
		pfadd = (FARPROC)add;

		if (pfadd == NULL)
		{
			AfxMessageBox(L"cannot locate add()");
		}

		// ��add()�е���ڴ��뱣����OldCode[]  
		_asm
		{
			lea edi, OldCode
			mov esi, pfadd
			cld
			movsd
			movsb
		}

		NewCode[0] = 0xe9;//ʵ����0xe9���൱��jmpָ��  
						  //��ȡMyadd()����Ե�ַ  
		_asm
		{
			lea eax, Myadd
			mov ebx, pfadd
			sub eax, ebx
			sub eax, 5
			mov dword ptr[NewCode + 1], eax
		}
		//�����ϣ�����NewCode[]���ָ���൱��Jmp Myadd  
		HookOn(); //���Կ���������  
	}
}


// CDLLHookApp ����

CDLLHookApp::CDLLHookApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDLLHookApp ����

CDLLHookApp theApp;

//��깳�Ӱ�װ����:  
BOOL InstallHook()
{

	hhk = ::SetWindowsHookEx(WH_MOUSE, MouseProc, hinst, 0);

	return true;
}

//ж����깳�Ӻ���  
void UninstallHook()
{
	::UnhookWindowsHookEx(hhk);
}


// CDLLHookApp ��ʼ��

BOOL CDLLHookApp::InitInstance()
{
	CWinApp::InitInstance();

	//���dll ʵ�������̾��  
	hinst = ::AfxGetInstanceHandle();
	DWORD dwPid = ::GetCurrentProcessId();
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid);
	//����ע�亯��  
	Inject();

	return TRUE;
}
