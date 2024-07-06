#pragma once


// about 对话框

class about : public CDialogEx
{
	DECLARE_DYNAMIC(about)

public:
	about(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~about();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CStatic m_textver;
	afx_msg void OnStnClicked65535();
};
