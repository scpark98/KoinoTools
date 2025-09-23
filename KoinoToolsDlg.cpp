
// KoinoToolsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "KoinoTools.h"
#include "KoinoToolsDlg.h"
#include "afxdialogex.h"

#include <thread>
#include "Common/Functions.h"

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
}

BEGIN_MESSAGE_MAP(CKoinoToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CKoinoToolsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKoinoToolsDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DROPFILES()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CKoinoToolsDlg::OnTvnSelchangedTree)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, &CKoinoToolsDlg::OnLvnEndLabelEditList)
	ON_COMMAND(ID_MENU_TREE_CONTEXT, &CKoinoToolsDlg::OnMenuTreeContext)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, &CKoinoToolsDlg::OnNMRClickTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE, &CKoinoToolsDlg::OnTvnEndLabelEditTree)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST, &CKoinoToolsDlg::OnLvnBeginLabelEditList)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, &CKoinoToolsDlg::OnTvnSelChangingTree)
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

	m_static_code_sign_manifest.set_back_color(Gdiplus::Color::Ivory);
	m_static_code_sign_manifest.set_round(10, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));
	m_static_code_sign_manifest.set_font_size(10);
	m_static_code_sign_manifest.set_tooltip_text(_T("manifest를 적용하여 CodeSign할 파일들을 여기에 drag&drop 합니다.\n(주의 : LMMAgent.exe는 반드시 manifest를 포함하여 CodeSign 해야 함!)"));

	m_static_code_sign_no_manifest.set_back_color(Gdiplus::Color::Ivory);
	m_static_code_sign_no_manifest.set_round(10, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));
	m_static_code_sign_no_manifest.set_font_size(10);
	m_static_code_sign_no_manifest.set_round(10, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));
	m_static_code_sign_no_manifest.set_tooltip_text(_T("manifest를 적용하지 않고 CodeSign할 파일들을 여기에 drag&drop 합니다.\n(주의 : LMMAgent.exe는 반드시 manifest를 포함하여 CodeSign 해야 함!)"));

	init_tree();
	init_list();
	init_rich();

	m_product = theApp.GetProfileString(_T("setting"), _T("recent product"), _T("LinkMeMine"));
	m_tree.select_item(m_product);
	//m_tree.iterate_tree_in_order();

	RestoreWindowPosition(&theApp, this);

	DragAcceptFiles();

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

	CDialogEx::OnCancel();
}

void CKoinoToolsDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

void CKoinoToolsDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

	trace(apply_manifest);
	trace(m_mt_path);
	trace(m_signtool_path);
	trace(m_manifest_folder);

	m_in_codesigning = true;

	for (int i = 0; i < m_files.size(); i++)
	{
		CString cmd;
		CString filename = get_part(m_files[i], fn_name);
		CString manifest_file = m_manifest_folder + _T("\\") + filename + _T(".manifest");
		CString result;

		m_rich.add(-1, _T("codesign start : %s (%d/%d)...\n"), filename, i + 1, m_files.size());

		//우선 해당 파일이 이미 codesign되어 있다면 오류가 발생하는 경우가 있으므로 delcert.exe로 지워준다.
		m_rich.add(-1, _T("delcert : %s"), filename);
		cmd.Format(_T("\"%s\\delcert.exe\" \"%s\""), m_signtool_path, m_files[i]);
		result = run_process(cmd, true);
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
			result = run_process(cmd, true);
		}

		m_thread_auto_password_input_paused = false;
		//Wait(10000);
		cmd.Format(_T("\"%s\" sign /sha1 %s /s my /t http://timestamp.digicert.com /fd sha1 /v \"%s\""),
			m_signtool_path, m_fingerprint, m_files[i]);
		m_rich.add(-1, _T("#1 phase codesign : %s\n"), cmd);
		result = run_process(cmd, true);

		while (FindWindowByCaption(_T("토큰 로그온"), true) != NULL)
			std::this_thread::sleep_for(std::chrono::seconds(1));

		m_thread_auto_password_input_paused = false;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cmd.Format(_T("\"%s\" sign /sha1 %s /s my /tr http://timestamp.digicert.com /as /fd SHA256 /td sha256 /v \"%s\""),
			m_signtool_path, m_fingerprint, m_files[i]);
		m_rich.add(-1, _T("#2 phase codesign : %s\n"), cmd);
		result = run_process(cmd, true);

		std::this_thread::sleep_for(std::chrono::seconds(1));

		m_rich.add(royalblue, _T("%s codesign completed.\n\n"), filename);
	}

	m_thread_auto_password_input = false;
	while (!m_thread_auto_password_input_terminated)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	TRACE(_T("codeSign job finished.\n"));
	m_rich.add(blue, _T("All files codesign completed\n"));

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

	Gdiplus::Color invalid_color = Gdiplus::Color(255, 193, 173);

	if (m_mt_path.IsEmpty() || !PathFileExists(m_mt_path) ||
		m_signtool_path.IsEmpty() || !PathFileExists(m_signtool_path))
	{
		m_list.set_back_color(0, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_back_color(0, -1);
	}

	if (m_manifest_folder.IsEmpty() || !PathIsDirectory(m_manifest_folder))
	{
		m_list.set_back_color(1, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_back_color(1, -1);
	}

	if (m_fingerprint.IsEmpty())
	{
		m_list.set_back_color(2, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_back_color(2, -1);
	}

	if (m_password.IsEmpty())
	{
		//m_list.set_text_color(3, -1, Gdiplus::Color::Crimson);
		m_list.set_back_color(3, -1, invalid_color);
		is_valid = false;
	}
	else
	{
		m_list.reset_back_color(3, -1);
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
		while (m_key_input.get_key_count() > 0)
			std::this_thread::sleep_for(std::chrono::seconds(1));

		//온전히 모두 입력될 때까지 기다려줘야 한다.
		//std::this_thread::sleep_for(std::chrono::seconds(1));

		//OK 버튼 클릭
		sc_mouse_event(mouse_event_lclick, rw.left + 380, rw.top + 258);
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
	int count = theApp.GetProfileInt(_T("product"), _T("count"), 0);

	//맨 처음 실행 시 count가 0이므로 기본 제품들로 채워준다.
	if (count == 0)
	{
		count = 3;
		theApp.WriteProfileInt(_T("product"), _T("count"), 3);
		theApp.WriteProfileString(_T("product"), i2S(0), _T("LinkMeMine"));
		theApp.WriteProfileString(_T("product"), i2S(1), _T("AnySupport"));
		theApp.WriteProfileString(_T("product"), i2S(2), _T("HelpU"));
	}

	for (int i = 0; i < count; i++)
	{
		CString product = theApp.GetProfileString(_T("product"), std::to_wstring(i).c_str(), _T(""));
		if (!product.IsEmpty())
			m_tree.InsertItem(product);
	}

}

void CKoinoToolsDlg::init_list()
{
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES);

	m_list.set_headings(_T("항목,100;경로,300;설명,300"));
	//set_font_name(_T("맑은 고딕"));
	//set_font_size(), set_font_name()을 호출하지 않고 set_header_height()을 호출하면
	//CHeaderCtrlEx::OnLayout()에서 에러가 발생한다.
	m_list.set_header_height(24);
	m_list.set_line_height(21);
	m_list.set_font_size(9);

	//m_list.set_column_data_type(col_filesize, column_data_type_numeric);

	//m_list.set_column_text_align(col_filesize, HDF_RIGHT, false);

	m_list.allow_edit_column(col_item, false);
	m_list.allow_edit_column(col_value, true);
	m_list.allow_edit_column(col_desc, false);

	m_list.load_column_width(&theApp, _T("list"));

	int index = m_list.add_item(_T("signtool path"));
	m_list.add_item(_T("manifest folder"));
	m_list.add_item(_T("fingerprint"));
	m_list.add_item(_T("password"));

	m_list.set_text(0, col_desc, _T("mt.exe, signtool.exe가 위치한 폴더 경로"));
	m_list.set_text(1, col_desc, _T("manifest 파일들이 위치한 폴더 경로"));
	m_list.set_text(2, col_desc, _T("인증서 지문 데이터"));
	m_list.set_text(3, col_desc, _T("CodeSign 암호"));

	//CVtListCtrl에서 header height, line height를 주면 간혹 0번 항목이 헤더에 가려진 채로 시작되는 경우가 있다.
	//뭔가 SetLayout()관련 처리가 부족한 듯 한데 우선 0번 항목을 선택시켜주면 이런 부작용이 나타나진 않는다.
	m_list.select_item(0);
}

void CKoinoToolsDlg::init_rich()
{
	m_rich.ShowTimeInfo(false);
	m_rich.SetLineSpacing(1);
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
	trace(m_product);

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

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CKoinoToolsDlg::OnMenuTreeContext()
{
	m_tree.add_new_item(NULL, _T("새 제품"), true, true);
}

void CKoinoToolsDlg::OnNMRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	return;


	HTREEITEM hItem = NULL;// = pNMTreeView->itemNew.hItem;	//얻어오지 못한다.
	CPoint pt;
	::GetCursorPos(&pt);
	m_tree.ScreenToClient(&pt);
	hItem = m_tree.HitTest(pt);

	if (!hItem)
		return;

	TRACE(_T("label = %s\n"), m_tree.GetItemText(hItem));

	//우클릭을 하면 일단 해당 노드를 선택상태로 만들어줘야 한다.
	m_tree.SelectItem(hItem);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_TREE);

	CMenu* pMenu = menu.GetSubMenu(0);

	::GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}

void CKoinoToolsDlg::OnTvnEndLabelEditTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TRACE(_T("%s\n"), m_tree.get_edit_new_text());
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
