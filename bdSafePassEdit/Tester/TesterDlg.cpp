// TesterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Tester.h"
#include "TesterDlg.h"
#include ".\testerdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTesterDlg 对话框



CTesterDlg::CTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_PassEd);
	DDX_Control(pDX, IDC_EDIT3, m_PassEd2);
}

BEGIN_MESSAGE_MAP(CTesterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDOK2, OnBnClickedOk2)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
END_MESSAGE_MAP()


// CTesterDlg 消息处理程序

BOOL CTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTesterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_PASS, m_PassEd.GetPassword());
}

void CTesterDlg::OnBnClickedOk2()
{
	SetDlgItemText(IDC_PASS, m_PassEd2.GetPassword());
}

void CTesterDlg::OnBnClickedButton1()
{
	m_PassEd.Clear();
}

void CTesterDlg::OnBnClickedButton2()
{
	m_PassEd2.Clear();
}

void CTesterDlg::OnBnClickedButton3()
{
	m_PassEd.InputChar('1', 0);
}

void CTesterDlg::OnBnClickedButton4()
{
	m_PassEd.InputChar('2', 0);
}

void CTesterDlg::OnBnClickedButton5()
{
	m_PassEd.InputChar('3', 0);
}

void CTesterDlg::OnBnClickedButton6()
{
	m_PassEd.InputChar(0, VK_BACK);
}
