// menu3Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// Cmenu3Dlg 对话框
class Cmenu3Dlg : public CDialog
{
// 构造
public:
	Cmenu3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MENU3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	void MyExpandTree(HTREEITEM hTreeItem);
	CTreeCtrl m_tree;
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CRect m_rectbtn1;
	CRect m_rectbtn2;
	CRect m_rectbtn3;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
