// aboutDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "menu3.h"
#include "aboutDlg.h"


// CaboutDlg �Ի���

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


// CaboutDlg ��Ϣ�������

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


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
		lpPath,                        // �ļ���
		GENERIC_READ,                    // ��Ȩ��
		0,                               // ������
		NULL,                            // ��ȫ����
		OPEN_EXISTING,                   // ���Ѵ��ڵ��ļ�
		FILE_ATTRIBUTE_NORMAL,           // �ļ�����
		NULL                             // ģ���ļ��ľ��
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(L"ERROR");
		return 1;
	}
	// ȷ���ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// �����ڴ��ȡ�ļ�
	char buffer[255] = { 0 };
	DWORD dwRead = 0;
	// ��ȡ�ļ�����
	if (ReadFile(hFile, buffer, dwFileSize, &dwRead, NULL)) {
		buffer[dwFileSize] = '\0'; // ����ַ�����ֹ��
	}
	// �ر��ļ����
	CloseHandle(hFile);

	GetDlgItem(IDC_STATIC1)->SetWindowText(AsciiToUnicode(buffer));

	GetDlgItem(IDC_EDIT1)->SetWindowText( L"Developer\r\n"
							L"	351			351Workshop@Bilibili\r\n"
							L"Sponsor\r\n"
							L"	IBM			IBM_Official@Bilibili\r\n"
							L"	Dr.Quest			�@֪��\r\n"
							L"Image Providing\r\n"
							L"	Alexis			lucss21a@Discord\r\n"
							L"Artworks\r\n"
							L"	WinPad			�Ȥ����ѧ���ֻ�����@Bilibili\r\n"
							L"Guidance and Testing\r\n"
							L"	Inter			-INTER_INIT-@Bilibili\r\n"
							L"	GoldApple			������ҵ�Ľ�ƻ��@Bilibili\r\n"
							L"	Zesa			LinuxMEMZ@Bilibili\r\n"
							L"	DZY			DZY20070614@Bilibili\r\n");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
