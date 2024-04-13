// DigHelp.cpp: 实现文件
//

#include "pch.h"
#include "menu2.h"
#include "DigHelp.h"
#include "afxdialogex.h"


// DigHelp 对话框

IMPLEMENT_DYNAMIC(DigHelp, CDialogEx)

DigHelp::DigHelp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

DigHelp::~DigHelp()
{
}

void DigHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_text1);
}


BEGIN_MESSAGE_MAP(DigHelp, CDialogEx)
	ON_BN_CLICKED(IDOK, &DigHelp::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DigHelp::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &DigHelp::OnBnClickedButton1)
END_MESSAGE_MAP()


// DigHelp 消息处理程序


void DigHelp::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void DigHelp::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

int beforeNum = 0;
TCHAR lpPath_[255] = { 0 };
void DigHelp::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int maxInfo = GetPrivateProfileInt(L"sug", L"maxInfo", -1, lpPath_);
	TCHAR currentInfo[255] = { 0 };
	srand(time(0));
	int curNum = 0;
	do {
		curNum = rand() % maxInfo; // 生成随机数
	} while (curNum==beforeNum);
	beforeNum = curNum;
	TCHAR currentID[255] = { 0 };
	wsprintf(currentID, L"%d", curNum);
	GetPrivateProfileString(L"sug", currentID, NULL, currentInfo, 255, lpPath_);
	m_text1.SetWindowText(currentInfo);
}


BOOL DigHelp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowWindow(SW_NORMAL);
	CRect rtDesk;
	CRect rtDlg;
	::GetWindowRect(::GetDesktopWindow(), &rtDesk);
	GetWindowRect(&rtDlg);
	int iXpos = rtDesk.Width() / 2 - rtDlg.Width() / 2;
	int iYpos = rtDesk.Height() / 2 - rtDlg.Height() / 2;
	SetWindowPos(NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);


	GetCurrentDirectory(MAX_PATH, lpPath_);
	wsprintf(lpPath_, L"%s\\menu.enus", lpPath_);

	int maxInfo = GetPrivateProfileInt(L"sug", L"maxInfo", -1, lpPath_);
	TCHAR currentInfo[255] = { 0 };
	srand(time(0));
	int randomNum = rand()%maxInfo; // 生成随机数
	beforeNum = randomNum;
	TCHAR currentID[255] = { 0 };
	wsprintf(currentID, L"%d", randomNum);
	GetPrivateProfileString(L"sug", currentID, NULL, currentInfo, 255, lpPath_);
	m_text1.SetWindowText(currentInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
