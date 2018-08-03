
// DllExcutorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DllExcutor.h"
#include "DllExcutorDlg.h"
#include "afxdialogex.h"
#include <string.h>
#include <string>
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDllExcutorDlg 对话框



CDllExcutorDlg::CDllExcutorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLLEXCUTOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDllExcutorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDllExcutorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDllExcutorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDllExcutorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDllExcutorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDllExcutorDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDllExcutorDlg 消息处理程序

BOOL CDllExcutorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDllExcutorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDllExcutorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDllExcutorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HINSTANCE hAddDll = NULL;

void CDllExcutorDlg::OnBnClickedOk()
{
	typedef int (__stdcall* AddProc)(int a, int b);//函数原型定义  
	AddProc add = NULL;

	if (hAddDll == NULL)
	{
		hAddDll = ::LoadLibrary(_T("DLLTest.dll"));//加载dll  
	}
	add = (AddProc)::GetProcAddress(hAddDll, "_add@8");//获取函数add地址  

	if (add != NULL) {
		int a = 1;
		int b = 2;
		int c = add(1, 2);
		CString tem;
		tem.Format(_T("%d+%d=%d"), a, b, c);
		AfxMessageBox(tem);
	}
	else {
		AfxMessageBox(_T("获取方法失败!"));
	}
}

HINSTANCE hinst;

void CDllExcutorDlg::OnBnClickedButton1()
{
	hinst = LoadLibrary(_T("DLLHook.dll"));
	if (hinst == NULL)
	{
		AfxMessageBox(_T("no DLLHook.dll!"));
		return;
	}
	typedef BOOL(CALLBACK *inshook)();
	inshook insthook;

	insthook = ::GetProcAddress(hinst, "InstallHook");
	if (insthook == NULL)
	{
		AfxMessageBox(_T("func not found!"));
		return;
	}

	DWORD pid = ::GetCurrentProcessId();
	BOOL ret = insthook();
}

void CDllExcutorDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码s
	typedef BOOL(CALLBACK *UnhookProc)();
	UnhookProc UninstallHook;
	UninstallHook = ::GetProcAddress(hinst, "UninstallHook");
	if (UninstallHook == NULL) UninstallHook();
	if (hinst != NULL)
	{
		::FreeLibrary(hinst);
	}
	if (hAddDll != NULL)
	{
		::FreeLibrary(hAddDll);
	}
	CDialog::OnCancel();
}


void CDllExcutorDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	char *url = "https://gss0.bdstatic.com/70cFsjip0QIZ8tyhnq/img/logo-zhidao.gif";
	char *path = "D:\\logo-zhidao.gif";
	HRESULT hResult = URLDownloadToFileA(NULL, url, path, 0, 0);
}
