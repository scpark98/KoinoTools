
// KoinoToolsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "KoinoTools.h"
#include "KoinoToolsDlg.h"
#include "afxdialogex.h"

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
	m_resize.Add(IDC_LIST, 0, 0, 100, 100);

	m_static_code_sign_manifest.set_back_color(Gdiplus::Color::Ivory);
	m_static_code_sign_manifest.set_round(10, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));
	m_static_code_sign_manifest.set_font_size(10);
	m_static_code_sign_manifest.set_tooltip_text(_T("LMMAgent.exe는 반드시 manifest를 포함할 것"));


	m_static_code_sign_no_manifest.set_back_color(Gdiplus::Color::Ivory);
	m_static_code_sign_no_manifest.set_round(10, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));
	m_static_code_sign_no_manifest.set_font_size(10);
	m_static_code_sign_no_manifest.set_round(10, Gdiplus::Color::Gray, get_sys_color(COLOR_3DFACE));

	init_tree();
	init_list();

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
	if (nIDEvent == timer_)
	{
		KillTimer(timer_);

		//CWnd* pWnd = FindWindowByCaption(_T("토큰 로그온"), true);
		CWnd* pWnd = FindWindowByCaption(_T("KoinoTools"), true);
		CRect rw;

		if (!pWnd)
			return;

		SetForegroundWindowForce(pWnd->m_hWnd, true);

		pWnd->GetWindowRect(rw);

		//암호 입력창으로 커서 이동
		SetCursorPos(rw.left + 220, rw.top + 181);

		//이미 입력된 암호가 있다면 지우기 위해 더블클릭 후 제거
		sc_mouse_event(mouse_event_ldbclick);
		keybd_event(VK_BACK, 0, 0, 0);

		//지정된 암호를 입력한다.
		CString pw = _T("Koino1807!");
		m_ki.add(pw);

		//OK 버튼 클릭
		Wait(1000);
		sc_mouse_event(mouse_event_lclick, rw.left + 380, rw.top + 258);
	}

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

	POINT pt;
	DragQueryPoint(hDropInfo, &pt);

	// Convert to client coordinates of main window
	::ClientToScreen(m_hWnd, &pt);        // absolute screen
	::ScreenToClient(m_hWnd, &pt);        // back to dialog client coords (safer)

	// Find child control at this point
	CWnd* pCtrl = ChildWindowFromPoint(pt);
	if (pCtrl == &m_static_code_sign_manifest)
		m_action = action_codesign_manifest;
		//TRACE(_T("m_static_code_sign_manifest\n"));
	else if (pCtrl == &m_static_code_sign_no_manifest)
		m_action = action_codesign_no_manifest;
		//TRACE(_T("m_static_code_sign_no_manifest\n"));

	TCHAR sfile[MAX_PATH];

	int count = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);

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
			codesign_manifest(false);
		else if (m_action == action_codesign_manifest)
			codesign_manifest(true);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//m_files 파일들을 대상으로 현재 선택된 액션을 취한다.
void CKoinoToolsDlg::codesign_manifest(bool manifest)
{

}

void CKoinoToolsDlg::init_tree()
{

}

void CKoinoToolsDlg::init_list()
{
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES);

	m_list.set_headings(_T("항목,100;경로,300;"));
	m_list.set_font_size(9);
	//set_font_name(_T("맑은 고딕"));
	//set_font_size(), set_font_name()을 호출하지 않고 set_header_height()을 호출하면
	//CHeaderCtrlEx::OnLayout()에서 에러가 발생한다.
	m_list.set_header_height(24);
	m_list.set_line_height(21);

	//m_list.set_column_data_type(col_filesize, column_data_type_numeric);

	//m_list.set_column_text_align(col_filesize, HDF_RIGHT, false);

	m_list.allow_edit_column(col_item, false);
	m_list.allow_edit_column(col_value, true);
	m_list.load_column_width(&theApp, _T("list"));

	int index = m_list.add_item(_T("mt path"));
	//m_list.set_item_text(index, col_value, theApp.m_mt_path);
	index = m_list.add_item(_T("signtool path"));
	index = m_list.add_item(_T("manifest path"));
}
