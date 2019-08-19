
// SeverDlg.h: 头文件
//
#include <string>

#pragma once
#define		WM_SOCKET	WM_USER + 100

// CSeverDlg 对话框
class CSeverDlg : public CDialogEx
{
// 构造
public:
	CSeverDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	SOCKET socket_server;
	SOCKET socket_client;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_status;
	afx_msg void OnStartrun();
	afx_msg void OnBnClickedCcancel();
	afx_msg void OnBnClickedSendbtn();
	CString m_recv;
	CString m_send;
protected:
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
