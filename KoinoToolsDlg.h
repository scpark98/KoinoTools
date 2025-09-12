
// KoinoToolsDlg.h: 헤더 파일
//

#pragma once

#include "Common/device/keyboard/SCKeyInput/SCKeyInput.h"
#include "Common/ResizeCtrl.h"
#include "Common/CStatic/SCStatic/SCStatic.h"
#include "Common/CListCtrl/CVtListCtrlEx/VtListCtrlEx.h"

// CKoinoToolsDlg 대화 상자
class CKoinoToolsDlg : public CDialogEx
{
// 생성입니다.
public:
	CKoinoToolsDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum TIMER_ID
	{
		timer_ = 0,
	};

	CResizeCtrl				m_resize;
	CSCKeyInput				m_ki;

	std::deque<CString>		m_files;

	enum PROJECT_ID
	{
		project_lmm_service,
		project_lmm_se_service,
		project_anysupport,
		project_helpu,
		project_helpu_jp,
		project_pcanypro,
		project_remote_sdk,
	};

	enum ACTION_ID
	{
		action_no_action = 0,
		action_codesign_manifest,
		action_codesign_no_manifest,
	};
	int						m_action = action_no_action;
	void					codesign_manifest(bool manifest);


	void					init_tree();

	enum LIST_COLUMN
	{
		col_item = 0,
		col_value,
	};
	void					init_list();


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KOINOTOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CSCStatic m_static_code_sign_manifest;
	CSCStatic m_static_code_sign_no_manifest;
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CVtListCtrlEx m_list;
};
