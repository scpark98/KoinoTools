
// KoinoToolsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "KoinoTools.h"
#include "KoinoToolsDlg.h"
#include "afxdialogex.h"

#include <thread>
#include <winsvc.h>

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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKoinoToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CODE_SIGN_MANIFEST, m_static_code_sign_manifest);
	DDX_Control(pDX, IDC_STATIC_CODE_SIGN_NO_MANIFEST, m_static_code_sign_no_manifest);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_RICH, m_rich);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_BUTTON_SPLITTER, m_vert_splitter);
}

BEGIN_MESSAGE_MAP(CKoinoToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CKoinoToolsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKoinoToolsDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
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
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);
	m_resize.Add(IDC_TREE, 0, 0, 0, 100);
	m_resize.Add(IDC_LIST, 0, 0, 100, 0);
	m_resize.Add(IDC_RICH, 0, 0, 100, 100);
	m_resize.Add(IDC_BUTTON_SPLITTER, 0, 0, 0, 100);

	//단락 모드는 set_tagged_text 가 호출되는 시점의 색상·폰트·line_spacing 로 단락을 build 하므로
	//모든 setter 가 반드시 set_tagged_text 보다 먼저 와야 한다.
	m_static_code_sign_no_manifest.set_back_color(Gdiplus::Color::Ivory);
	m_static_code_sign_no_manifest.set_round(8, Gdiplus::Color::RoyalBlue, get_sys_color(COLOR_3DFACE));
	m_static_code_sign_no_manifest.set_font_size(10);
	m_static_code_sign_no_manifest.set_tagged_text(_T("Drop exe files here for<br><b>CodeSign with <cr=red>No Manifest"));
	m_static_code_sign_no_manifest.set_tooltip_text(_T("manifest를 적용하지 않고 CodeSign할 파일들을 여기에 drag&drop 합니다.\n(주의 : LMMAgent.exe는 반드시 manifest를 포함하여 CodeSign 해야 함!)"));

	m_static_code_sign_manifest.set_back_color(Gdiplus::Color::AntiqueWhite);
	m_static_code_sign_manifest.set_round(8, Gdiplus::Color::IndianRed, get_sys_color(COLOR_3DFACE));
	m_static_code_sign_manifest.set_font_size(10);
	m_static_code_sign_manifest.set_tagged_text(_T("Drop exe files here for<br><b>CodeSign with <cr=blue>Manifest"));
	m_static_code_sign_manifest.set_tooltip_text(_T("manifest를 적용하여 CodeSign할 파일들을 여기에 drag&drop 합니다.\n(주의 : LMMAgent.exe는 반드시 manifest를 포함하여 CodeSign 해야 함!)"));

	init_tree();
	init_list();
	init_rich();

	m_product = theApp.GetProfileString(_T("setting"), _T("recent product"), _T("LinkMeMine"));
	m_tree.select_item(m_product);
	//m_tree.iterate_tree_in_order();

	int min_size = 160;
	m_vert_splitter.set_type(CControlSplitter::CS_VERT, true, Gdiplus::Color::LightGray);
	m_vert_splitter.AddToTopOrLeftCtrls(IDC_TREE, min_size);
	m_vert_splitter.AddToBottomOrRightCtrls(IDC_STATIC_CODE_SIGN_NO_MANIFEST, 0, 0, SPF_LEFT);
	m_vert_splitter.AddToBottomOrRightCtrls(IDC_STATIC_CODE_SIGN_MANIFEST, 0, 0, SPF_LEFT);
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CKoinoToolsDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
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
			m_rich.add(blue, _T("success exporting registry info."));
		}
		else
		{
			m_rich.add(red, _T("fail to RegSaveKeyEx() for save registry."));
			m_rich.add(red, _T("%s"), get_error_str(res));
		}
	}
	else
	{
		m_rich.add(red, _T("fail to RegOpenKeyEx() for export registry."));
	}

	Wait(1000);
	*/

	CDialogEx::OnCancel();
}

void CKoinoToolsDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

//codesign 완료 알림으로 켜둔 작업표시줄 깜빡임을 사용자 액션 시 즉시 끈다.
//깜빡이던 상태(m_taskbar_flashing)일 때만 FLASHW_STOP 을 호출한다.
void CKoinoToolsDlg::stop_taskbar_flash()
{
	if (!m_taskbar_flashing)
		return;

	FlashWindowEx(FLASHW_STOP, 0, 0);
	m_taskbar_flashing = false;
}

void CKoinoToolsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (nState != WA_INACTIVE)
		stop_taskbar_flash();
}

void CKoinoToolsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		stop_taskbar_flash();
}

void CKoinoToolsDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString delcert_path;
	delcert_path.Format(_T("%s\\delcert.exe"), m_list.get_text(0, col_value));
	if (!PathFileExists(delcert_path))
	{
		AfxMessageBox(delcert_path + _T("\n\n위 파일이 존재하지 않습니다.\ndelcert.exe가 없을 경우 이미 CodeSign된 파일은 실패할 수 있습니다."));
		return;
	}

	m_action = action_no_action;

	TCHAR sfile[MAX_PATH];
	int count = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

	POINT pt;
	DragQueryPoint(hDropInfo, &pt);

	// Convert to client coordinates of main window
	//::ClientToScreen(m_hWnd, &pt);        // absolute screen
	//::ScreenToClient(m_hWnd, &pt);        // back to dialog client coords (safer)

	// Find child control at this point
	CWnd* pCtrl = ChildWindowFromPoint(pt);
	if (pCtrl == &m_static_code_sign_manifest)
	{
		m_action = action_codesign_manifest;
		TRACE(_T("m_static_code_sign_manifest\n"));
	}
	else if (pCtrl == &m_static_code_sign_no_manifest)
	{
		m_action = action_codesign_no_manifest;
		TRACE(_T("m_static_code_sign_no_manifest\n"));
	}
	else if (pCtrl == &m_list)
	{
		int item;
		int sub_item;
		CRect rlist;
		m_list.GetWindowRect(rlist);
		ScreenToClient(rlist);

		pt.x -= rlist.left;
		pt.y -= rlist.top;
		m_list.hit_test(pt, item, sub_item, true);
		if (item == 0)
		{
			DragQueryFile(hDropInfo, 0, sfile, MAX_PATH);

			CString path = sfile;
			theApp.WriteProfileString(_T("product\\") + m_product, _T("signtool path"), path);

			m_list.set_text(0, col_value, path);

			m_mt_path = path + _T("\\mt.exe");
			m_signtool_path = path + _T("\\signtool.exe");

			check_valid_condition();
			return;
		}
	}

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
		if (m_action == action_codesign_manifest)
		{
			//처음엔 thread_auto_password_input()만 thread로 돌리고
			//codesign_manifest()는 그냥 함수 호출로 실행했으나
			//run_process(cmd, true);로 signtool.exe가 실행되면
			//thread_auto_password_input() 또한 hold 상태가 되어버리므로
			//둘 다 thread로 돌리도록 수정함.
			std::thread th0(&CKoinoToolsDlg::thread_auto_password_input, this);
			th0.detach();

			//codesign_manifest(true);
			std::thread th1(&CKoinoToolsDlg::thread_codesign_manifest, this, true);
			th1.detach();
		}
		else if (m_action == action_codesign_no_manifest)
		{
			std::thread th0(&CKoinoToolsDlg::thread_auto_password_input, this);
			th0.detach();

			//codesign_manifest(false);
			std::thread th1(&CKoinoToolsDlg::thread_codesign_manifest, this, false);
			th1.detach();
		}
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//m_files 파일들을 대상으로 현재 선택된 액션을 취한다.
void CKoinoToolsDlg::thread_codesign_manifest(bool apply_manifest)
{
	if (!check_valid_condition())
		return;

	sctrace(apply_manifest);
	sctrace(m_mt_path);
	sctrace(m_signtool_path);
	sctrace(m_manifest_folder);

	m_in_codesigning = true;
	bool error_occured = false;

	for (int i = 0; i < m_files.size(); i++)
	{
		CString cmd;
		CString filename = get_part(m_files[i], fn_name);
		CString manifest_file = m_manifest_folder + _T("\\") + filename + _T(".manifest");
		CString result;

		m_rich.add(royalblue, _T("codesign start : %s (%d/%d)...\n"), filename, i + 1, m_files.size());

		//파일이 열려있으면 코드사인이 실패하므로 에러로 처리한다.
		//_taccess()를 써봤으나 0이 리턴되고(사용중이 아니라고 판별)
		//_tfopen_s()을 써도 "r"이 가능하다고 판별되어 실행 프로세스인지를 검사하도록 수정함.
		HWND hWnd = get_hwnd_by_exe_file(m_files[i]);
		if (hWnd)
		{
			error_occured = true;
			m_rich.add(red, _T("파일이 사용중이므로 코드사인 할 수 없습니다.\n"));
			break;
		}

		//우선 해당 파일이 이미 codesign되어 있다면 오류가 발생하는 경우가 있으므로 delcert.exe로 지워준다.
		m_rich.add(-1, _T("delcert : %s"), filename);
		cmd.Format(_T("\"%s\\delcert.exe\" \"%s\""), m_signtool_path, m_files[i]);
		result = run_command(cmd);
		m_rich.add(blue, _T(" ok\n"));
		std::this_thread::sleep_for(std::chrono::seconds(1));


		if (apply_manifest)
		{
			if (!PathFileExists(manifest_file))
			{
				AfxMessageBox(_T("manifest 파일이 존재하지 않습니다.\n\n") + manifest_file);
				return;
			}

			cmd.Format(_T("\"%s\" -manifest \"%s\" -outputresource:\"%s\""), m_mt_path, manifest_file, m_files[i]);
			m_rich.add(-1, _T("manifest cmd : %s\n"), cmd);
			result = run_command(cmd);
		}

		m_thread_auto_password_input_paused = false;
		//Wait(10000);
		cmd.Format(_T("\"%s\" sign /sha1 %s /s my /t http://timestamp.digicert.com /fd sha1 /v \"%s\""),
			m_signtool_path, m_fingerprint, m_files[i]);
		m_rich.add(Gdiplus::Color(Gdiplus::Color::DimGray).ToCOLORREF(), _T("#1 phase codesign : %s\n"), cmd);
		result = run_command(cmd);

		while (FindWindowByCaption(_T("토큰 로그온"), true) != NULL)
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m_thread_auto_password_input_paused = false;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cmd.Format(_T("\"%s\" sign /sha1 %s /s my /tr http://timestamp.digicert.com /as /fd SHA256 /td sha256 /v \"%s\""),
			m_signtool_path, m_fingerprint, m_files[i]);
		m_rich.add(Gdiplus::Color(Gdiplus::Color::DimGray).ToCOLORREF(), _T("#2 phase codesign : %s\n"), cmd);
		result = run_command(cmd);

		//std::this_thread::sleep_for(std::chrono::seconds(1));
		while (FindWindowByCaption(_T("토큰 로그온"), true) != NULL)
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m_rich.add(royalblue, _T("%s codesign completed.\n"), filename);
	}

	m_thread_auto_password_input = false;
	while (!m_thread_auto_password_input_terminated)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	if (error_occured)
	{
		m_rich.add(red, _T("All codesign job cancelled.\n"));
	}
	else
	{
		TRACE(_T("codeSign job finished.\n"));
		m_rich.add(blue, _T("All files codesign completed.\n---------------------------------------\n"));
	}

	//20260722 by claude. codesign 중에는 thread_auto_password_input()이 "토큰 로그온" 창을 foreground로 끌어올리므로
	//작업이 끝나는 시점에 이 창은 거의 항상 뒤에 있다. 그래서 완료를 작업표시줄 버튼 깜빡임으로 알린다.
	//CWnd::FlashWindowEx가 전역 ::FlashWindowEx를 가리므로 인자는 (flags, count, timeout) 3개다.
	//이 멤버는 FLASHWINFO를 채워 ::FlashWindowEx를 호출하는 것이 전부라 워커 스레드에서 호출해도 안전하다.
	//timeout = 0이면 시스템 기본 깜빡임 주기를 쓴다.
	FlashWindowEx(FLASHW_ALL, 2, 0);
	m_taskbar_flashing = true;

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
	m_rich.set_line_spacing(1);
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
	int res = AfxMessageBox(label + _T("\n\n위 항목 및 하위 항목들을 모두 삭제합니다.\n항목을 삭제하면 되돌릴 수 없습니다."), MB_OKCANCEL);

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

	m_rich.add(-1, _T("try to stop %s...\n"), service_name);
	status_code = service_command(service_name, _T("stop"), error_code, &detail);

	CString str;
	str.Format(_T("status_code = %d(%s), error_code = %d (%s)"), status_code, get_service_status_str(status_code), error_code, detail);
	m_rich.add(error_code == 0 ? blue : red, _T("%s\n"), str);
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

	m_rich.add(-1, _T("try to restart %s...\n"), service_name);
	status_code = service_command(service_name, _T("restart"), error_code, &detail);

	CString str;
	str.Format(_T("status_code = %d(%s), error_code = %d (%s)"), status_code, get_service_status_str(status_code), error_code, detail);
	m_rich.add(error_code == 0 ? blue : red, _T("%s\n"), str);
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

	m_rich.add(-1, _T("try to delete %s...\n"), service_name);
	status_code = service_command(service_name, _T("delete"), error_code, &detail);

	CString str;
	str.Format(_T("status_code = %d(%s), error_code = %d (%s)"), status_code, get_service_status_str(status_code), error_code, detail);
	m_rich.add(error_code == 0 ? blue : red, _T("%s\n"), str);
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
		AfxMessageBox(_T("not defined"));
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
		AfxMessageBox(_T("not defined"));
	}

	LSTATUS status;
	//3군데 모두 돌면서 동일한 키를 삭제한다.
	for (int i = 0; i < url_scheme_protocols.size(); i++)
	{
		status = RegDeleteTree(HKEY_CLASSES_ROOT, url_scheme_protocols[i]);
		if (status == ERROR_SUCCESS)
			m_rich.addl(blue, _T("HKEY_CLASSES_ROOT\\%s deleted"), url_scheme_protocols[i]);
		else if (status == ERROR_FILE_NOT_FOUND)
			m_rich.addl(blue, _T("HKEY_CLASSES_ROOT\\%s does not exists. skip."), url_scheme_protocols[i]);
		else
			m_rich.addl(red, _T("HKEY_CLASSES_ROOT\\%s delete failed. status = %d"), url_scheme_protocols[i], status);

		status = RegDeleteTree(HKEY_CURRENT_USER, _T("Software\\Classes\\") + url_scheme_protocols[i]);
		if (status == ERROR_SUCCESS)
			m_rich.addl(blue, _T("HKEY_CURRENT_USER\\Software\\Classes\\%s deleted"), url_scheme_protocols[i]);
		else if (status == ERROR_FILE_NOT_FOUND)
			m_rich.addl(blue, _T("HKEY_CURRENT_USER\\%s does not exists. skip."), url_scheme_protocols[i]);
		else
			m_rich.addl(red, _T("HKEY_CURRENT_USER\\Software\\Classes\\%s delete failed"), url_scheme_protocols[i]);

		status = RegDeleteTree(HKEY_LOCAL_MACHINE, _T("Software\\Classes\\") + url_scheme_protocols[i]);
		if (status == ERROR_SUCCESS)
			m_rich.addl(blue, _T("HKEY_LOCAL_MACHINE\\Software\\Classes\\%s deleted"), url_scheme_protocols[i]);
		else if (status == ERROR_FILE_NOT_FOUND)
			m_rich.addl(blue, _T("HKEY_LOCAL_MACHINE\\%s does not exists. skip."), url_scheme_protocols[i]);
		else
			m_rich.addl(red, _T("HKEY_LOCAL_MACHINE\\Software\\Classes\\%s delete failed"), url_scheme_protocols[i]);
	}
}
