
// DllExcutorDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDllExcutorDlg �Ի���



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


// CDllExcutorDlg ��Ϣ�������

BOOL CDllExcutorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDllExcutorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDllExcutorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HINSTANCE hAddDll = NULL;

void CDllExcutorDlg::OnBnClickedOk()
{
	typedef int (__stdcall* AddProc)(int a, int b);//����ԭ�Ͷ���  
	AddProc add = NULL;

	if (hAddDll == NULL)
	{
		hAddDll = ::LoadLibrary(_T("DLLTest.dll"));//����dll  
	}
	add = (AddProc)::GetProcAddress(hAddDll, "_add@8");//��ȡ����add��ַ  

	if (add != NULL) {
		int a = 1;
		int b = 2;
		int c = add(1, 2);
		CString tem;
		tem.Format(_T("%d+%d=%d"), a, b, c);
		AfxMessageBox(tem);
	}
	else {
		AfxMessageBox(_T("��ȡ����ʧ��!"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������s
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char *url = "https://gss0.bdstatic.com/70cFsjip0QIZ8tyhnq/img/logo-zhidao.gif";
	char *path = "D:\\logo-zhidao.gif";
	HRESULT hResult = URLDownloadToFileA(NULL, url, path, 0, 0);
}
