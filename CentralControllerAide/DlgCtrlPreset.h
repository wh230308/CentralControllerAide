#pragma once

class CCentralControllerAideDlg;

// CDlgCtrlPreset dialog

class CDlgCtrlPreset : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCtrlPreset)

public:
    CDlgCtrlPreset(CWnd* pParent /*=NULL*/, CCentralControllerAideDlg* pMainDlg, const CRect& rect);
	virtual ~CDlgCtrlPreset();

// Dialog Data
	enum { IDD = IDD_DLG_CTRL_PRESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnClickedBtnGetPreset();
	afx_msg void OnClickedBtnSetPreset();

    virtual BOOL OnInitDialog();
    void SetPowerOnSignalCtrlsPos();
    void SetDelayPowerOffCtrlsPos();
    void SetPresetItemCtrlsPos();
    void SetBtnCtrlPos();
	BYTE GetPresetCode1();
	BYTE GetPresetCode2();

public:
    void OnRecvGetPresetCode(BYTE byErr, const BYTE* lpSerialData, ULONG uSize);
    void OnRecvSetPresetCode(BYTE byErr);

private:
    CCentralControllerAideDlg* m_pMainDlg;
    CRect m_tabClientRect;	
};
