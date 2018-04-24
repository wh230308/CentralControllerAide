#pragma once
#include <memory>
#include <tuple>
#include <vector>
#include <string>
#include <map>
#include <condition_variable>
#include <mutex>
#include "afxwin.h"
#include "HexIntegerEdit.h"

class CCentralControllerAideDlg;

// CDlgProjectorConfig dialog

struct ProjectorConfig {
    std::string strBrand;
    std::string strModel;
    BYTE byBaudrate;
    std::string strParity;
    BYTE byDatabits;
    BYTE byStopbits;
    std::string strPoweron;
    std::string strPoweroff;
    std::string strRGB;
    std::string strVIDEO;
    std::string strDelay;
};

typedef std::map<std::string, std::shared_ptr<ProjectorConfig>> ProjectorModelList;
typedef std::map<std::string, std::shared_ptr<ProjectorModelList>> ProjectorBrandList;

struct ConditionGroup{
    std::condition_variable cond;
    std::mutex mtx;
    bool bIsReady = false;
};

class CDlgProjectorConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProjectorConfig)

public:
    CDlgProjectorConfig(CWnd* pParent /*=NULL*/, CCentralControllerAideDlg* pMainDlg, const CRect& rect);
	virtual ~CDlgProjectorConfig();

// Dialog Data
	enum { IDD = IDD_DLG_PROJECTOR_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
    virtual BOOL OnInitDialog();
    void SetProjectorBrandCtrlsPos();
    void SetProjectorModelCtrlsPos();
    void SetProjectorSerialCtrlsPos();
    void SetProjectorOpcodeCtrlPos();
    void SetProjectorTypeCtrlsPos();
    BOOL LoadProjectorConfig(const std::string& filename, ProjectorBrandList& brandList);
	void SetCtrlDefaultValues();
	BOOL UpdateProjectorConfig(const std::string& filename);
	BOOL WriteFile(const std::vector<char>& data);

    afx_msg void OnClickedBtnDelBrand();
    afx_msg void OnClickedBtnDelModel();
    afx_msg void OnClickedBtnGetSerialConfig();
    afx_msg void OnClickedBtnSetSerialConfig();
    afx_msg void OnClickedCheckHexintegerSwitch();
    afx_msg void OnClickedBtnGetProjectorOpcode();
    afx_msg void OnClickedBtnSetProjectorOpcode();
    afx_msg void OnClickedBtnSaveProjectorConfig();
	afx_msg void OnSelchangeListProjectorBrand();
	afx_msg void OnSelchangeListProjectorModel();

public:
    void OpcodeTransform(UINT uEditCtrlResId);
	BOOL GetProjectorOpcode();
	BOOL SetProjectorOpcode();
	BOOL SaveProjectorOpcode();

    void OnRecvGetProjectorSerialConfig(BYTE byErr, const BYTE* bySerialConfig, ULONG uSize);
    void OnRecvSetProjectorSerialConfig(BYTE byErr);
    void OnRecvGetProjectorPowerOnOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize);
    void OnRecvSetProjectorPowerOnOpcode(BYTE byErr);
    void OnRecvGetProjectorPowerOffOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize);
    void OnRecvSetProjectorPowerOffOpcode(BYTE byErr);
    void OnRecvGetProjectorRGBOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize);
    void OnRecvSetProjectorRGBOpcode(BYTE byErr);
    void OnRecvGetProjectorVideoOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize);
    void OnRecvSetProjectorVideoOpcode(BYTE byErr);
    void OnRecvGetProjectorDelayOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize);
    void OnRecvSetProjectorDelayOpcode(BYTE byErr);

private:
    CCentralControllerAideDlg* m_pMainDlg;
    CRect m_tabClientRect;
	CListBox m_listBrand;
	CListBox m_listModel;
    CHexIntegerEdit m_editPowerOn;
    CHexIntegerEdit m_editPowerOff;
    CHexIntegerEdit m_editRGB;
    CHexIntegerEdit m_editVIDEO;
    CHexIntegerEdit m_editDelay;
    ProjectorBrandList m_brandList;

	BOOL m_bIsGettingProjectorOpcode;
	HANDLE m_hGetProjectorOpcodeThread;
	BOOL m_bIsSettingProjectorOpcode;
	HANDLE m_hSetProjectorOpcodeThread;
	BOOL m_bIsSavingProjectorOpcode;
	HANDLE m_hSaveProjectorOpcodeThread;

    ConditionGroup m_getOpcodeCond;
    ConditionGroup m_setOpcodeCond;
};
