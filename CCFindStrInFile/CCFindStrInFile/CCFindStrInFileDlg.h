
// CCFindStrInFileDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CCCFindStrInFileDlg dialog
class CCCFindStrInFileDlg : public CDialogEx
{
// Construction
public:
	CCCFindStrInFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCFINDSTRINFILE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnOpenfoleder();
	CEdit m_Dir;
	CEdit m_Str;
};
