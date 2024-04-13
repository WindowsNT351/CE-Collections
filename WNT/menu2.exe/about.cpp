// about.cpp: 实现文件
//

#include "pch.h"
#include "menu2.h"
#include "about.h"
#include "afxdialogex.h"


// about 对话框

IMPLEMENT_DYNAMIC(about, CDialogEx)

about::about(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUT_DIALOG, pParent)
{

}

about::~about()
{
}

void about::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICVER, m_textver);
}


BEGIN_MESSAGE_MAP(about, CDialogEx)
	ON_BN_CLICKED(IDOK, &about::OnBnClickedOk)
END_MESSAGE_MAP()


// about 消息处理程序


void about::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

TCHAR* AsciiToUnicode(char *str)
{
	DWORD dwNum = 0;
	dwNum = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	TCHAR* pwHostName = new TCHAR[dwNum];
	MultiByteToWideChar(CP_ACP, 0, str, -1, pwHostName, dwNum);
	return pwHostName;
}

TCHAR lpPath__[255] = { 0 };
BOOL about::OnInitDialog()
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

	wsprintf(lpPath__, L"..\\CECD0.20");

	HANDLE hFile = CreateFile(
		lpPath__,                        // 文件名
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

	m_textver.SetWindowText(AsciiToUnicode(buffer));



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
