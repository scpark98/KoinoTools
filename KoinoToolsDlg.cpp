
// KoinoToolsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "KoinoTools.h"
#include "KoinoToolsDlg.h"
#include "afxdialogex.h"

#include <thread>
#include <winsvc.h>
#include <shobjidl.h>	//ITaskbarList3(작업표시줄 progress 표시)

#include "Common/Functions.h"
#include "Common/system/CCmdLine/CmdLine.h"
#include "SDSEncryptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKoinoToolsDlg 대화 상자



CKoinoToolsDlg::CKoinoToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KOINOTOOLS_DIALOG, pParent)
{
	//20260908 by claude. LoadIcon 은 크기를 못 고른다 — 항상 SM_CXICON(큰 아이콘) 한 장만 로드한다.
	//그것을 작은 아이콘 자리에도 쓰면 타이틀바·작업표시줄의 16px 자리가 .ico 안의 전용 16x16 대신
	//32x32 를 축소한 그림이 되어 흐려진다. 크기별로 따로 로드해 각 자리에 맞는 이미지를 쓴다.
	//LR_SHARED — 시스템 메트릭 크기로 로드하는 표준 경우라 시스템이 캐시·해제한다(DestroyIcon 금지).
	m_hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
		::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR | LR_SHARED);
	m_hIcon_small = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
		::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR | LR_SHARED);
}

void CKoinoToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CODE_SIGN, m_static_code_sign);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_RICH, m_rich);
	DDX_Control(pDX, IDC_BUTTON_SPLITTER, m_vert_splitter);
}

BEGIN_MESSAGE_MAP(CKoinoToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CKoinoToolsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKoinoToolsDlg::OnBnClickedCancel)
	ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CKoinoToolsDlg::OnTvnSelchangedTree)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, &CKoinoToolsDlg::OnLvnEndLabelEditList)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE, &CKoinoToolsDlg::OnTvnEndLabelEditTree)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST, &CKoinoToolsDlg::OnLvnBeginLabelEditList)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, &CKoinoToolsDlg::OnTvnSelChangingTree)
	ON_COMMAND(ID_MENU_LMM_SDS_ENCRYPT, &CKoinoToolsDlg::OnMenuLmmSDSEncrypt)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_TREE_DELETE, &CKoinoToolsDlg::OnMenuTreeDelete)
	ON_COMMAND(ID_MENU_TREE_NEW_ITEM, &CKoinoToolsDlg::OnMenuTreeNewItem)
	ON_COMMAND(ID_MENU_TREE_RENAME, &CKoinoToolsDlg::OnMenuTreeRename)
	ON_COMMAND(ID_MENU_TREE_VIEW_REGEDIT, &CKoinoToolsDlg::OnMenuTreeViewRegEdit)
	ON_COMMAND(ID_MENU_DRAG_FULL_WINDOWS, &CKoinoToolsDlg::OnMenuDragFullWindows)
	ON_COMMAND(ID_MENU_TREE_SERVICE_STOP, &CKoinoToolsDlg::OnMenuTreeServiceStop)
	ON_COMMAND(ID_MENU_TREE_SERVICE_RESTART, &CKoinoToolsDlg::OnMenuTreeServiceRestart)
	ON_COMMAND(ID_MENU_TREE_SERVICE_DELETE, &CKoinoToolsDlg::OnMenuTreeServiceDelete)
	ON_COMMAND(ID_MENU_TREE_LOG_FOLDER, &CKoinoToolsDlg::OnMenuTreeLogFolder)
	ON_COMMAND(ID_MENU_TREE_DELETE_REG_URLSCHEME_INFO, &CKoinoToolsDlg::OnMenuTreeDeleteRegUrlSchemeInfo)
	ON_MESSAGE(WM_APP_CODESIGN_PROGRESS, &CKoinoToolsDlg::OnCodesignProgress)
	ON_MESSAGE(WM_APP_CODESIGN_DONE, &CKoinoToolsDlg::OnCodesignDone)
	ON_MESSAGE(WM_APP_SHOW_MSGBOX, &CKoinoToolsDlg::OnShowMsgbox)
	ON_MESSAGE(WM_APP_UI_INVOKE, &CKoinoToolsDlg::OnUiInvoke)
END_MESSAGE_MAP()


// CKoinoToolsDlg 메시지 처리기

BOOL CKoinoToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon_small, FALSE);	// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);
	m_resize.Add(IDC_TREE, 0, 0, 0, 100);
	m_resize.Add(IDC_STATIC_CODE_SIGN, 0, 0, 100, 0);
	m_resize.Add(IDC_LIST, 0, 0, 100, 0);
	m_resize.Add(IDC_RICH, 0, 0, 100, 100);
	m_resize.Add(IDC_BUTTON_SPLITTER, 0, 0, 0, 100);
	//20260909 by claude. ptMinTrackSize 는 window rect(외곽) 기준이라, Win11 의 보이지 않는 리사이즈 테두리 때문에
	//820x530 을 주면 보이는 창은 그만큼 작아진다. get_window_size_for_visible 로 그 창의 실측 보정을 더해
	//"보이는 최소 크기"가 정확히 820x530 이 되게 한다.
	m_resize.SetMinimumTrackingSize(get_window_size_for_visible(m_hWnd, 820, 530));

	//단락 모드는 set_tagged_text 가 호출되는 시점의 색상·폰트·line_spacing 로 단락을 build 하므로
	//모든 setter 가 반드시 set_tagged_text 보다 먼저 와야 한다.
	m_static_code_sign.set_back_color(Gdiplus::Color::Ivory);
	m_static_code_sign.set_round(8, Gdiplus::Color::RoyalBlue, get_sys_color(COLOR_3DFACE));
	m_static_code_sign.set_font_size(10);
	m_static_code_sign.set_tagged_text(_T("<sz=14>Drop exe files for <b><cr=royalblue>CodeSign</b>"));
	m_static_code_sign.set_tooltip_text(_T("CodeSign할 실행 파일들을 Drag&Drop 합니다.\nLMMAgent.exe 등 <cr=crimson><b>with Manifest</b></cr> 필수 파일은 자동으로 manifest를 포함해 서명하고, 그 외 파일은 <cr=royalblue><b>No Manifest</b></cr> 방식으로 서명합니다."));

	init_tree();
	init_list();
	init_rich();

	m_product = theApp.GetProfileString(_T("setting"), _T("recent product"), _T("LinkMeMine"));
	m_tree.select_item(m_product);
	//m_tree.iterate_tree_in_order();

	int min_size = 160;
	m_vert_splitter.set_type(CControlSplitter::CS_VERT, true, Gdiplus::Color::LightGray);
	m_vert_splitter.AddToTopOrLeftCtrls(IDC_TREE, min_size);
	m_vert_splitter.AddToBottomOrRightCtrls(IDC_STATIC_CODE_SIGN, 0, 0, SPF_LEFT);
	m_vert_splitter.AddToBottomOrRightCtrls(IDC_LIST, 320);
	m_vert_splitter.AddToBottomOrRightCtrls(IDC_RICH);
	//m_vert_splitter.AddToBottomOrRightCtrls(IDC_CHECK);

	Wait(10);
	RestoreWindowPosition(&theApp, this);

	//이 프로그램은 관리자 권한으로 실행되고 탐색기는 사용자 권한으로 실행되므로
	//UAC가 켜져있을 경우에는 탐색기에서 이 프로그램으로 Drag&Drop을 할 수 없도록 윈도우 정책이 정해져 있다.
	//따라서 아래와 같이 두 줄을 호출해줘야만 가능해진다.
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	ChangeWindowMessageFilterEx(m_hWnd, WM_DROPFILES, MSGFLT_ADD, nullptr);

	DragAcceptFiles();

	CString caption;
	caption.Format(_T("KoinoTools (ver %s)"), get_file_property());
	SetWindowText(caption);

	//모든 메시지박스에서 공유할 인스턴스 생성(타이틀/아이콘 지정 → 타이틀바 정상 표시).
	m_msgbox.create(this, _T("KoinoTools"), IDR_MAINFRAME);

	//작업표시줄 progress 표시용 ITaskbarList3(Win7+). COM STA 초기화 후 생성한다.
	//CoInitialize 가 S_FALSE(이미 초기화)여도 SUCCEEDED 이며, OnDestroy 에서 CoUninitialize 로 짝을 맞춘다.
	m_com_initialized = SUCCEEDED(CoInitialize(NULL));
	if (SUCCEEDED(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_taskbar))))
		m_taskbar->HrInit();

	//CCmdLine test code
	/*
	CString param = _T("-i 70.117.80.127 -p 7002 -sn 171675 -fr 1572807 -id 1 -t 0 -rh 136902 -rd 10001 -gi None -gp 0 -pn \"구미 -&nbsp PC\" -pi 70.117.80.120 -un inoh.seo -ui 70.117.129.85 -tn 0 -p2p 1 -p2pi 70.117.80.120 -p2pp 7002 -sizex 0 -sizey 0 -wm 1 -wms \"water -mark -str\" -dm 0 -ra 1");
	CCmdLine cmdLine(param);

	for (auto arg : cmdLine)
	{
		TRACE(_T("arg.first = %s\n"), arg.first);

		for (int i = 0; i < arg.second.m_strings.size(); i++)
			TRACE(_T("m_strings[%d] = %s\n"), i, arg.second.m_strings[i]);
	}
	*/

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CKoinoToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKoinoToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKoinoToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKoinoToolsDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}

void CKoinoToolsDlg::OnBnClickedOk()
{
}

void CKoinoToolsDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_list.save_column_width(&theApp, _T("list"));

	//프로그램을 종료할 때 실행파일 위치에 레지스트리 파일로도 항상 백업한다.
	//reg.exe를 run_process()로 호출하면 간혹 종료 시 무한 waiting이 되는 현상이 발생한다.
	//ShellExecute()로 실행하니 그런 현상은 발생하지 않는다.
	CString param;

	DeleteFile(get_exe_directory() + _T("\\KoinoTools.reg"));
	param.Format(_T("export \"HKCU\\Software\\Koino\\KoinoTools\" \"%s\\KoinoTools.reg\""), get_exe_directory());
	run_command(param);
	//ShellExecute(m_hWnd, _T("open"), _T("reg.exe"), param, NULL, SW_HIDE);

	/* RegSaveKeyEx()는 binary 형태로 저장된다. 아마도 RegLoadKeyEx()와 같은 함수와 같이 사용해야하는 듯 하다.
	HKEY hKey;
	LONG res;

	res = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Koino\\KoinoTools"), 0, KEY_ALL_ACCESS, &hKey);
	if (res == ERROR_SUCCESS)
	{
		//RegSaveKeyEx()를 하기 위해서는 반드시 previlege를 조정해줘야 한다.
		bool set_result = set_privilege(SE_BACKUP_NAME, TRUE);
		set_result = set_privilege(SE_RESTORE_NAME, TRUE);

		//결과파일이 존재하면 183 에러가 발생한다. 반드시 삭제시키고 저장해야 한다.
		DeleteFile(get_exe_directory() + _T("\\KoinoTools.reg"));

		res = RegSaveKey(hKey, get_exe_directory() + _T("\\KoinoTools.reg"), NULL);// , REG_NO_COMPRESSION);
		if (res == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			m_rich.add(Gdiplus::Color(Gdiplus::Color::Blue),_T("success exporting registry info."));
		}
		else
		{
			m_rich.add(Gdiplus::Color(Gdiplus::Color::Red),_T("fail to RegSaveKeyEx() for save registry."));
			m_rich.add(Gdiplus::Color(Gdiplus::Color::Red),_T("%s"), get_error_str(res));
		}
	}
	else
	{
		m_rich.add(Gdiplus::Color(Gdiplus::Color::Red),_T("fail to RegOpenKeyEx() for export registry."));
	}

	Wait(1000);
	*/

	CDialogEx::OnCancel();
}

//20260907 by claude. 최소 창 크기 제한. 이 프로젝트는 DPI-unaware 라 OS 가 논리 픽셀로 가상화해 주므로
//배율과 무관하게 이 값을 그대로 쓴다.
void CKoinoToolsDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//lpMMI->ptMinTrackSize.x = 820;
	//lpMMI->ptMinTrackSize.y = 530;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CKoinoToolsDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

//작업표시줄 progress 값/상태를 설정한다. error 면 빨강(TBPF_ERROR), 아니면 녹색(TBPF_NORMAL).
//(사용자가 창을 activate 하면 reset_taskbar_progress 로 제거)
void CKoinoToolsDlg::set_taskbar_progress(int percent, bool error /*= false*/)
{
	if (m_taskbar == nullptr)
		return;

	m_taskbar->SetProgressState(m_hWnd, error ? TBPF_ERROR : TBPF_NORMAL);
	m_taskbar->SetProgressValue(m_hWnd, (ULONGLONG)percent, 100);
	m_taskbar_progress_shown = true;
}

//사용자가 창을 activate 하면 progress 를 제거해 원래 작업표시줄 버튼으로 되돌린다.
void CKoinoToolsDlg::reset_taskbar_progress()
{
	if (m_taskbar == nullptr || !m_taskbar_progress_shown)
		return;

	//NOPROGRESS 만으로 안 지워지는 경우 대비해 값도 0 으로 내린 뒤 상태를 제거한다.
	m_taskbar->SetProgressValue(m_hWnd, 0, 100);
	m_taskbar->SetProgressState(m_hWnd, TBPF_NOPROGRESS);
	m_taskbar_progress_shown = false;
	m_codesign_finished = false;
	logWriteD(_T("reset_taskbar_progress: cleared"));
}

//AfxMessageBox 대체. 어느 스레드에서 호출해도 SendMessage 로 UI 스레드에 위임하여 공유 멤버 m_msgbox 로 띄운다.
//SendMessage 는 UI 스레드가 처리를 끝낼 때까지 동기 대기하므로 눌린 버튼 ID 를 그대로 반환할 수 있다.
//(UI 스레드에서 호출하면 SendMessage 가 곧바로 창 프로시저를 호출하므로 추가 비용/문제 없음)
int CKoinoToolsDlg::show_message(const CString& text, int type)
{
	return (int)SendMessage(WM_APP_SHOW_MSGBOX, (WPARAM)&text, (LPARAM)type);
}

//WM_APP_SHOW_MSGBOX 핸들러. 항상 UI 스레드에서 실행되므로 m_msgbox 를 안전하게 사용한다.
LRESULT CKoinoToolsDlg::OnShowMsgbox(WPARAM wParam, LPARAM lParam)
{
	const CString& text = *reinterpret_cast<const CString*>(wParam);
	return (LRESULT)m_msgbox.DoModal(text, (int)lParam);
}

//임의의 UI 작업을 UI 스레드로 넘긴다. func 를 힙에 올려 lParam 으로 실어 보내고, 처리 실패 시엔 누수 방지로 해제한다.
void CKoinoToolsDlg::invoke_ui(std::function<void()> func)
{
	const HWND hWnd = GetSafeHwnd();
	if (!::IsWindow(hWnd))
		return;

	auto* pfunc = new std::function<void()>(std::move(func));
	if (!::PostMessage(hWnd, WM_APP_UI_INVOKE, 0, reinterpret_cast<LPARAM>(pfunc)))
		delete pfunc;
}

//WM_APP_UI_INVOKE 핸들러. 항상 UI 스레드에서 실행되므로 넘겨받은 람다가 m_rich 등 UI 를 안전하게 만진다.
LRESULT CKoinoToolsDlg::OnUiInvoke(WPARAM /*wParam*/, LPARAM lParam)
{
	std::unique_ptr<std::function<void()>> pfunc(reinterpret_cast<std::function<void()>*>(lParam));
	(*pfunc)();
	return 0;
}

//워커 스레드 로그용. 문자열은 워커에서 만들고(add 는 포맷 함수라 %s 로 넘겨 재해석을 피한다) 실제 그리기는 UI 스레드에서.
void CKoinoToolsDlg::rich_add(Gdiplus::Color cr, LPCTSTR lpszFormat, ...)
{
	CString s;
	va_list args;
	va_start(args, lpszFormat);
	s.FormatV(lpszFormat, args);
	va_end(args);

	invoke_ui([this, cr, s]() { m_rich.add(cr, _T("%s"), (LPCTSTR)s); });
}

void CKoinoToolsDlg::rich_addl_tagged(Gdiplus::Color cr, LPCTSTR lpszFormat, ...)
{
	CString s;
	va_list args;
	va_start(args, lpszFormat);
	s.FormatV(lpszFormat, args);
	va_end(args);

	invoke_ui([this, cr, s]() { m_rich.addl_tagged(cr, _T("%s"), (LPCTSTR)s); });
}

//워커 스레드가 각 단계(delcert / codesign #1 / #2)마다 PostMessage 로 진행률을 보내고, 여기서 progress 를 갱신한다.
LRESULT CKoinoToolsDlg::OnCodesignProgress(WPARAM wParam, LPARAM /*lParam*/)
{
	set_taskbar_progress((int)wParam);
	logWriteD(_T("OnCodesignProgress: %d%%"), (int)wParam);
	return 0;
}

//codesign 전체 완료 시 실행된다. progress 를 100%(실패면 빨강)로 마무리하고, 완료를 눈에 띄게 알리려 작업표시줄 버튼을 2번 깜빡인다.
//깜빡임(순간 강조)과 progress(지속 표시)는 독립 API라 동시에 적용된다. 둘 다 사용자가 창을 activate 하면 지워진다.
LRESULT CKoinoToolsDlg::OnCodesignDone(WPARAM success, LPARAM /*lParam*/)
{
	set_taskbar_progress(100, success == 0);
	m_codesign_finished = true;		//이제부터 사용자 activate/클릭 시 progress 를 0%로 리셋한다.
	FlashWindowEx(FLASHW_ALL, 2, 0);
	logWriteD(_T("OnCodesignDone: success=%d, progress 100%%, flash x2"), (int)success);
	return 0;
}

void CKoinoToolsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	//코드사인이 완전히 끝난 뒤(m_codesign_finished)에만 리셋한다. 코드사인 중 토큰창이 닫히며 생기는
	//우발적 재활성화로는 진행률이 지워지지 않도록 한다.
	if (nState != WA_INACTIVE && m_codesign_finished)
		reset_taskbar_progress();
}

void CKoinoToolsDlg::OnDestroy()
{
	if (m_taskbar != nullptr)
	{
		m_taskbar->Release();
		m_taskbar = nullptr;
	}
	if (m_com_initialized)
	{
		CoUninitialize();
		m_com_initialized = false;
	}

	CDialogEx::OnDestroy();
}

void CKoinoToolsDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString delcert_path;
	delcert_path.Format(_T("%s\\delcert.exe"), m_list.get_text(0, col_value));
	if (!PathFileExists(delcert_path))
	{
		show_message(delcert_path + _T("\n\n위 파일이 존재하지 않습니다.\ndelcert.exe가 없을 경우 이미 CodeSign된 파일은 실패할 수 있습니다."));
		return;
	}

	TCHAR sfile[MAX_PATH];
	int count = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

	//20260908 by claude. 떨어뜨린 위치를 따지지 않는다 — 어디에 떨구든 codesign 이다.
	//전에는 리스트 위에 떨구면 그 폴더를 signtool 경로로 넣었는데, 실행파일을 리스트에 잘못 떨어뜨리면
	//경로 설정이 엉뚱한 값으로 덮여 다음 서명이 조용히 실패했다. 경로는 리스트에서 직접 입력한다.
	m_files.clear();

	for (int i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, sfile, MAX_PATH);

		if (PathIsDirectory(sfile))
			continue;

		m_files.push_back(sfile);
	}

	if (m_files.size())
	{
		//처음엔 thread_auto_password_input()만 thread로 돌리고
		//codesign_manifest()는 그냥 함수 호출로 실행했으나
		//run_process(cmd, true);로 signtool.exe가 실행되면
		//thread_auto_password_input() 또한 hold 상태가 되어버리므로
		//둘 다 thread로 돌리도록 수정함.
		m_codesign_finished = false;	//새 codesign 시작 → 완료 플래그 초기화

		std::thread th0(&CKoinoToolsDlg::thread_auto_password_input, this);
		th0.detach();

		//파일별로 manifest 필수 여부를 판별해 서명한다(LMMAgent.exe 등은 with Manifest, 나머지는 No Manifest).
		std::thread th1(&CKoinoToolsDlg::thread_codesign, this);
		th1.detach();
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//codesign 단계 실패를 사용자에게 확실히 알린다: 로그창(빨강) + 로그파일(error) + 메시지박스.
//exit_code 는 mt/signtool 이 반환한 0이 아닌 종료코드, output 은 그 표준출력+표준에러(실제 에러 문구 포함).
void CKoinoToolsDlg::report_codesign_step_error(LPCTSTR step_name, DWORD exit_code, const CString& output)
{
	CString msg;
	msg.Format(_T("%s 실패 [exit=%u]\n%s"), step_name, exit_code, output);

	rich_add(Gdiplus::Color(Gdiplus::Color::Red), _T("%s\n"), msg);
	logWriteE(_T("%s 실패: exit=%u\n%s"), step_name, exit_code, output);
	show_message(msg, MB_ICONERROR);
}

//반드시 with Manifest 로 서명해야 하는 실행파일 목록. 여기에 파일명(대소문자 무관)만 추가하면 자동으로 with Manifest 로 서명된다.
//목록에 없는 파일은 모두 No Manifest 로 서명된다.
bool CKoinoToolsDlg::is_manifest_required(const CString& filename) const
{
	static const LPCTSTR manifest_required[] =
	{
		_T("LMMAgent.exe"),
	};

	for (auto name : manifest_required)
	{
		if (filename.CompareNoCase(name) == 0)
			return true;
	}
	return false;
}

//통합 드롭으로 받은 m_files 를 서명한다. manifest 여부는 파일마다 is_manifest_required() 로 판별한다
//(LMMAgent.exe 등 with-Manifest 필수 파일은 with Manifest, 그 외는 No Manifest).
void CKoinoToolsDlg::thread_codesign()
{
	if (!check_valid_condition())
		return;

	sctrace(m_mt_path);
	sctrace(m_signtool_path);
	sctrace(m_manifest_folder);

	m_in_codesigning = true;
	bool error_occured = false;

	//작업표시줄 progress 용. 파일당 3단계(delcert / codesign #1 / #2). 단일 파일이면 33/66/100%.
	//ITaskbarList3 는 UI 스레드에서만 호출 가능하므로 진행률을 PostMessage 로 UI 스레드에 넘긴다.
	int completed_steps = 0;
	const int total_steps = (int)m_files.size() * 3;
	auto post_progress = [this, &completed_steps, total_steps]()
	{
		int pct = (total_steps > 0) ? (completed_steps * 100 / total_steps) : 0;
		PostMessage(WM_APP_CODESIGN_PROGRESS, (WPARAM)pct, 0);
	};

	for (int i = 0; i < m_files.size(); i++)
	{
		CString cmd;
		CString filename = get_part(m_files[i], fn_name);
		CString manifest_file = m_manifest_folder + _T("\\") + filename + _T(".manifest");
		CString result;

		//파일별 자동 라우팅: LMMAgent.exe 등 필수 목록은 with Manifest, 나머지는 No Manifest.
		bool apply_manifest = is_manifest_required(filename);

		//모드 문구를 태그로 강조한다(with=crimson, No=blue, 둘 다 bold). addl_tagged 가 CSCParagraph 파서로 해석한다.
		rich_addl_tagged(Gdiplus::Color::RoyalBlue, _T("\ncodesign start : %s (%s) (%d/%d)..."),
			filename,
			apply_manifest ? _T("<b><cr=crimson>with Manifest</cr></b>") : _T("<b><cr=blue>No Manifest</cr></b>"),
			i + 1, m_files.size());

		//파일이 열려있으면 코드사인이 실패하므로 에러로 처리한다.
		//_taccess()를 써봤으나 0이 리턴되고(사용중이 아니라고 판별)
		//_tfopen_s()을 써도 "r"이 가능하다고 판별되어 실행 프로세스인지를 검사하도록 수정함.
		HWND hWnd = get_hwnd_by_exe_file(m_files[i]);
		if (hWnd)
		{
			error_occured = true;
			rich_add(Gdiplus::Color(Gdiplus::Color::Red),_T("파일이 사용중이므로 코드사인 할 수 없습니다.\n"));
			break;
		}

		//우선 해당 파일이 이미 codesign되어 있다면 오류가 발생하는 경우가 있으므로 delcert.exe로 지워준다.
		rich_add(Gdiplus::Color::Transparent,_T("delcert : %s"), filename);
		cmd.Format(_T("\"%s\\delcert.exe\" \"%s\""), m_signtool_path, m_files[i]);
		DWORD rc_del = 0;
		result = run_command(cmd, INFINITE, &rc_del);
		//delcert 는 best-effort(미서명 파일이면 지울 게 없어 0이 아닐 수 있음)라 실패로 취급하지 않고 기록만 한다.
		logWriteD(_T("delcert output (exit=%u):\n%s"), rc_del, result);

		//delcert 프로세스는 run_command 에서 종료를 대기했지만, 종료 직후에도 파일이 잠시 잠겨 있으면
		//곧바로 이어지는 mt/signtool 이 파일을 열지 못하고 조용히 실패한다(팝업도 안 뜨고 완료로 표시되던 원인).
		//고정 sleep 대신, 파일 핸들이 완전히 풀려 단독으로 열 수 있을 때까지 기다린 뒤 다음 단계로 진행한다.
		if (!wait_until_file_writable(m_files[i], 10000))
		{
			error_occured = true;
			rich_add(Gdiplus::Color(Gdiplus::Color::Red),_T(" 실패(파일 잠금이 해제되지 않음)\n"));
			logWriteE(_T("delcert 후 10초 내 파일 잠금 해제 실패: %s"), m_files[i]);
			break;
		}
		rich_add(Gdiplus::Color(Gdiplus::Color::Blue),_T(" ok\n"));


		if (apply_manifest)
		{
			if (!PathFileExists(manifest_file))
			{
				//return 으로 빠지면 완료 통지(WM_APP_CODESIGN_DONE)·progress 리셋이 누락되므로 error 로 처리하고 중단한다.
				error_occured = true;
				rich_add(Gdiplus::Color(Gdiplus::Color::Red), _T("manifest 파일이 존재하지 않습니다.\n%s\n"), manifest_file);
				logWriteE(_T("manifest 파일 없음: %s"), manifest_file);
				show_message(_T("manifest 파일이 존재하지 않습니다.\n\n") + manifest_file, MB_ICONERROR);
				break;
			}

			cmd.Format(_T("\"%s\" -manifest \"%s\" -outputresource:\"%s\""), m_mt_path, manifest_file, m_files[i]);
			rich_add(Gdiplus::Color::Transparent,_T("manifest cmd : %s\n"), cmd);
			DWORD rc_mt = 0;
			result = run_command(cmd, INFINITE, &rc_mt);
			logWriteD(_T("mt output (exit=%u):\n%s"), rc_mt, result);
			if (rc_mt != 0)
			{
				report_codesign_step_error(_T("manifest 삽입(mt.exe)"), rc_mt, result);
				error_occured = true;
				break;
			}

			//mt.exe 도 exe 의 리소스를 수정하므로 동일하게 파일 잠금이 풀린 뒤 서명하도록 한다.
			if (!wait_until_file_writable(m_files[i], 10000))
			{
				error_occured = true;
				rich_add(Gdiplus::Color(Gdiplus::Color::Red),_T("manifest 삽입 후 파일 잠금이 해제되지 않았습니다.\n"));
				logWriteE(_T("mt 후 10초 내 파일 잠금 해제 실패: %s"), m_files[i]);
				break;
			}
		}

		completed_steps++;	//1단계: delcert(+manifest) 준비 완료
		post_progress();

		m_thread_auto_password_input_paused = false;
		//Wait(10000);
		cmd.Format(_T("\"%s\" sign /sha1 %s /s my /t http://timestamp.digicert.com /fd sha1 /v \"%s\""),
			m_signtool_path, m_fingerprint, m_files[i]);
		rich_add(Gdiplus::Color(Gdiplus::Color::DarkGray), _T("#1 phase codesign : %s\n"), cmd);
		DWORD rc_sign1 = 0;
		result = run_command(cmd, INFINITE, &rc_sign1);
		logWriteD(_T("signtool #1 output (exit=%u):\n%s"), rc_sign1, result);
		if (rc_sign1 != 0)
		{
			report_codesign_step_error(_T("#1 phase codesign(signtool)"), rc_sign1, result);
			error_occured = true;
			break;
		}

		completed_steps++;	//2단계: codesign #1 완료
		post_progress();

		while (FindWindowByCaption(_T("토큰 로그온"), true) != NULL)
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m_thread_auto_password_input_paused = false;
		//기존엔 여기서 무조건 1초를 기다렸다(#1 직후 파일이 덜 준비된 채 #2 가 실패하는 것을 피하려던 고정 딜레이).
		//#1 signtool 은 run_command 에서 종료를 대기했으므로, 파일이 단독으로 열리는 즉시 진행하면 된다 → 체감 딜레이 최소화.
		//혹시 10초 내 안 풀려도 그대로 #2 를 시도하며, 실제 실패는 아래 exit code 검사에서 잡아 보고한다.
		wait_until_file_writable(m_files[i], 10000);
		cmd.Format(_T("\"%s\" sign /sha1 %s /s my /tr http://timestamp.digicert.com /as /fd SHA256 /td sha256 /v \"%s\""),
			m_signtool_path, m_fingerprint, m_files[i]);
		rich_add(Gdiplus::Color(Gdiplus::Color::DarkGray), _T("#2 phase codesign : %s\n"), cmd);
		DWORD rc_sign2 = 0;
		result = run_command(cmd, INFINITE, &rc_sign2);
		logWriteD(_T("signtool #2 output (exit=%u):\n%s"), rc_sign2, result);
		if (rc_sign2 != 0)
		{
			report_codesign_step_error(_T("#2 phase codesign(signtool)"), rc_sign2, result);
			error_occured = true;
			break;
		}

		completed_steps++;	//3단계: codesign #2 완료
		post_progress();

		//std::this_thread::sleep_for(std::chrono::seconds(1));
		while (FindWindowByCaption(_T("토큰 로그온"), true) != NULL)
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

		rich_add(Gdiplus::Color(Gdiplus::Color::RoyalBlue),_T("%s codesign completed.\n"), filename);
	}

	m_thread_auto_password_input = false;
	while (!m_thread_auto_password_input_terminated)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	if (error_occured)
	{
		rich_add(Gdiplus::Color(Gdiplus::Color::Red),_T("All codesign job cancelled.\n"));
	}
	else
	{
		TRACE(_T("codeSign job finished.\n"));
		rich_add(Gdiplus::Color(Gdiplus::Color::Blue),_T("All files codesign completed.\n---------------------------------------\n"));
	}

	//20260722 by claude. codesign 중에는 thread_auto_password_input()이 "토큰 로그온" 창을 foreground로 끌어올리므로
	//작업이 끝나는 시점에 이 창은 거의 항상 뒤에 있다. 그래서 완료를 작업표시줄 progress 100% + 2번 깜빡임으로 알린다.
	//ITaskbarList3 는 COM STA(=UI 스레드)에서 호출해야 하는데 이 워커 스레드에서는 불가하므로,
	//PostMessage 로 UI 스레드(OnCodesignDone)에 위임한다. wParam = 성공(1)/실패(0).
	logWriteD(_T("codesign worker finished. post WM_APP_CODESIGN_DONE (thread=%u)"), ::GetCurrentThreadId());
	PostMessage(WM_APP_CODESIGN_DONE, error_occured ? 0 : 1, 0);

	m_in_codesigning = false;
}

bool CKoinoToolsDlg::check_valid_condition()
{
	bool is_valid = true;

	if (m_product.IsEmpty())
	{
		//AfxMessageBox(_T("제품을 선택해주세요."));
		return false;
	}

	Gdiplus::Color invalid_color = Gdiplus::Color(255, 64, 64);

	if (m_mt_path.IsEmpty() || !PathFileExists(m_mt_path) ||
		m_signtool_path.IsEmpty() || !PathFileExists(m_signtool_path))
	{
		m_list.set_text_color(0, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_text_color(0, -1);
	}

	if (m_manifest_folder.IsEmpty() || !PathIsDirectory(m_manifest_folder))
	{
		m_list.set_text_color(1, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_text_color(1, -1);
	}

	if (m_fingerprint.IsEmpty())
	{
		m_list.set_text_color(2, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_text_color(2, -1);
	}

	if (m_password.IsEmpty())
	{
		//m_list.set_text_color(3, -1, Gdiplus::Color::Crimson);
		m_list.set_text_color(3, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_text_color(3, -1);
	}

	return is_valid;
}

void CKoinoToolsDlg::thread_auto_password_input()
{
	m_thread_auto_password_input = true;
	m_thread_auto_password_input_paused = true;

	while (m_thread_auto_password_input)
	{
		if (m_thread_auto_password_input_paused)
		{
			TRACE(_T("m_thread_auto_password_input_paused...\n"));
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}
		else
			TRACE(_T("not paused.\n"));

		CWnd* pWnd = NULL;
		//CString finding_title = _T("KoinoTools");
		CString finding_title = _T("토큰 로그온");
		CRect rw;

		//토큰 로그온 창이 나타날 때까지 대기
		while ((pWnd = FindWindowByCaption(finding_title, true)) == NULL)
		{
			if (!m_thread_auto_password_input)
				break;

			TRACE(_T("finding \"%s\"...\n"), finding_title);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		if (!m_thread_auto_password_input)
		{
			TRACE(_T("m_thread_auto_password_input = false\n"));
			break;
		}

		TRACE(_T("found \"%s\"...\n"), finding_title);

		SetForegroundWindowForce(pWnd->m_hWnd, true);

		pWnd->GetWindowRect(rw);

		//암호 입력창으로 커서 이동
		SetCursorPos(rw.left + 220, rw.top + 181);
		//SetCursorPos(rw.left + 698, rw.top + 54);

		//이미 입력된 암호가 있다면 지우기 위해 더블클릭 후 제거
		sc_mouse_event(mouse_event_ldbclick);
		keybd_event(VK_BACK, 0, 0, 0);

		//지정된 암호를 입력한다.
		m_key_input.input(m_password);
		//자동 입력될 키가 아직 버퍼에 남아있다면 500ms 정도 기다린다.
		while (m_key_input.get_key_count() > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

		//m_key_input에 모든 키가 자동 타이핑 된 후 바로 엔터를 치면 마지막 키가 입력이 안된 상태일 수 있으므로
		//온전히 모두 입력될 때까지 약간 더 여유를 준다.
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		//OK 버튼 클릭
		//windows10, windows11 약간 UI가 달라서 ok버튼 위치가 다를 수 있다. Enter키로 처리한다.
		keybd_event(VK_RETURN, 0, 0, 0);
		//sc_mouse_event(mouse_event_lclick, rw.left + 380, rw.top + 258);
		m_thread_auto_password_input_paused = true;
	}

	m_thread_auto_password_input = false;
	m_thread_auto_password_input_paused = true;
	m_thread_auto_password_input_terminated = true;
	TRACE(_T("thread_auto_password_input() terminated.\n"));
}

void CKoinoToolsDlg::init_tree()
{
	//default=24를 20으로 조정.
	m_tree.SetItemHeight(20);

	//순서는 의미없이 그냥 resource editor에 등록된 순서대로 추가함.
	m_tree.set_imagelist(IDI_ANYSUPPORT, IDI_AUTH, IDI_BUBBLE, IDI_CHECK, IDI_DESKTOP, IDI_DIAGRAM, IDI_DOCS, IDI_HELPU, IDI_LINKMEMINE, IDI_LINKMEMINE_SE, IDI_PCANYPRO);

	//registry의 product 섹션 아래 모든 항목을 가져와서 트리에 추가한다.
	std::deque<CString> enum_subkeys;
	enum_registry_subkeys(HKEY_CURRENT_USER, m_reg_product_root, enum_subkeys);

	for (auto subkey : enum_subkeys)
	{
		subkey.Replace(m_reg_product_root + _T("\\"), _T(""));
		sctrace(subkey);

		std::deque<CString> token;
		get_token_str(subkey, token, _T("\\"), false);

		int icon_index;

		//최종 경로면 그대로 노드를 추가하고
		if (token.size() == 1)
		{
			icon_index = get_icon_index(token[0]);
			m_tree.InsertItem(token[0], icon_index, icon_index);
		}
		//다중 경로면 말단 경로까지 탐색한 후 추가한다.
		else
		{
			HTREEITEM hItem = NULL;

			for (int i = 0; i < token.size(); i++)
			{
				if (i < token.size() - 1)
				{
					hItem = m_tree.find_item(token[i], hItem);
				}
				else
				{
					icon_index = get_icon_index(token[i]);
					m_tree.InsertItem(token[i], icon_index, icon_index, hItem);
				}
			}
		}
	}

	m_tree.expand_all();
}

int CKoinoToolsDlg::get_icon_index(CString product_name)
{
	if (find(product_name, _T("anysupport")) >= 0)
		return 0;
	else if (find(product_name, _T("helpu")) >= 0)
		return 7;
	else if (find(product_name, _T("linkmemine_se")) >= 0)
		return 9;
	else if (find(product_name, _T("linkmemine")) >= 0)
		return 8;
	else if (find(product_name, _T("pcanypro")) >= 0)
		return 10;

	return 1;
}

void CKoinoToolsDlg::init_list()
{
	//LVS_EX_FLATSB 사용 금지 — CSCListCtrl 은 CSCScrollbar overlay 가 단독 결정자.
	//FlatSB 모듈이 native scrollbar 비트를 시각화하면 overlay 와 충돌(드래그 시 사라지는 가로바 현상 등).
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.set_headings(_T("항목,100;경로,300;설명,300"));
	m_list.set_header_height(24);
	m_list.set_line_height(22);
	m_list.set_font_size(9);

	m_list.allow_edit_column(col_item, false);
	m_list.allow_edit_column(col_value, true);
	m_list.allow_edit_column(col_desc, false);

	m_list.allow_sort(false);

	m_list.restore_column_width(&theApp, _T("list"));

	int index = m_list.add_item(_T("signtool path"));
	m_list.add_item(_T("manifest folder"));
	m_list.add_item(_T("fingerprint"));
	m_list.add_item(_T("password"));

	m_list.set_text(0, col_desc, _T("mt.exe, signtool.exe가 위치한 폴더 경로"));
	m_list.set_text(1, col_desc, _T("manifest 파일들이 위치한 폴더 경로"));
	m_list.set_text(2, col_desc, _T("인증서 지문 데이터"));
	m_list.set_text(3, col_desc, _T("CodeSign 암호"));

	m_list.set_default_text_color(Gdiplus::Color::DimGray);

	m_list.set_use_own_context_menu(false);

	//CSCListCtrl에서 header height, line height를 주면 간혹 0번 항목이 헤더에 가려진 채로 시작되는 경우가 있다.
	//뭔가 SetLayout()관련 처리가 부족한 듯 한데 우선 0번 항목을 선택시켜주면 이런 부작용이 나타나진 않는다.
	m_list.select_item(0);
}

void CKoinoToolsDlg::init_rich()
{
	m_rich.show_time_info(false);
	m_rich.set_line_spacing(1.0f);
	m_rich.set_default_text_color(Gdiplus::Color::DimGray);
}

void CKoinoToolsDlg::OnTvnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_product = m_tree.get_selected_item_text(true);
	if (m_product.IsEmpty())
		return;

	CString path = theApp.GetProfileString(_T("product\\") + m_product, _T("signtool path"), _T(""));
	m_mt_path = path + _T("\\mt.exe");
	m_signtool_path = path + _T("\\signtool.exe");
	m_manifest_folder = theApp.GetProfileString(_T("product\\") + m_product, _T("manifest folder"), _T(""));
	m_fingerprint = theApp.GetProfileString(_T("product\\") + m_product, _T("fingerprint"), _T(""));
	m_password = theApp.GetProfileString(_T("product\\") + m_product, _T("password"), _T(""));

	m_list.set_text(0, col_value, path);
	m_list.set_text(1, col_value, m_manifest_folder);
	m_list.set_text(2, col_value, m_fingerprint);
	m_list.set_text(3, col_value, m_password);

	check_valid_condition();

	theApp.WriteProfileString(_T("setting"), _T("recent product"), m_product);

	*pResult = 0;
}

void CKoinoToolsDlg::OnLvnEndLabelEditList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_product = m_tree.get_selected_item_text(true);
	sctrace(m_product);

	int item = m_list.get_recent_edit_item();
	int subitem = m_list.get_recent_edit_subitem();

	if (item == 0)
	{
		CString path = m_list.get_text(item, subitem);
		m_mt_path = path + _T("\\mt.exe");
		m_signtool_path = path + _T("\\signtool.exe");

		theApp.WriteProfileString(_T("product\\") + m_product, _T("signtool path"), path);
	}
	else if (item == 1)
	{
		m_manifest_folder = m_list.get_text(item, subitem);
		theApp.WriteProfileString(_T("product\\") + m_product, _T("manifest folder"), m_manifest_folder);
	}
	else if (item == 2)
	{
		m_fingerprint = m_list.get_text(item, subitem);
		theApp.WriteProfileString(_T("product\\") + m_product, _T("fingerprint"), m_fingerprint);
	}
	else if (item == 3)
	{
		m_password = m_list.get_text(item, subitem);
		theApp.WriteProfileString(_T("product\\") + m_product, _T("password"), m_password);
	}

	check_valid_condition();

	*pResult = 0;
}

BOOL CKoinoToolsDlg::PreTranslateMessage(MSG* pMsg)
{
	//codesign 완료 후 남은 progress 는 사용자가 창과 상호작용하면 제거한다.
	//완료 시 앱이 이미 활성 상태면 OnActivate 가 안 오므로(=클릭해도 리셋 안 되던 문제), 클릭/키 입력도 원복 트리거로 삼는다.
	if (m_codesign_finished &&
		(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN ||
		 pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_KEYDOWN))
	{
		reset_taskbar_progress();
	}

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_F2:
				if (GetFocus() == &m_tree)
				{
					m_tree.edit_item();
					return TRUE;
				}
				else if (GetFocus() == &m_list)
				{
					m_list.edit_item();
					return TRUE;
				}
				break;
			case VK_DELETE:
				if (GetFocus() == &m_tree)
				{
					OnMenuTreeDelete();
					return TRUE;
				}
				break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CKoinoToolsDlg::OnMenuTreeNewItem()
{
	m_tree.add_new_item(NULL, _T("새 제품"), true, true);

	m_product = m_tree.get_selected_item_text(true);
	sctrace(m_product);

	CString subkey;

	subkey.Format(_T("%s\\%s"), m_reg_product_root, m_product);

	HKEY hKey;
	LONG res;

	res = RegCreateKeyEx(HKEY_CURRENT_USER, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	RegCloseKey(hKey);
}


void CKoinoToolsDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//SCTreeCtrl::OnContextMenu 가 wParam = tree HWND 로 forward 한 경우 분기.
	//(NM_RCLICK 은 SCTreeCtrl 에서 ON_NOTIFY_REFLECT_EX 로 먼저 잡아 parent 로 안 옴.)
	if (pWnd && pWnd->GetSafeHwnd() == m_tree.GetSafeHwnd())
	{
		show_tree_context_menu(point);
		return;
	}

	CMenu menu;
	CMenu* pMenu = NULL;

	menu.LoadMenu(IDR_MENU_CONTEXT);
	pMenu = menu.GetSubMenu(0);

	BOOL enabled;
	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, NULL, &enabled, 0);

	menu.CheckMenuItem(ID_MENU_DRAG_FULL_WINDOWS, enabled ? MF_CHECKED : MF_UNCHECKED);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CKoinoToolsDlg::show_tree_context_menu(CPoint point_screen)
{
	//SCTreeCtrl::OnContextMenu 가 hItem == NULL 이면 forward 안 하므로 여기서는 항상 선택 항목 존재.
	//선택도 SCTreeCtrl 쪽에서 이미 수행됨.
	CPoint pt_client = point_screen;
	m_tree.ScreenToClient(&pt_client);
	HTREEITEM hItem = m_tree.HitTest(pt_client);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_TREE);

	CMenu* pMenu = menu.GetSubMenu(0);

	menu.EnableMenuItem(ID_MENU_TREE_RENAME, hItem ? MF_ENABLED : MF_DISABLED);
	menu.EnableMenuItem(ID_MENU_TREE_DELETE, hItem ? MF_ENABLED : MF_DISABLED);

	CString label = m_tree.get_selected_item_text();
	CString service_name;
	if (label.CompareNoCase(_T("LinkMeMine")) == 0)
		service_name = _T("LMMSvcAgentService");
	else if (label.CompareNoCase(_T("LinkMeMine_SE")) == 0)
		service_name = _T("LMMSEAgentService");

	if (service_name.IsEmpty())
	{
		menu.EnableMenuItem(ID_MENU_TREE_SERVICE_RESTART, MF_DISABLED);
		menu.EnableMenuItem(ID_MENU_TREE_SERVICE_STOP, MF_DISABLED);
		menu.EnableMenuItem(ID_MENU_TREE_SERVICE_DELETE, MF_DISABLED);
	}
	else
	{
		menu.ModifyMenu(ID_MENU_TREE_SERVICE_RESTART, MF_BYCOMMAND, ID_MENU_TREE_SERVICE_RESTART, service_name + _T(" 재시작(&R)"));
		menu.ModifyMenu(ID_MENU_TREE_SERVICE_STOP, MF_BYCOMMAND, ID_MENU_TREE_SERVICE_STOP, service_name + _T(" 중지(&S)"));
		menu.ModifyMenu(ID_MENU_TREE_SERVICE_DELETE, MF_BYCOMMAND, ID_MENU_TREE_SERVICE_DELETE, service_name + _T(" 삭제(&D)"));

		DWORD service_status = 0;
		DWORD error_code = 0;
		CString detail;

		service_status = service_command(service_name, _T("query"), error_code, &detail);

		menu.EnableMenuItem(ID_MENU_TREE_SERVICE_STOP, service_status == SERVICE_RUNNING ? MF_ENABLED : MF_DISABLED);
		menu.EnableMenuItem(ID_MENU_TREE_SERVICE_DELETE, service_status > 0 ? MF_ENABLED : MF_DISABLED);
	}

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point_screen.x, point_screen.y, this);
}

void CKoinoToolsDlg::OnTvnEndLabelEditTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString old_label = m_tree.get_edit_old_text();
	CString new_label = m_tree.get_edit_new_text();
	sctrace(new_label);

	if (old_label == new_label)
		return;

	m_product = m_tree.get_selected_item_text(true);
	m_product = m_product.Left(m_product.ReverseFind('\\'));
	sctrace(m_product);

	CString subkey_old;
	CString subkey_new;

	subkey_old.Format(_T("%s\\%s\\%s"), m_reg_product_root, m_product, old_label);
	subkey_new.Format(_T("%s\\%s\\%s"), m_reg_product_root, m_product, new_label);

	HKEY hKey;
	LONG res;

	if (is_exist_registry_key(HKEY_CURRENT_USER, subkey_old))
	{
		res = RegOpenKeyEx(HKEY_CURRENT_USER, subkey_old, 0, KEY_ALL_ACCESS, &hKey);
		if (res == ERROR_SUCCESS)
		{
			//RegRenameKey()는 공식적으로 지원하지 않으며 에러가 발생한다.
			//res = RegRenameKey(hKey, old_label, new_label);

			HKEY hKey_dst;
			res = RegCreateKeyEx(HKEY_CURRENT_USER, subkey_new, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_dst, NULL);
			res = RegCopyTree(hKey, nullptr, hKey_dst);
			res = RegDeleteTree(HKEY_CURRENT_USER, subkey_old);
			RegCloseKey(hKey_dst);
			RegCloseKey(hKey);

			theApp.WriteProfileString(_T("setting"), _T("recent product"), m_product + _T("\\") + new_label);

			//이름이 변경되면 해당 제품에 맞는 아이콘으로 변경해준다.
			int icon_index = get_icon_index(new_label);
			m_tree.SetItemImage(m_tree.GetSelectedItem(), icon_index, icon_index);
		}
	}

	*pResult = 0;
}

void CKoinoToolsDlg::OnLvnBeginLabelEditList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CKoinoToolsDlg::OnTvnSelChangingTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//리스트 항목을 편집을 완료하지 않고 트리의 제품 항목을 선택하면
	//OnTvnSelChangedTree() 함수에서 새로 선택된 트리 항목이 선택되고
	//편집된 항목의 값이 새 제품의 필드에 들어가는 오류가 발생한다.
	//따라서 트리의 선택이 바뀌기 전에 리스트의 편집을 종료시켜 줘야 한다.
	m_list.edit_end();
	Wait(10);
	*pResult = 0;
}

void CKoinoToolsDlg::OnMenuLmmSDSEncrypt()
{
	CSDSEncryptDlg dlg;
	dlg.DoModal();
}

void CKoinoToolsDlg::OnMenuTreeDelete()
{
	CString label = m_tree.get_selected_item_text();
	int res = show_message(label + _T("\n\n위 항목 및 하위 항목들을 모두 삭제합니다.\n항목을 삭제하면 되돌릴 수 없습니다."), MB_OKCANCEL);

	if (res == IDCANCEL)
		return;

	CString subkey;

	label = m_tree.get_selected_item_text(true);
	subkey.Format(_T("%s\\%s"), m_reg_product_root, label);

	res = RegDeleteTree(HKEY_CURRENT_USER, subkey);

	m_tree.delete_item(m_tree.GetSelectedItem());
}

void CKoinoToolsDlg::OnMenuTreeRename()
{
	m_tree.rename_item();
}

void CKoinoToolsDlg::OnMenuTreeViewRegEdit()
{
	CString param;

	//registry editor에게 특정 경로를 마지막 경로라고 지정하게 한 뒤
	//cmd line : REG ADD HKCU\Software\Microsoft\Windows\CurrentVersion\Applets\Regedit /v LastKey /t REG_SZ /d "컴퓨터\HKEY_CURRENT_USER\SOFTWARE\Koino" /f
	param.Format(_T("add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit /v LastKey /t REG_SZ /d \"컴퓨터\\HKEY_CURRENT_USER\\SOFTWARE\\Koino\\KoinoTools\\product\" /f"));
	ShellExecute(m_hWnd, _T("open"), _T("reg.exe"), param, NULL, SW_HIDE);
	Wait(10);

	//registry editor를 열어준다.
	ShellExecute(m_hWnd, _T("open"), _T("regedit.exe"), NULL, NULL, SW_SHOWNORMAL);
}

void CKoinoToolsDlg::OnMenuDragFullWindows()
{
	BOOL enabled;

	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, NULL, &enabled, 0);

	enabled = !enabled;
	SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, enabled, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

void CKoinoToolsDlg::OnMenuTreeServiceStop()
{
	CString label = m_tree.get_selected_item_text();
	CString service_name;

	if (label.CompareNoCase(_T("LinkMeMine")) == 0)
		service_name = _T("LMMSvcAgentService");
	else if (label.CompareNoCase(_T("LinkMeMine_SE")) == 0)
		service_name = _T("LMMSEAgentService");
	else
	{
		return;
	}

	DWORD error_code = 0;
	CString detail;
	DWORD status_code = 0;

	m_rich.add(Gdiplus::Color::Transparent,_T("try to stop %s...\n"), service_name);
	status_code = service_command(service_name, _T("stop"), error_code, &detail);

	CString str;
	str.Format(_T("status_code = %d(%s), error_code = %d (%s)"), status_code, get_service_status_str(status_code), error_code, detail);
	m_rich.add(error_code == 0 ? Gdiplus::Color(Gdiplus::Color::Blue) : Gdiplus::Color(Gdiplus::Color::Red),_T("%s\n"), str);
}

void CKoinoToolsDlg::OnMenuTreeServiceRestart()
{
	CString label = m_tree.get_selected_item_text();
	CString service_name;

	if (label.CompareNoCase(_T("LinkMeMine")) == 0)
		service_name = _T("LMMSvcAgentService");
	else if (label.CompareNoCase(_T("LinkMeMine_SE")) == 0)
		service_name = _T("LMMSEAgentService");
	else
	{
		return;
	}

	DWORD error_code = 0;
	CString detail;
	DWORD status_code = 0;

	m_rich.add(Gdiplus::Color::Transparent,_T("try to restart %s...\n"), service_name);
	status_code = service_command(service_name, _T("restart"), error_code, &detail);

	CString str;
	str.Format(_T("status_code = %d(%s), error_code = %d (%s)"), status_code, get_service_status_str(status_code), error_code, detail);
	m_rich.add(error_code == 0 ? Gdiplus::Color(Gdiplus::Color::Blue) : Gdiplus::Color(Gdiplus::Color::Red),_T("%s\n"), str);
}

void CKoinoToolsDlg::OnMenuTreeServiceDelete()
{
	CString label = m_tree.get_selected_item_text();
	CString service_name;

	if (label.CompareNoCase(_T("LinkMeMine")) == 0)
		service_name = _T("LMMSvcAgentService");
	else if (label.CompareNoCase(_T("LinkMeMine_SE")) == 0)
		service_name = _T("LMMSEAgentService");
	else
	{
		return;
	}

	DWORD error_code = 0;
	CString detail;
	DWORD status_code = 0;

	m_rich.add(Gdiplus::Color::Transparent,_T("try to delete %s...\n"), service_name);
	status_code = service_command(service_name, _T("delete"), error_code, &detail);

	CString str;
	str.Format(_T("status_code = %d(%s), error_code = %d (%s)"), status_code, get_service_status_str(status_code), error_code, detail);
	m_rich.add(error_code == 0 ? Gdiplus::Color(Gdiplus::Color::Blue) : Gdiplus::Color(Gdiplus::Color::Red),_T("%s\n"), str);
}

void CKoinoToolsDlg::OnMenuTreeLogFolder()
{
	CString label = m_tree.get_selected_item_text();

	if (label.CompareNoCase(_T("LinkMeMine")) == 0)
	{
		ShellExecute(m_hWnd, _T("open"), _T("C:\\Users\\Public\\Documents\\LinkMeMine"), NULL, NULL, SW_SHOWNORMAL);
	}
	else if (label.CompareNoCase(_T("LinkMeMine_SE")) == 0)
	{
		ShellExecute(m_hWnd, _T("open"), _T("C:\\Users\\Public\\Documents\\LinkMeMineSE"), NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		show_message(_T("not defined"));
	}
}

void CKoinoToolsDlg::OnMenuTreeDeleteRegUrlSchemeInfo()
{
	//URLScheme 정보는 총 3군데에 등록된다.
	//https://docs.google.com/presentation/d/1Rc18rcW05aMKB4FbQLFV-N-FKEg1hGmK62_01AzoLTQ/edit?slide=id.g3705e7456f4_0_0#slide=id.g3705e7456f4_0_0
	//HKEY_CURRENT_USER에 등록하면 윈도우에서 HKEY_CURRENT_USER + HKEY_LOCAL_MACHINE 의 \Software\Classes 를 합쳐서
	//HKEY_CLASSES_ROOT\Software\Classes 로 사용하므로 프로그램에서는 HKEY_CURRENT_USER 까지만 만들어주면 된다.
	//단, 삭제할 때는 3군데 모두 삭제해줘야 한다.
	CString label = m_tree.get_selected_item_text();
	std::deque<CString> url_scheme_protocols;

	if (label.CompareNoCase(_T("LinkMeMine")) == 0)
	{
		url_scheme_protocols.push_back(_T("manuallauncher.lmm.service"));
	}
	else if (label.CompareNoCase(_T("LinkMeMine_SE")) == 0)
	{
		url_scheme_protocols.push_back(_T("manuallauncher.lmmse.service"));
	}
	else if (label.CompareNoCase(_T("AnySupport")) == 0)
	{
		url_scheme_protocols.push_back(_T("manuallauncher.anysupport.service.host"));
		url_scheme_protocols.push_back(_T("manuallauncher.anysupport.service.supporter"));
	}
	else if (label.CompareNoCase(_T("HelpU")) == 0)
	{
		url_scheme_protocols.push_back(_T("manuallauncher.helpu.service.host"));
		url_scheme_protocols.push_back(_T("manuallauncher.helpu.service.supporter"));
	}
	else
	{
		show_message(_T("not defined"));
	}

	LSTATUS status;
	//3군데 모두 돌면서 동일한 키를 삭제한다.
	for (int i = 0; i < url_scheme_protocols.size(); i++)
	{
		status = RegDeleteTree(HKEY_CLASSES_ROOT, url_scheme_protocols[i]);
		if (status == ERROR_SUCCESS)
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Blue),_T("HKEY_CLASSES_ROOT\\%s deleted"), url_scheme_protocols[i]);
		else if (status == ERROR_FILE_NOT_FOUND)
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Blue),_T("HKEY_CLASSES_ROOT\\%s does not exists. skip."), url_scheme_protocols[i]);
		else
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Red),_T("HKEY_CLASSES_ROOT\\%s delete failed. status = %d"), url_scheme_protocols[i], status);

		status = RegDeleteTree(HKEY_CURRENT_USER, _T("Software\\Classes\\") + url_scheme_protocols[i]);
		if (status == ERROR_SUCCESS)
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Blue),_T("HKEY_CURRENT_USER\\Software\\Classes\\%s deleted"), url_scheme_protocols[i]);
		else if (status == ERROR_FILE_NOT_FOUND)
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Blue),_T("HKEY_CURRENT_USER\\%s does not exists. skip."), url_scheme_protocols[i]);
		else
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Red),_T("HKEY_CURRENT_USER\\Software\\Classes\\%s delete failed"), url_scheme_protocols[i]);

		status = RegDeleteTree(HKEY_LOCAL_MACHINE, _T("Software\\Classes\\") + url_scheme_protocols[i]);
		if (status == ERROR_SUCCESS)
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Blue),_T("HKEY_LOCAL_MACHINE\\Software\\Classes\\%s deleted"), url_scheme_protocols[i]);
		else if (status == ERROR_FILE_NOT_FOUND)
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Blue),_T("HKEY_LOCAL_MACHINE\\%s does not exists. skip."), url_scheme_protocols[i]);
		else
			m_rich.addl(Gdiplus::Color(Gdiplus::Color::Red),_T("HKEY_LOCAL_MACHINE\\Software\\Classes\\%s delete failed"), url_scheme_protocols[i]);
	}
}
