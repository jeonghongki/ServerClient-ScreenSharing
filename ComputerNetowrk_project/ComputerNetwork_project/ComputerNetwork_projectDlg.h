
// ComputerNetwork_projectDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;

// CComputerNetworkprojectDlg 대화 상자
class CComputerNetworkprojectDlg : public CDialogEx
{
// 생성입니다.
public:
	CComputerNetworkprojectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPUTERNETWORK_PROJECT_DIALOG };
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
private:
	static BOOL CALLBACK GetWindowList(HWND hWnd, LPARAM lParam);
public:
	CStatic m_picture;
	CComboBox m_combo;
	static vector<CWnd*> m_pWnds;
	static CStringArray m_strArray;
	CWnd* m_pCaptureWnd;
	BOOL m_Draw;
	CPoint m_point;
	int m_CaptureMode;
	CIPAddressCtrl m_IPAddress;
	CString m_strMyIP;
	CString m_strOtherIP;
	afx_msg void OnCbnSelchangeComboList();
	void DeleteWindowList();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRadioServer();
	afx_msg void OnBnClickedRadioClient();
	afx_msg void OnBnClickedButtonConnect();
	CEdit m_Port;
};
