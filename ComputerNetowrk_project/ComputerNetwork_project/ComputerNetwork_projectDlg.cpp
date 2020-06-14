
// ComputerNetwork_projectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ComputerNetwork_project.h"
#include "ComputerNetwork_projectDlg.h"
#include "afxdialogex.h"

#include <gdiplus.h>
#include <atlimage.h>
#pragma comment (lib, "Gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// static 멤버변수 초기화
CStringArray CComputerNetworkprojectDlg::m_strArray;
vector <CWnd*> CComputerNetworkprojectDlg::m_pWnds;

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


// CComputerNetworkprojectDlg 대화 상자



CComputerNetworkprojectDlg::CComputerNetworkprojectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPUTERNETWORK_PROJECT_DIALOG, pParent)
	, m_CaptureMode(0)
{
	m_pCaptureWnd = NULL;
	m_Draw = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CComputerNetworkprojectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DRAW, m_picture);
	DDX_Control(pDX, IDC_COMBO_LIST, m_combo);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_IPAddress);
	DDX_Radio(pDX, IDC_RADIO_SERVER, m_CaptureMode);
	DDX_Control(pDX, IDC_EDIT_PORT, m_Port);
}

BEGIN_MESSAGE_MAP(CComputerNetworkprojectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_LIST, &CComputerNetworkprojectDlg::OnCbnSelchangeComboList)
	ON_BN_CLICKED(IDC_REFRESH, &CComputerNetworkprojectDlg::OnBnClickedRefresh)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO_SERVER, &CComputerNetworkprojectDlg::OnBnClickedRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, &CComputerNetworkprojectDlg::OnBnClickedRadioClient)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CComputerNetworkprojectDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CComputerNetworkprojectDlg 메시지 처리기

BOOL CComputerNetworkprojectDlg::OnInitDialog()
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
	// IP주소 가져오기
	char name[255];
	PHOSTENT hostinfo;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((hostinfo = gethostbyname(name)) != NULL)
			m_strMyIP = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
	}

	OnBnClickedRefresh();
	OnBnClickedRadioServer();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CComputerNetworkprojectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CComputerNetworkprojectDlg::OnPaint()
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
		HDC hdc;
		int cx, cy;
		int color_depth;
		CRect wndrect, rect;
		CString temp;
		CImage image;

		// 윈도우 DC 얻기
		hdc = ::GetDC(NULL);
		HBITMAP hBitmap = NULL;

		if (m_pCaptureWnd != NULL)
		{
			if (::IsWindow(m_pCaptureWnd->m_hWnd))
			{
				hdc = m_pCaptureWnd->GetDC()->m_hDC;
				m_pCaptureWnd->GetClientRect(&wndrect);

				cx = wndrect.Width();
				cy = wndrect.Height();

				m_combo.GetLBText(m_combo.GetCurSel(), temp);
				if (temp != "01. [화면공유 프로그램]")
				{
					hBitmap = ::CreateCompatibleBitmap(hdc, wndrect.Width(), wndrect.Height());

					if (!hBitmap)
						MessageBox(_T("해당 윈도우는 이미 종료되었거나, 문제가 있습니다.\n윈도우 목록 새로고침을 수행합니다."), _T("Error"), MB_OK | MB_ICONERROR);

					::SelectObject(hdc, hBitmap);
					::PrintWindow(m_pCaptureWnd->m_hWnd, hdc, 2);

					StretchBlt(hdc, -8, -8, wndrect.right - wndrect.left + 8, wndrect.bottom - wndrect.top + 8, hdc, 0, 0, wndrect.right - wndrect.left - 8, wndrect.bottom - wndrect.top - 8, SRCCOPY);
				}
			}
			else
			{
				OnBnClickedRefresh();
				return;
			}
		}

		CClientDC dc((CStatic*)GetDlgItem(IDC_STATIC_DRAW));
		m_picture.GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		if (m_pCaptureWnd != NULL)
		{
			// 현재 화면의 픽셀당 컬러 비트수 구하기
			color_depth = GetDeviceCaps(hdc, BITSPIXEL);
			image.Create(cx, cy, color_depth, 0); 
			if (cx == 0 || cy == 0)
			{
				m_pCaptureWnd = NULL;
				MessageBox(_T("해당 윈도우는 이미 종료되었거나, 문제가 있습니다.\n윈도우 목록 새로고침을 수행합니다."), _T("Error"), MB_OK | MB_ICONERROR);
				DeleteWindowList();
				OnBnClickedRefresh();
			}
			else
			{
				BitBlt(image.GetDC(), 0, 0, cx, cy, hdc, 0, 0, SRCCOPY);
				dc.SetStretchBltMode(HALFTONE);
				image.Draw(dc, 0, 0, rect.Width(), rect.Height());			
				image.ReleaseDC();
				image.Save(L"Capture.png", Gdiplus::ImageFormatPNG);
			}
		}
		else
		{
			rect.SetRect(0, 0, rect.Width(), rect.Height());
			dc.FillRect(rect, &CBrush(RGB(0, 0, 0)));
		}
		::ReleaseDC(NULL, hdc);

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CComputerNetworkprojectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CComputerNetworkprojectDlg::GetWindowList(HWND hWnd, LPARAM lParam)
{
	// TODO: 여기에 구현 코드 추가.
	CWnd* pWnd = NULL;
	pWnd = CWnd::FromHandle(hWnd);

	if (pWnd)
	{
		// 보이지 않는 윈도우는 출력 안한다
		if (::IsWindowVisible(hWnd) == FALSE)
			return TRUE;
		// 캡션 문자열이 없는 경우 출력 안한다
		if (::GetWindowTextLength(hWnd) == 0)
			return TRUE;
		// 부모가 있는 자식 윈도우는 출력하지 않는다
		if (::GetParent(hWnd) != 0)
			return TRUE;

		// 윈도우 벡터에 윈도우 리스트 저장
		m_pWnds.push_back(pWnd);

		TCHAR title[256] = { 0 };
		::GetWindowText(hWnd, title, 256);

		CString str;
		int num = (int)m_strArray.GetCount() + 1;
		str.Format(_T("%02d. [%s]"), num, title);

		m_strArray.Add(str);
	}
	else
		return FALSE;

	return TRUE;
}


void CComputerNetworkprojectDlg::OnCbnSelchangeComboList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_combo.GetCount() > 0)
	{
		int num = m_combo.GetCurSel();
		// 캡쳐할 윈도우 저장
		m_pCaptureWnd = m_pWnds[num];

		Invalidate(FALSE);
	}
}


void CComputerNetworkprojectDlg::DeleteWindowList()
{
	// TODO: 여기에 구현 코드 추가.

	// 기존 내용들을 삭제한다.
	m_pWnds.clear();
	m_combo.ResetContent();
	m_strArray.RemoveAll();
}


void CComputerNetworkprojectDlg::OnBnClickedRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	DeleteWindowList();

	// 모든 윈도우 리스트를 가져온다.
	EnumWindows(&CComputerNetworkprojectDlg::GetWindowList, 0);

	for (int i = 0; i < m_strArray.GetSize(); i++)
		m_combo.AddString(m_strArray[i]);

	// combo박스의 리스트 제일 처음이 선택되도록 한다.
	if (m_combo.GetCurSel() > 0)
		m_combo.SetCurSel(0);

}


void CComputerNetworkprojectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rt;
	((CStatic*)GetDlgItem(IDC_STATIC_DRAW))->GetWindowRect(&rt);

	ScreenToClient(&rt);
	
	if (rt.PtInRect(point))
	{
		m_Draw = TRUE;
		m_point.x = point.x;
		m_point.y = point.y;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CComputerNetworkprojectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_Draw == TRUE)
	{
		CClientDC dc(this);
		CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen *old_pen = dc.SelectObject(&pen);
		
		CRect rt;
		((CStatic*)GetDlgItem(IDC_STATIC_DRAW))->GetWindowRect(&rt);

		ScreenToClient(&rt);

		if (rt.PtInRect(point))
		{
			dc.MoveTo(m_point.x, m_point.y);
			dc.LineTo(point.x, point.y);
			m_point.x = point.x;
			m_point.y = point.y;
		}	
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CComputerNetworkprojectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 그리기 모드 종료
	if (m_Draw == TRUE)
	{
		m_Draw = FALSE;

		HDC hdc;
		CClientDC dc(m_picture.GetSafeOwner());

		CRect rt;
		((CStatic*)GetDlgItem(IDC_STATIC_DRAW))->GetWindowRect(&rt);

		int color_depth;

		// 윈도우 DC 얻기
		hdc = ::GetDC(NULL);
		// 현재 화면의 해상도 구하기
		color_depth = GetDeviceCaps(hdc, BITSPIXEL);

		CImage image;
		image.Create(rt.Width(), rt.Height(), color_depth, 0);
		BitBlt(image.GetDC(), 0, 0, rt.Width(), rt.Height(), hdc, rt.left, rt.top, SRCCOPY);
		image.ReleaseDC();
		image.Save(L"Capture.png", Gdiplus::ImageFormatPNG);
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CComputerNetworkprojectDlg::OnBnClickedRadioServer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IPAddress.SetWindowText(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);
	m_Port.SetWindowText(_T("8000"));
	m_Port.EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("OPEN"));
}


void CComputerNetworkprojectDlg::OnBnClickedRadioClient()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IPAddress.SetWindowText(_T(""));
	m_IPAddress.EnableWindow(TRUE);
	m_Port.SetWindowText(_T(""));
	m_Port.EnableWindow(TRUE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Connect"));
}


void CComputerNetworkprojectDlg::OnBnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if (!m_CaptureMode)
	{
		((CComputerNetworkprojectApp*)AfxGetApp())->InitServer(m_strMyIP);
		GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	}
	else
	{
		CString strIP, strport;
		GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);
		GetDlgItemText(IDC_EDIT_PORT, strport);
		if ((_ttoi(strport) == 8000) || (_ttoi(strport) == 8001))
		{
			AfxMessageBox(_T("해당 Port는 서버에서 사용하므로 다른 Port 번호를 입력하세요"));
			m_Port.SetWindowText(_T(""));
		}
		else if (strIP != _T("0.0.0.0"))
		{
			GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_IPADDRESS_SERVER)->EnableWindow(FALSE);
			m_strOtherIP = strIP;
			((CComputerNetworkprojectApp*)AfxGetApp())->Connect(strIP, strport);
		}
		else
			AfxMessageBox(_T("접속할 서버의 IP 주소를 입력하세요"));
	}
}
