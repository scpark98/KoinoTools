
// KoinoToolsDlg.h: 헤더 파일
//

#pragma once

#include "Common/device/keyboard/SCKeyInput/SCKeyInput.h"
#include "Common/ResizeCtrl.h"
#include "Common/ControlSplitter.h"
#include "Common/CStatic/SCStatic/SCStatic.h"
#include "Common/CTreeCtrl/SCTreeCtrl/SCTreeCtrl.h"
#include "Common/CListCtrl/CSCListCtrl/SCListCtrl.h"
#include "Common/CEdit/SCEdit/SCEdit.h"
#include "Common/CEdit/RichEditCtrlEx/RichEditCtrlEx.h"
#include "Common/log/SCLog/SCLog.h"
#include "Common/messagebox/CSCMessageBox/SCMessageBox.h"

//codesign 진행/완료(워커 스레드)를 UI 스레드로 넘겨 처리하기 위한 사용자 메시지.
//작업표시줄 progress(ITaskbarList3)는 COM STA 스레드(=UI 스레드)에서 호출해야 하므로 통지를 UI 스레드로 넘긴다.
#define WM_APP_CODESIGN_PROGRESS	(WM_APP + 1)	//wParam = 진행률(0~100)
#define WM_APP_CODESIGN_DONE		(WM_APP + 2)	//wParam = 성공(1)/실패(0)
//메시지박스를 항상 UI 스레드에서 띄우기 위한 메시지. 워커 스레드에서 SendMessage 로 보내면 UI 스레드로 마샬링되어
//공유 멤버 m_msgbox 를 안전하게 사용할 수 있다. wParam = const CString*(본문), lParam = 버튼/아이콘 타입.
#define WM_APP_SHOW_MSGBOX			(WM_APP + 3)

struct ITaskbarList3;	//작업표시줄 progress 표시용(Win7+). 정의는 .cpp 의 <shobjidl.h> 에서.

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

	//드롭으로 받은 m_files 를 파일별로 서명한다. manifest 여부는 is_manifest_required() 로 파일마다 판별.
	void					thread_codesign();
	//해당 실행파일이 반드시 with-Manifest 로 서명되어야 하는지(LMMAgent.exe 등 지정 목록). 목록 외는 No-Manifest.
	bool					is_manifest_required(const CString& filename) const;
	bool					m_in_codesigning = false;

	//작업표시줄 progress 로 codesign 완료를 알린다. 완료 시 100%, 사용자가 창을 activate 하면 0%(제거).
	ITaskbarList3*			m_taskbar = nullptr;			//OnInitDialog 에서 생성, OnDestroy 에서 Release
	bool					m_com_initialized = false;		//CoInitialize 성공 여부(OnDestroy 에서 짝 맞춰 CoUninitialize)
	bool					m_taskbar_progress_shown = false;
	//codesign 전체 완료 여부. 완료(=OnCodesignDone) 후에만 사용자 activate/클릭으로 progress 를 0%로 리셋한다.
	//(코드사인 중에는 토큰창이 닫히며 앱이 우발적으로 재활성화되므로, 그때 리셋되면 진행률이 도중에 지워진다)
	bool					m_codesign_finished = false;
	void					set_taskbar_progress(int percent, bool error = false);	//progress 값/상태 설정(error 면 빨강)
	void					reset_taskbar_progress();		//activate 시 progress 제거
	//워커 스레드가 각 단계/완료 시 PostMessage 로 보내고, UI 스레드에서 progress 를 갱신한다(COM STA 이므로 UI 스레드 필수).
	afx_msg LRESULT			OnCodesignProgress(WPARAM wParam, LPARAM lParam);	//wParam=진행률(0~100)
	afx_msg LRESULT			OnCodesignDone(WPARAM wParam, LPARAM lParam);		//wParam=성공(1)/실패(0): 100%(또는 error) + 2번 깜빡임

	//모든 메시지박스를 이 공유 인스턴스로 띄운다(타이틀/테마 유지). OnInitDialog 에서 create() 로 초기화.
	CSCMessageBox			m_msgbox;
	//AfxMessageBox 대체. 어느 스레드에서 호출해도 WM_APP_SHOW_MSGBOX 로 UI 스레드에서 m_msgbox 를 띄우고 버튼 ID 를 반환한다.
	int						show_message(const CString& text, int type = MB_OK);
	afx_msg LRESULT			OnShowMsgbox(WPARAM wParam, LPARAM lParam);

	//product 선택, 각 항목 경로 및 존재여부 체크
	bool					check_valid_condition();

	//codesign이 시작되면 이 thread가 구동되고 "토큰 로그온" 창이 뜨면 자동으로 암호를 입력해 준다.
	//n개의 파일에 대한 codesign이 모두 끝나면 thread가 종료된다.
	void					thread_auto_password_input();
	bool					m_thread_auto_password_input = false;				//thread가 구동중인지
	bool					m_thread_auto_password_input_paused = true;			//thread를 일시정지 시킬지 여부
	bool					m_thread_auto_password_input_terminated = true;		//thread가 정상적으로 종료되었는지 판별

	void					thread_run_codesign(CString cmd);

	//codesign 각 단계(mt/signtool)가 0이 아닌 종료코드를 반환하면 로그창(빨강)+로그파일+메시지박스로 실패를 알린다.
	void					report_codesign_step_error(LPCTSTR step_name, DWORD exit_code, const CString& output);

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
	HICON m_hIcon;			//큰 아이콘 (SM_CXICON) — 작업표시줄 / Alt-Tab.
	HICON m_hIcon_small;	//작은 아이콘 (SM_CXSMICON) — 타이틀바 / 작업표시줄 축소 표시.

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
	//No-Manifest/Manifest 두 드롭 영역을 하나로 통합. 남는 단일 드롭 타깃(리소스 ID 는 기존 NO_MANIFEST 유지).
	CSCStatic			m_static_code_sign;
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CSCListCtrl m_list;
	CSCTreeCtrl m_tree;
	afx_msg void OnTvnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnEndLabelEditList(NMHDR* pNMHDR, LRESULT* pResult);
	CRichEditCtrlEx m_rich;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void show_tree_context_menu(CPoint point_screen);
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
	afx_msg void OnMenuTreeServiceStop();
	afx_msg void OnMenuTreeServiceRestart();
	afx_msg void OnMenuTreeServiceDelete();
	afx_msg void OnMenuTreeLogFolder();
	afx_msg void OnMenuTreeDeleteRegUrlSchemeInfo();
};
