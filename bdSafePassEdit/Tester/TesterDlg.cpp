// TesterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Tester.h"
#include "TesterDlg.h"
#include ".\testerdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTesterDlg �Ի���



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


// CTesterDlg ��Ϣ�������

BOOL CTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTesterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
