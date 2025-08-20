#pragma once


// CaboutDlg 对话框

class CaboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CaboutDlg)

public:
	CaboutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CaboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
