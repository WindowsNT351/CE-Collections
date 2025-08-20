// aboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "menu3.h"
#include "aboutDlg.h"


// CaboutDlg 对话框

IMPLEMENT_DYNAMIC(CaboutDlg, CDialog)

CaboutDlg::CaboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CaboutDlg::IDD, pParent)
{

}

CaboutDlg::~CaboutDlg()
{
}

void CaboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CaboutDlg, CDialog)
END_MESSAGE_MAP()


// CaboutDlg 消息处理程序

TCHAR* AsciiToUnicode(char *str)
{
	DWORD dwNum = 0;
	dwNum = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	TCHAR* pwHostName = new TCHAR[dwNum];
	MultiByteToWideChar(CP_ACP, 0, str, -1, pwHostName, dwNum);
	return pwHostName;
}

BOOL CaboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化


	MessageBox(L"aaaaa");

	ShowWindow(SW_NORMAL);
	CRect rtDesk;
	CRect rtDlg;
	::GetWindowRect(::GetDesktopWindow(), &rtDesk);
	GetWindowRect(&rtDlg);
	int iXpos = rtDesk.Width() / 2 - rtDlg.Width() / 2;
	int iYpos = rtDesk.Height() / 2 - rtDlg.Height() / 2;
	SetWindowPos(NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);

	TCHAR lpPath[255] = { 0 };
	wsprintf(lpPath, L"..\\CECD0.20");

	HANDLE hFile = CreateFile(
		lpPath,                        // 文件名
		GENERIC_READ,                    // 读权限
		0,                               // 不共享
		NULL,                            // 安全属性
		OPEN_EXISTING,                   // 打开已存在的文件
		FILE_ATTRIBUTE_NORMAL,           // 文件属性
		NULL                             // 模板文件的句柄
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(L"ERROR");
		return 1;
	}
	// 确定文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 分配内存读取文件
	char buffer[255] = { 0 };
	DWORD dwRead = 0;
	// 读取文件内容
	if (ReadFile(hFile, buffer, dwFileSize, &dwRead, NULL)) {
		buffer[dwFileSize] = '\0'; // 添加字符串终止符
	}
	// 关闭文件句柄
	CloseHandle(hFile);

	GetDlgItem(IDC_STATIC1)->SetWindowText(AsciiToUnicode(buffer));

	GetDlgItem(IDC_EDIT1)->SetWindowText( L"Developer\r\n"
							L"	351			351Workshop@Bilibili\r\n"
							L"Sponsor\r\n"
							L"	IBM			IBM_Official@Bilibili\r\n"
							L"	Dr.Quest			蓬岸@知乎\r\n"
							L"Image Providing\r\n"
							L"	Alexis			lucss21a@Discord\r\n"
							L"Artworks\r\n"
							L"	WinPad			とある科学の手机副屏@Bilibili\r\n"
							L"Guidance and Testing\r\n"
							L"	Inter			-INTER_INIT-@Bilibili\r\n"
							L"	GoldApple			不务正业的金苹果@Bilibili\r\n"
							L"	Zesa			LinuxMEMZ@Bilibili\r\n"
							L"	DZY			DZY20070614@Bilibili\r\n");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
