
// SeverDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "Sever.h"
#include "SeverDlg.h"
#include "afxdialogex.h"

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

// CSeverDlg 对话框
CSeverDlg::CSeverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEVER_DIALOG, pParent)
	, m_status(_T(""))
	, m_recv(_T(""))
	, m_send(_T(""))
{
	//m_send.Empty();  //开始就将发送狂清空
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSeverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_RECV, m_recv);
	DDX_Text(pDX, IDC_SEND, m_send);
}

BEGIN_MESSAGE_MAP(CSeverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STARTRUN, &CSeverDlg::OnStartrun)
//	ON_NOTIFY(BCN_DROPDOWN, IDC_STARTRUN, &CSeverDlg::OnSocket)
//ON_NOTIFY(BCN_DROPDOWN, IDC_STARTRUN, &CSeverDlg::OnSocket)
//ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_STARTRUN, &CSeverDlg::OnSendbtn)
ON_BN_CLICKED(IDCCANCEL, &CSeverDlg::OnBnClickedCcancel)
ON_BN_CLICKED(IDC_SENDBTN, &CSeverDlg::OnBnClickedSendbtn)
ON_MESSAGE(WM_SOCKET, &CSeverDlg::OnSocket)
ON_BN_CLICKED(IDC_BUTTON1, &CSeverDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &CSeverDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSeverDlg 消息处理程序

BOOL CSeverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

void CSeverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSeverDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标 //显示。
HCURSOR CSeverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//开启网络服务事件
void CSeverDlg::OnStartrun()
{
	// TODO: 在此添加控件通知处理程序代码

	//加载Winsock dll
	WORD version = MAKEWORD(2, 0);
	WSADATA wsadata;
	if (WSAStartup(version, &wsadata))
	{
		MessageBox(TEXT("加载Winsock dll 失败"));
		return;
	}

	//创建套接字
	socket_server = socket(AF_INET, SOCK_STREAM, NULL);
	if (INVALID_SOCKET == socket_server)
	{
		MessageBox(TEXT("套接字创建失败"));
		return;
	}

	char hostname[20] = "";
	if (gethostname(hostname, 20))
	{
		MessageBox(TEXT("主机名获取失败"));
		return;
	}

	//得到电脑的主机名
	/*CString temp = (CString)hostname;
	MessageBox(temp);*/

	hostent *htent = gethostbyname(hostname);
	if (htent == NULL)
	{
		MessageBox(TEXT("主机IP获取失败"));
		return;
	}

	LPSTR lpAddr = htent->h_addr_list[0];
	IN_ADDR inAddr;
	memmove(&inAddr, lpAddr, 4);
	char* ipAddress = inet_ntoa(inAddr);

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.S_un.S_addr = inet_addr(ipAddress);

	//addr.sin_addr.S_un.S_addr);

	//检查得到的IP地址
	/*CString temp = (CString)ipAddress;
	MessageBox(temp);*/

	//绑定套接字
	if (bind(socket_server, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		MessageBox(TEXT("绑定IP和port出错"));
		return;
	}

	//监听
	if (listen(socket_server, SOMAXCONN))
	{
		MessageBox(TEXT("监听出错"));
		return;
	}

	if (WSAAsyncSelect(socket_server, this->m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ))
	{
		MessageBox(TEXT("异步设置出错"));
		return;
	}
	MessageBox(TEXT("开启服务成功"));
}

//断开连接
void CSeverDlg::OnBnClickedCcancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (WSACleanup())
	{
		MessageBox(TEXT("卸载Winsock dll失败"));
		return;
	}
	if (socket_server)
	{
		closesocket(socket_server);
	}
	if (socket_client)
	{
		closesocket(socket_client);
	}
	MessageBox(_T("成功断开！"));
}

//发送信息
void CSeverDlg::OnBnClickedSendbtn()
{
	// TODO: 在此添加控件通知处理程序代码	
	UpdateData(true);  //将控件中的值更新到变量中

	if (m_send.IsEmpty())
	{
		MessageBox(_T("发送消息不能为空！"));
		return;
	}

	if (SOCKET_ERROR == send(socket_client, (char*)m_send.GetBuffer(), 100, NULL))
	{
		MessageBox(TEXT("消息发送出错"));
		return;
	}
	m_recv += "服务器端：";
	m_recv += m_send;
	m_recv += "\r\n";
	m_send.Empty();  //将发送框内清空
	UpdateData(false);  //将变量的值更新到控件中
}

//响应连接与读取的函数
afx_msg LRESULT CSeverDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
		case FD_ACCEPT:
		{
			SOCKADDR_IN addr;
			int len = sizeof(SOCKADDR_IN);
			socket_client = accept(socket_server, (sockaddr*)&addr, &len);
			if (socket_client == INVALID_SOCKET)
			{
				MessageBox(TEXT("客户端套接字创建出错"));
				return 0;
			}
			//获取对方ip
			m_status += "客户端IP：";
			m_status += inet_ntoa(addr.sin_addr);
			MessageBox(_T("已经与客户端连接！"));
			UpdateData(false);
			break;
		}

		case FD_READ:
		{
			UpdateData(true);
			char buff[1024] = "";
			if (!recv(socket_client, buff, 1024, NULL))
			{
				MessageBox(TEXT("数据接收出错"));
				return 0;
			}
			m_recv += "客户端：";
			m_recv += (LPCTSTR)buff;   //这里得用LPCTSTR来强转，用CString不行
			m_recv += "\r\n";
			UpdateData(false);
		}
	}
}

//退出
void CSeverDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

//关于
void CSeverDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg cbd;
	cbd.DoModal();
}
