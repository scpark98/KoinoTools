
// KoinoToolsDlg.h: 헤더 파일
//

#pragma once

#include "Common/device/keyboard/SCKeyInput/SCKeyInput.h"
#include "Common/ResizeCtrl.h"
#include "Common/ControlSplitter.h"
#include "Common/CStatic/SCStatic/SCStatic.h"
#include "Common/CTreeCtrl/SCTreeCtrl/SCTreeCtrl.h"
#include "Common/CListCtrl/CVtListCtrlEx/VtListCtrlEx.h"
#include "Common/CEdit/SCEdit/SCEdit.h"
#include "Common/CEdit/RichEditCtrlEx/RichEditCtrlEx.h"

// CKoinoToolsDlg 대화 상자
class CKoinoToolsDlg : public CDialogEx
{
// 생성입니다.
public:
	CKoinoToolsDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	enum TIMER_ID
	{
		timer_release_block_input = 0,
	};

	CResizeCtrl				m_resize;
	CSCKeyInput				m_key_input;

	std::deque<CString>		m_files;

	CString					m_product;
	CString					m_reg_product_root = _T("Software\\Koino\\KoinoTools\\product");

	//CString					m_last_edit_product;//LvnEndLabelEditList 주석 참조
	CString					m_mt_path;            //mt.exe fullpath
	CString					m_signtool_path;      //signtool.exe fullpath
	CString					m_manifest_folder;    //manifest 폴더 fullpath
	CString					m_fingerprint;
	CString					m_password;

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
	void					thread_codesign_manifest(bool apply_manifest);
	bool					m_in_codesigning = false;

	//product 선택, 각 항목 경로 및 존재여부 체크
	bool					check_valid_condition();

	//codesign이 시작되면 이 thread가 구동되고 "토큰 로그온" 창이 뜨면 자동으로 암호를 입력해 준다.
	//n개의 파일에 대한 codesign이 모두 끝나면 thread가 종료된다.
	void					thread_auto_password_input();
	bool					m_thread_auto_password_input = false;				//thread가 구동중인지
	bool					m_thread_auto_password_input_paused = true;			//thread를 일시정지 시킬지 여부
	bool					m_thread_auto_password_input_terminated = true;		//thread가 정상적으로 종료되었는지 판별

	void					thread_run_codesign(CString cmd);

	void					init_tree();
	int						get_icon_index(CString product_name);

	enum LIST_COLUMN
	{
		col_item = 0,
		col_value,
		col_desc,
	};
	void					init_list();

	void					init_rich();


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
	CSCTreeCtrl m_tree;
	afx_msg void OnTvnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnEndLabelEditList(NMHDR* pNMHDR, LRESULT* pResult);
	CRichEditCtrlEx m_rich;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMRClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnEndLabelEditTree(NMHDR* pNMHDR, LRESULT* pResult);
	CSCEdit m_edit1;
	afx_msg void OnLvnBeginLabelEditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelChangingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuLmmSDSEncrypt();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuTreeDelete();
	afx_msg void OnMenuTreeNewItem();
	afx_msg void OnMenuTreeRename();
	CControlSplitter m_vert_splitter;
	afx_msg void OnMenuTreeViewRegEdit();
	afx_msg void OnMenuDragFullWindows();
};
