
// CentralControllerAideDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "SerialMFC.h"
#include "DataUtil.h"
#include "afxwin.h"
#include <vector>

class CDlgProjectorConfig;
class CDlgCtrlPreset;

// CCentralControllerAideDlg dialog
class CCentralControllerAideDlg : public CDialogEx
{
// Construction
public:
	CCentralControllerAideDlg(CWnd* pParent = NULL);	// standard constructor
	~CCentralControllerAideDlg();

// Dialog Data
	enum { IDD = IDD_CENTRALCONTROLLERAIDE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    afx_msg void OnSelchangeTabCtrlConfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnRecvSerialMsg(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClickedBtnOpenSerialcomm();

    void InitTabCtrl(const CRect& rect);
    void InitSerialCtrl(const CRect& rect);
    BOOL TraverseSerialComm(std::vector<CString>& serialCommList);
    BOOL OpenSerialCmom(const CString& strName);
    void CloseSerialCmom();
	void ParseRecvSerialData(BYTE* byData, ULONG uSize);
    void DispatchSerialMsg(BYTE byFunction, BYTE byErr, const BYTE* lpSerialData, ULONG uSize);

public:
    BOOL IsSerialCmomOpened() const { return m_serial.IsOpen(); }
	BOOL SendSerialData(BYTE* byData, ULONG uSize);

private:
    CTabCtrl m_tabCtrlConfig;
    CDlgProjectorConfig* m_pProjectorConfig;
    CDlgCtrlPreset* m_pCtrlPreset;

	CSerialMFC	m_serial;
    CButton m_editOpenSerialComm;
    BOOL m_bIsOpened;
};
