#pragma once


// CaboutDlg �Ի���

class CaboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CaboutDlg)

public:
	CaboutDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CaboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
