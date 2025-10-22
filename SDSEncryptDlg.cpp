// SDSEncryptDlg.cpp: 구현 파일
//

#include "pch.h"
#include "KoinoTools.h"
#include "afxdialogex.h"
#include "SDSEncryptDlg.h"

#include "Common/Functions.h"

// CSDSEncryptDlg 대화 상자

IMPLEMENT_DYNAMIC(CSDSEncryptDlg, CDialogEx)

CSDSEncryptDlg::CSDSEncryptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SDS_ENCRYPT, pParent)
{

}

CSDSEncryptDlg::~CSDSEncryptDlg()
{
}

void CSDSEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ENCRYPTED, m_edit_encrypted_text);
	DDX_Control(pDX, IDC_EDIT_PLAIN, m_edit_plain_text);
}


BEGIN_MESSAGE_MAP(CSDSEncryptDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSDSEncryptDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSDSEncryptDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_ENCRYPTED, &CSDSEncryptDlg::OnEnChangeEditEncryptedText)
	ON_EN_CHANGE(IDC_EDIT_PLAIN, &CSDSEncryptDlg::OnEnChangeEditPlainText)
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


// CSDSEncryptDlg 메시지 처리기

BOOL CSDSEncryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);
	m_resize.Add(IDC_STATIC_ENCRYPTED, 0, 50, 0, 0);
	m_resize.Add(IDC_EDIT_PLAIN, 0, 0, 100, 50);
	m_resize.Add(IDC_EDIT_ENCRYPTED, 0, 50, 100, 50);

	m_sp.SetHeader("Koino1807!123456\0");
	m_sp.SetKey("SDSLinkMeMine123\0");

	//CSCEdit은 default로 DT_VCENTER지만 이 창에서는 긴 파라미터를 주로 다루므로
	//WordWrap을 위해 DT_TOP으로 변경.
	m_edit_plain_text.set_line_align(DT_TOP);
	m_edit_encrypted_text.set_line_align(DT_TOP);

	RestoreWindowPosition(&theApp, this, _T("SDSEncryptDlg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSDSEncryptDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CSDSEncryptDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CSDSEncryptDlg::OnEnChangeEditEncryptedText()
{
	CString encrypted = m_edit_encrypted_text.get_text();

	std::string encoded = CT2CA(encrypted);
	std::string decoded;

	//base64_decode -> decrypt
	decoded = base64_decode(encoded);
	m_sp.Encrypt((char*)(decoded.c_str()), decoded.size(), false);
	
	CString decrypted(decoded.c_str());
	m_edit_plain_text.set_text(decrypted);
}

void CSDSEncryptDlg::OnEnChangeEditPlainText()
{
	CString decrypted = m_edit_plain_text.get_text();

	std::string decoded = CT2CA(decrypted);
	std::string encoded;

	//encrypt -> base64_encode
	m_sp.Encrypt((char*)(decoded.c_str()), decoded.size(), true);
	encoded = base64_encode(decoded);

	CString encrypted(encoded.c_str());
	m_edit_encrypted_text.set_text(encrypted);
}

void CSDSEncryptDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this, _T("SDSEncryptDlg"));
}
