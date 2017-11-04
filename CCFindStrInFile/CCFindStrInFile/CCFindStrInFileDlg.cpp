
// CCFindStrInFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CCFindStrInFile.h"
#include "CCFindStrInFileDlg.h"
#include "afxdialogex.h"
#include <iostream>

#include "filenameos.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CCCFindStrInFileDlg dialog



CCCFindStrInFileDlg::CCCFindStrInFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CCFINDSTRINFILE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCCFindStrInFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_Dir);
	DDX_Control(pDX, IDC_EDIT_STR, m_Str);
	DDX_Control(pDX, IDC_PROGRESS, m_ShowStr);
}

BEGIN_MESSAGE_MAP(CCCFindStrInFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENFOLEDER, &CCCFindStrInFileDlg::OnBnClickedBtnOpenfoleder)
	ON_BN_CLICKED(IDFIND, &CCCFindStrInFileDlg::OnBnClickedFind)
	ON_MESSAGE(MSG_SHOW_MSG, &CCCFindStrInFileDlg::ShowMsg)
END_MESSAGE_MAP()


// CCCFindStrInFileDlg message handlers

BOOL CCCFindStrInFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCCFindStrInFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCCFindStrInFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCCFindStrInFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//start find str file
void CCCFindStrInFileDlg::OnBnClickedBtnOpenfoleder()
{
	// TODO: Add your control notification handler code here
	//打开文件夹
#define MAX_CFileDialog_FILE_COUNT 99
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)

	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);
	CFileDialog dlgFile(TRUE);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.Flags |= OFN_ALLOWMULTISELECT;
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	dlgFile.DoModal();
	m_strDir = ofn.lpstrFile;
	m_Dir.SetWindowText(m_strDir);
	fileName.ReleaseBuffer();
	m_ShowStr.SetWindowText(m_strDir);
}

//find str
void CCCFindStrInFileDlg::OnBnClickedFind()
{
	// TODO: Add your control notification handler code here
	m_Str.GetWindowText(m_strInd);
	USES_CONVERSION;
	string strFileStr = W2A(m_strInd.GetBuffer());
	if (strFileStr == "")
	{
		MessageBox(_T("字符串为空,请重新输入！"), _T("提示"), MB_OK);
		return;
	}
	string strFilePath = W2A(m_strDir.GetBuffer());
	if (strFilePath == "")
	{
		MessageBox(_T("文件路径为空,请重新选择！"), _T("提示"), MB_OK);
		return;
	}
	
	HANDLE hThread;
	DWORD ThreadID;
	hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReverseThread, (LPVOID)this, 0,&ThreadID);
}


//打印字符串
LRESULT CCCFindStrInFileDlg::ShowMsg(WPARAM wParam, LPARAM lParam)
{
	CString* strPath = (CString*)lParam;
	m_ShowStr.SetWindowText(*strPath);
	return 0;
}

LRESULT CCCFindStrInFileDlg::ReverseThread(LPVOID lpvoid)
{
	CCCFindStrInFileDlg* pDlg = (CCCFindStrInFileDlg*)lpvoid;
	USES_CONVERSION;
	string strFilePath = W2A(pDlg->m_strDir.GetBuffer());
	ReverseDirectory(strFilePath, pDlg->m_StruPath.m_arrFilePath);
	return 0;
}
