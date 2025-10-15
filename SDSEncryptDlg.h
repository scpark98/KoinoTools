#pragma once
#include "afxdialogex.h"

#include "Common/ResizeCtrl.h"
#include "Common/SeedProvider.h"
#include "Common/CEdit/SCEdit/SCEdit.h"

// CSDSEncryptDlg 대화 상자

class CSDSEncryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSDSEncryptDlg)

public:
	CSDSEncryptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSDSEncryptDlg();

	CResizeCtrl			m_resize;
	SeedProvider		m_sp;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SDS_ENCRYPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CSCEdit m_edit_encrypted_text;
	CSCEdit m_edit_plain_text;
	afx_msg void OnEnChangeEditEncryptedText();
	afx_msg void OnEnChangeEditPlainText();
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};
