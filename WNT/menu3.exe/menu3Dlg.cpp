// menu3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "menu3.h"
#include "menu3Dlg.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit1;
	CStatic m_textver;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_STATIC1, m_textver);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cmenu3Dlg 对话框




Cmenu3Dlg::Cmenu3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cmenu3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmenu3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}

BEGIN_MESSAGE_MAP(Cmenu3Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &Cmenu3Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &Cmenu3Dlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &Cmenu3Dlg::OnNMDblclkTree1)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// Cmenu3Dlg 消息处理程序

void Cmenu3Dlg::MyExpandTree(HTREEITEM hTreeItem)
{
	if (!m_tree.ItemHasChildren(hTreeItem))//如果树控件根节点没有子节点则返回
	{
		return;
	}
	HTREEITEM hNextItem = m_tree.GetChildItem(hTreeItem);//若树控件的根节点有子节点则获取根节点的子节点
	while (hNextItem != NULL)//若有
	{
		MyExpandTree(hNextItem);//递归，展开子节点下的所有子节点
		hNextItem = m_tree.GetNextItem(hNextItem, TVGN_NEXT);//获取根节点的下一个子节点
	}
	m_tree.Expand(hTreeItem, TVE_EXPAND);//展开节点
}

TCHAR lpPath[255] = { 0 };
HTREEITEM *treeStr;
BOOL Cmenu3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	wsprintf(lpPath, L".\\menu.enus");

	GetDlgItem(IDC_STATICBTN1)->GetWindowRect(&m_rectbtn1);
	ScreenToClient(&m_rectbtn1);
	GetDlgItem(IDC_STATICBTN2)->GetWindowRect(&m_rectbtn2);
	ScreenToClient(&m_rectbtn2);
	GetDlgItem(IDC_STATICBTN3)->GetWindowRect(&m_rectbtn3);
	ScreenToClient(&m_rectbtn3);

	//SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	int iNodeNum = GetPrivateProfileInt(L"0", L"nodeNum", 255, lpPath);
	treeStr = new HTREEITEM[iNodeNum]();
	for (int i = 0; i < iNodeNum; i++)
	{
		TCHAR iniCBuffer[255] = { 0 };
		wsprintf(iniCBuffer, L"%d", i);
		int currentFat = GetPrivateProfileInt(iniCBuffer, L"fat", -1, lpPath);
		if (currentFat == -1)
			continue;//201C101
		
		TCHAR currentName[255] = { 0 };
		GetPrivateProfileString(iniCBuffer, L"name", NULL, currentName, 255, lpPath);
		if (currentFat == -2)
			treeStr[0] = m_tree.InsertItem(currentName, 1, 0, TVI_ROOT);
		else
			treeStr[i] = m_tree.InsertItem(currentName, 1, 0, treeStr[currentFat]);
	}

	MyExpandTree(m_tree.GetRootItem());


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cmenu3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmenu3Dlg::OnPaint()
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
		//CDialog::OnPaint();
		CPaintDC   dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC   dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);  //对话框的背景图片  

		BITMAP   bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap   *pbmpOld = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR Cmenu3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cmenu3Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void Cmenu3Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
	HTREEITEM hselected = m_tree.GetSelectedItem();
	int currentTreeID = 0;
	int iNodeNum = GetPrivateProfileInt(L"0", L"nodeNum", 255, lpPath);
	for (; currentTreeID < iNodeNum; currentTreeID++)
	{
		if (hselected == treeStr[currentTreeID])
			break;
	}

	TCHAR currentINIHead[255] = { 0 };
	TCHAR currentINICmd[255] = { 0 };
	TCHAR currentINIPara[255] = { 0 };
	TCHAR currentINIDir[255] = { 0 };
	wsprintf(currentINIHead, L"%d", currentTreeID);
	GetPrivateProfileString(currentINIHead, L"cmd", NULL, currentINICmd, 255, lpPath);
	GetPrivateProfileString(currentINIHead, L"para", NULL, currentINIPara, 255, lpPath);
	GetPrivateProfileString(currentINIHead, L"dir", NULL, currentINIDir, 255, lpPath);
	if (currentINICmd[0] != NULL)
	{
		ShellExecute(NULL, L"open", currentINICmd, currentINIPara, currentINIDir, SW_SHOWNORMAL);
	}
}

void Cmenu3Dlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedOk();
}

HBRUSH Cmenu3Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (pWnd->GetDlgCtrlID() == IDC_STATICBTN1 ||
		pWnd->GetDlgCtrlID() == IDC_STATICBTN2 ||
		pWnd->GetDlgCtrlID() == IDC_STATICBTN3)
	{
		pDC->SetTextColor(RGB(64,148,199));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void Cmenu3Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (point.x > m_rectbtn1.left && point.x < m_rectbtn1.right && point.y < m_rectbtn1.bottom && point.y > m_rectbtn1.top)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (point.x > m_rectbtn2.left && point.x < m_rectbtn2.right && point.y < m_rectbtn2.bottom && point.y > m_rectbtn2.top)
	{
		ShellExecute(NULL, L"open", L"..\\DOCS\\HELP.txt", NULL, NULL, SW_SHOWNORMAL);
	}
	else if (point.x > m_rectbtn3.left && point.x < m_rectbtn3.right && point.y < m_rectbtn3.bottom && point.y > m_rectbtn3.top)
	{
		ShellExecute(NULL, L"open", L"..\\DOCS\\VERS.bmp", NULL, NULL, SW_SHOWNORMAL);
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void Cmenu3Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ((point.x > m_rectbtn1.left && point.x < m_rectbtn1.right && point.y < m_rectbtn1.bottom && point.y > m_rectbtn1.top) ||
		(point.x > m_rectbtn2.left && point.x < m_rectbtn2.right && point.y < m_rectbtn2.bottom && point.y > m_rectbtn2.top) ||
		(point.x > m_rectbtn3.left && point.x < m_rectbtn3.right && point.y < m_rectbtn3.bottom && point.y > m_rectbtn3.top))
	{
		HCURSOR hCursor;
		hCursor = ::LoadCursor ( NULL, IDC_HAND );
		if(hCursor == NULL)
			hCursor = LoadCursor ( AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR1) );
		::SetCursor ( hCursor );
		Sleep(10);
	}
	CDialog::OnMouseMove(nFlags, point);
}

TCHAR* AsciiToUnicode(char *str)
{
	DWORD dwNum = 0;
	dwNum = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	TCHAR* pwHostName = new TCHAR[dwNum];
	MultiByteToWideChar(CP_ACP, 0, str, -1, pwHostName, dwNum);
	return pwHostName;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

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
	char buffer[255] = { 0 };
	HANDLE hFile = CreateFile(
		lpPath,                        // 文件名
		GENERIC_READ,                    // 读权限
		0,                               // 不共享
		NULL,                            // 安全属性
		OPEN_EXISTING,                   // 打开已存在的文件
		FILE_ATTRIBUTE_NORMAL,           // 文件属性
		NULL                             // 模板文件的句柄
	);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		//MessageBox(L"Can't read CECD0.20");
		//return 1;

	}
	else
	{
		// 确定文件大小
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		// 分配内存读取文件
		DWORD dwRead = 0;
		// 读取文件内容
		if (ReadFile(hFile, buffer, dwFileSize, &dwRead, NULL)) 
		{
			buffer[dwFileSize] = '\0'; // 添加字符串终止符
		}
		// 关闭文件句柄
		CloseHandle(hFile);
	}


	GetDlgItem(IDC_STATIC1)->SetWindowText(AsciiToUnicode(buffer));

	GetDlgItem(IDC_EDIT1)->SetWindowText( 
							L"Developer\r\n"
							L"	351			351Workshop@Bilibili\r\n"
							L"Sponsor\r\n"
							L"	IBM			IBM_Official@Bilibili\r\n"
							L"	Dr.Quest		蓬岸@知乎\r\n"
							L"Image Providing\r\n"
							L"	Alexis			lucss21a@Discord\r\n"
							L"	IBM			IBM_Official@Bilibili\r\n"
							L"	Dr.Quest		蓬岸@知乎\r\n"
							L"Artworks\r\n"
							L"	WinPad			とある科学の手机副屏@Bilibili\r\n"
							L"Guidance and Testing\r\n"
							L"	Inter			-INTER_INIT-@Bilibili\r\n"
							L"	GoldApple		不务正业的金苹果@Bilibili\r\n"
							L"	Zesa			Zesa_IO1i0lO@Bilibili\r\n"
							L"	DZY			DZY20070614@Bilibili\r\n");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
