#pragma once


// DigHelp 对话框

class DigHelp : public CDialogEx
{
	DECLARE_DYNAMIC(DigHelp)

public:
	DigHelp(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DigHelp();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CStatic m_text1;
};
