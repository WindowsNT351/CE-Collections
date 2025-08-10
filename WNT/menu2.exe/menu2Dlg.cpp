
// menu2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "menu2.h"
#include "menu2Dlg.h"
#include "afxdialogex.h"
#include "DigHelp.h"
#include "about.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXTREE 255

// Cmenu2Dlg 对话框



Cmenu2Dlg::Cmenu2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MENU2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmenu2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}

BEGIN_MESSAGE_MAP(Cmenu2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &Cmenu2Dlg::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDOK, &Cmenu2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Cmenu2Dlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &Cmenu2Dlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &Cmenu2Dlg::OnNMClickSyslink2)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK3, &Cmenu2Dlg::OnNMClickSyslink3)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &Cmenu2Dlg::OnNMDblclkTree1)
END_MESSAGE_MAP()


// Cmenu2Dlg 消息处理程序


void Cmenu2Dlg::MyExpandTree(HTREEITEM hTreeItem)
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
BOOL Cmenu2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetCurrentDirectory(MAX_PATH, lpPath);
	wsprintf(lpPath, L"%s\\menu.enus", lpPath);
	

	SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	//treeStr[0] = m_tree.InsertItem(L"Windows CE", 1, 0, TVI_ROOT);
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

	DigHelp *dlg = new DigHelp;
	dlg->Create(IDD_DIALOG1, NULL);
	dlg->ShowWindow(SW_SHOWNORMAL);//SW_SHOW是对话框的显示方式

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmenu2Dlg::OnPaint()
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
		//CDialogEx::OnPaint();
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cmenu2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cmenu2Dlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void doCommand(TCHAR *cmd, TCHAR *para)
{
	ShellExecute(NULL, L"open", cmd, para, NULL, SW_SHOWNORMAL);
}

void doCommandD(TCHAR *cmd, TCHAR *para, TCHAR *dir)
{
	ShellExecute(NULL, L"open", cmd, para, dir, SW_SHOWNORMAL);
}

void Cmenu2Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
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
		doCommandD(currentINICmd, currentINIPara, currentINIDir);
}


void Cmenu2Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}


void Cmenu2Dlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	doCommand(L"..\\DOCS\\help.txt", NULL);
	*pResult = 0;
}


void Cmenu2Dlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	doCommand(L"..\\DOCS\\vers.bmp", NULL);
	*pResult = 0;
}


void Cmenu2Dlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//doCommand(L".\\about.exe", NULL);

	about *dlg = new about;
	dlg->Create(IDD_ABOUT_DIALOG, NULL);
	dlg->ShowWindow(SW_SHOWNORMAL);//SW_SHOW是对话框的显示方式

	*pResult = 0;
}





void Cmenu2Dlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	Cmenu2Dlg::OnBnClickedOk();
	*pResult = 0;
}
