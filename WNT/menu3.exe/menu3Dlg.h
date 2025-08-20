// menu3Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// Cmenu3Dlg �Ի���
class Cmenu3Dlg : public CDialog
{
// ����
public:
	Cmenu3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MENU3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
