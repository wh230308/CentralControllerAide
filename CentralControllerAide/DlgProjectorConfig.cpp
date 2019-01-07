// DlgProjectorConfig.cpp : implementation file
//

#include "stdafx.h"
#include "CentralControllerAide.h"
#include "DlgProjectorConfig.h"

#include "afxdialogex.h"
#include <algorithm>
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_helper.hpp"
#include "DataUtil.h"
#include "io.h"
#include "CentralControllerAideDlg.h"

#define CONFIG_LIST_NODE			(_T("ConfigList"))
#define PROJECTOR_CONFIG_NODE		(_T("ProjectorConfig"))
#define PROJECTOR_BRAND_NODE		(_T("Brand"))
#define PROJECTOR_MODEL_NODE		(_T("Model"))
#define PROJECTOR_BAUDRATE_NODE		(_T("Baudrate"))
#define PROJECTOR_PARITY_NODE		(_T("Parity"))
#define PROJECTOR_DATABITS_NODE		(_T("Databits"))
#define PROJECTOR_STOPBITS_NODE		(_T("Stopbits"))
#define PROJECTOR_POWERON_NODE		(_T("PowerOn"))
#define PROJECTOR_POWEROFF_NODE		(_T("PowerOff"))
#define PROJECTOR_RGB_NODE			(_T("RGB"))
#define PROJECTOR_VIDEO_NODE		(_T("VIDEO"))
#define PROJECTOR_DELAY_NODE		(_T("Delay"))

#define PROJECTOR_CONFIG_FILENAME	(_T("ProjectorConfig.xml"))

// CDlgProjectorConfig dialog

IMPLEMENT_DYNAMIC(CDlgProjectorConfig, CDialogEx)

CDlgProjectorConfig::CDlgProjectorConfig(
CWnd* pParent /*=NULL*/, CCentralControllerAideDlg* pMainDlg, const CRect& rect)
	: CDialogEx(CDlgProjectorConfig::IDD, pParent)
	, m_pMainDlg(pMainDlg)
    , m_tabClientRect(rect)
	, m_bIsGettingProjectorOpcode(FALSE)
	, m_hGetProjectorOpcodeThread(NULL)
	, m_bIsSettingProjectorOpcode(FALSE)
	, m_hSetProjectorOpcodeThread(NULL)
	, m_bIsSavingProjectorOpcode(FALSE)
	, m_hSaveProjectorOpcodeThread(NULL)
{
}

CDlgProjectorConfig::~CDlgProjectorConfig()
{
}

void CDlgProjectorConfig::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PROJECTOR_BRAND, m_listBrand);
    DDX_Control(pDX, IDC_LIST_PROJECTOR_MODEL, m_listModel);
    DDX_Control(pDX, IDC_EDIT_POWERON_OPCODE, m_editPowerOn);
    DDX_Control(pDX, IDC_EDIT_POWEROFF_OPCODE, m_editPowerOff);
    DDX_Control(pDX, IDC_EDIT_RGB_OPCODE, m_editRGB);
    DDX_Control(pDX, IDC_EDIT_VIDEO_OPCODE, m_editVIDEO);
    DDX_Control(pDX, IDC_EDIT_DELAY_OPCODE, m_editDelay);
}


BEGIN_MESSAGE_MAP(CDlgProjectorConfig, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_DEL_BRAND, &CDlgProjectorConfig::OnClickedBtnDelBrand)
    ON_BN_CLICKED(IDC_BTN_DEL_MODEL, &CDlgProjectorConfig::OnClickedBtnDelModel)
    ON_BN_CLICKED(IDC_BTN_GET_SERIAL_CONFIG, &CDlgProjectorConfig::OnClickedBtnGetSerialConfig)
    ON_BN_CLICKED(IDC_BTN_SET_SERIAL_CONFIG, &CDlgProjectorConfig::OnClickedBtnSetSerialConfig)
    ON_BN_CLICKED(IDC_BTN_GET_PROJECTOR_OPCODE, &CDlgProjectorConfig::OnClickedBtnGetProjectorOpcode)
    ON_BN_CLICKED(IDC_BTN_SET_PROJECTOR_OPCODE, &CDlgProjectorConfig::OnClickedBtnSetProjectorOpcode)
    ON_BN_CLICKED(IDC_BTN_SAVE_PROJECTOR_CONFIG, &CDlgProjectorConfig::OnClickedBtnSaveProjectorConfig)
	ON_LBN_SELCHANGE(IDC_LIST_PROJECTOR_BRAND, &CDlgProjectorConfig::OnSelchangeListProjectorBrand)
	ON_LBN_SELCHANGE(IDC_LIST_PROJECTOR_MODEL, &CDlgProjectorConfig::OnSelchangeListProjectorModel)
    ON_BN_CLICKED(IDC_CHECK_HEXINTEGER_SWITCH, &CDlgProjectorConfig::OnClickedCheckHexintegerSwitch)
	ON_COMMAND(IDC_RADIO_CODE1, &CDlgProjectorConfig::OnRadioCode1Selected)
	ON_COMMAND(IDC_RADIO_CODE2, &CDlgProjectorConfig::OnRadioCode2Selected)
END_MESSAGE_MAP()


// CDlgProjectorConfig message handlers


BOOL CDlgProjectorConfig::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    SetProjectorBrandCtrlsPos();
    SetProjectorModelCtrlsPos();
    SetProjectorSerialCtrlsPos();
    SetProjectorOpcodeCtrlPos();
    SetProjectorTypeCtrlsPos();
	if (LoadProjectorConfig(PROJECTOR_CONFIG_FILENAME, m_brandList)) {
		SetCtrlDefaultValues();
	}

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProjectorConfig::SetProjectorBrandCtrlsPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.left += 10;
    groupRect.top += 10;
    groupRect.right = m_tabClientRect.right / 2 - 100;
    groupRect.bottom = groupRect.top + 260;
    GetDlgItem(IDC_STATIC_PROJECTOR_BRAND)->MoveWindow(&groupRect);

    CRect listboxBrandRect(groupRect);
    listboxBrandRect.left += 5;
    listboxBrandRect.top += 20;
    listboxBrandRect.right -= 5;
    listboxBrandRect.bottom = listboxBrandRect.top + 200;
    GetDlgItem(IDC_LIST_PROJECTOR_BRAND)->MoveWindow(&listboxBrandRect);

    CRect editDelBrandRect(groupRect);
    editDelBrandRect.left = groupRect.right - 75;
    editDelBrandRect.top = groupRect.bottom - 28;
    editDelBrandRect.right = groupRect.right - 5;
    editDelBrandRect.bottom = groupRect.bottom - 5;
    GetDlgItem(IDC_BTN_DEL_BRAND)->MoveWindow(&editDelBrandRect);
}

void CDlgProjectorConfig::SetProjectorModelCtrlsPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.left += 10;
    groupRect.top += 280;
    groupRect.right = m_tabClientRect.right / 2 - 100;
    groupRect.bottom = groupRect.top + 262;
    GetDlgItem(IDC_STATIC_PROJECTOR_MODEL)->MoveWindow(&groupRect);

    CRect listboxModelRect(groupRect);
    listboxModelRect.left += 5;
    listboxModelRect.top += 20;
    listboxModelRect.right -= 5;
    listboxModelRect.bottom = listboxModelRect.top + 190;
    GetDlgItem(IDC_LIST_PROJECTOR_MODEL)->MoveWindow(&listboxModelRect);

    CRect editDelModelRect(groupRect);
    editDelModelRect.left = groupRect.right - 75;
    editDelModelRect.top = groupRect.bottom - 28;
    editDelModelRect.right = groupRect.right - 5;
    editDelModelRect.bottom = groupRect.bottom - 5;
    GetDlgItem(IDC_BTN_DEL_MODEL)->MoveWindow(&editDelModelRect);
}

void CDlgProjectorConfig::SetProjectorSerialCtrlsPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.left = m_tabClientRect.right / 2 - 90;
    groupRect.top += 10;
    groupRect.right = m_tabClientRect.right - 10;
    groupRect.bottom = groupRect.top + 110;
    GetDlgItem(IDC_STATIC_SERIAL_CONFIG)->MoveWindow(&groupRect);

    LONG nItemWidth = groupRect.right - groupRect.left;
    LONG nItemOffset = (nItemWidth / 4 - 70) / 2;

    CRect textBaudrateRect(groupRect);
    textBaudrateRect.left += nItemOffset;
    textBaudrateRect.top += 25;
    textBaudrateRect.right = textBaudrateRect.left + 70;
    textBaudrateRect.bottom = textBaudrateRect.top + 15;
    GetDlgItem(IDC_STATIC_BAUDRATE)->MoveWindow(&textBaudrateRect);

    CRect comboBaudrateRect(groupRect);
    comboBaudrateRect.left += nItemOffset;
    comboBaudrateRect.top += 42;
    comboBaudrateRect.right = comboBaudrateRect.left + 70;
    comboBaudrateRect.bottom = comboBaudrateRect.top + 20;
    GetDlgItem(IDC_COMBO_BAUDRATE)->MoveWindow(&comboBaudrateRect);
    ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString(_T("9600"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString(_T("19200"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString(_T("38400"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString(_T("56000"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString(_T("57600"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->AddString(_T("115200"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->SetCurSel(0);

    CRect textParityBitsRect(groupRect);
    textParityBitsRect.left += nItemWidth / 4 + nItemOffset;
    textParityBitsRect.top += 25;
    textParityBitsRect.right = textParityBitsRect.left + 70;
    textParityBitsRect.bottom = textParityBitsRect.top + 15;
    GetDlgItem(IDC_STATIC_PARITYBITS)->MoveWindow(&textParityBitsRect);

    CRect comboParityBitsRect(groupRect);
    comboParityBitsRect.left += nItemWidth / 4 + nItemOffset;
    comboParityBitsRect.top += 42;
    comboParityBitsRect.right = comboParityBitsRect.left + 70;
    comboParityBitsRect.bottom = comboParityBitsRect.top + 20;
    GetDlgItem(IDC_COMBO_PARITYBITS)->MoveWindow(&comboParityBitsRect);
    ((CComboBox*)GetDlgItem(IDC_COMBO_PARITYBITS))->AddString(_T("NONE"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_PARITYBITS))->AddString(_T("ODD"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_PARITYBITS))->AddString(_T("EVEN"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_PARITYBITS))->SetCurSel(0);

    CRect textDataBitRect(groupRect);
    textDataBitRect.left += nItemWidth / 2 + nItemOffset;
    textDataBitRect.top += 25;
    textDataBitRect.right = textDataBitRect.left + 70;
    textDataBitRect.bottom = textDataBitRect.top + 15;
    GetDlgItem(IDC_STATIC_DATABIT)->MoveWindow(&textDataBitRect);

    CRect comboDataBitRect(groupRect);
    comboDataBitRect.left += nItemWidth / 2 + nItemOffset;
    comboDataBitRect.top += 42;
    comboDataBitRect.right = comboDataBitRect.left + 70;
    comboDataBitRect.bottom = comboDataBitRect.top + 20;
    GetDlgItem(IDC_COMBO_DATABIT)->MoveWindow(&comboDataBitRect);
    ((CComboBox*)GetDlgItem(IDC_COMBO_DATABIT))->AddString(_T("5"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_DATABIT))->AddString(_T("6"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_DATABIT))->AddString(_T("7"));
	((CComboBox*)GetDlgItem(IDC_COMBO_DATABIT))->AddString(_T("8"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_DATABIT))->SetCurSel(3);

    CRect textStopBitRect(groupRect);
    textStopBitRect.left += (nItemWidth / 4) * 3 + nItemOffset;
    textStopBitRect.top += 25;
    textStopBitRect.right = textStopBitRect.left + 70;
    textStopBitRect.bottom = textStopBitRect.top + 15;
    GetDlgItem(IDC_STATIC_STOPBIT)->MoveWindow(&textStopBitRect);

    CRect comboStopBitRect(groupRect);
    comboStopBitRect.left += (nItemWidth / 4) * 3 + nItemOffset;
    comboStopBitRect.top += 42;
    comboStopBitRect.right = comboStopBitRect.left + 70;
    comboStopBitRect.bottom = comboStopBitRect.top + 20;
    GetDlgItem(IDC_COMBO_STOPBIT)->MoveWindow(&comboStopBitRect);
    ((CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT))->AddString(_T("1"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT))->AddString(_T("1.5"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT))->AddString(_T("2"));
    ((CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT))->SetCurSel(0);

    CRect editGetSerialRect(groupRect);
    editGetSerialRect.left = groupRect.right - 155;
    editGetSerialRect.top = groupRect.bottom - 28;
    editGetSerialRect.right = groupRect.right - 85;
    editGetSerialRect.bottom = groupRect.bottom - 5;
    GetDlgItem(IDC_BTN_GET_SERIAL_CONFIG)->MoveWindow(&editGetSerialRect);

    CRect editSetSerialRect(groupRect);
    editSetSerialRect.left = groupRect.right - 75;
    editSetSerialRect.top = groupRect.bottom - 28;
    editSetSerialRect.right = groupRect.right - 5;
    editSetSerialRect.bottom = groupRect.bottom - 5;
    GetDlgItem(IDC_BTN_SET_SERIAL_CONFIG)->MoveWindow(&editSetSerialRect);
}

void CDlgProjectorConfig::SetProjectorOpcodeCtrlPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.left = m_tabClientRect.right / 2 - 90;
	groupRect.top += 123;
    groupRect.right = m_tabClientRect.right - 10;
    groupRect.bottom = groupRect.top + 315;
    GetDlgItem(IDC_STATIC_PROJECTOR_OPCODE)->MoveWindow(&groupRect);

	CRect textRadioCode1Rect(groupRect);
	textRadioCode1Rect.left = groupRect.right - 253;
	textRadioCode1Rect.top += 20;
	textRadioCode1Rect.right = groupRect.right - 193;
	textRadioCode1Rect.bottom = textRadioCode1Rect.top + 15;
	GetDlgItem(IDC_RADIO_CODE1)->MoveWindow(&textRadioCode1Rect);
	((CButton*)GetDlgItem(IDC_RADIO_CODE1))->SetCheck(BST_CHECKED);

	CRect textRadioCode2Rect(groupRect);
	textRadioCode2Rect.left = groupRect.right - 183;
	textRadioCode2Rect.top += 20;
	textRadioCode2Rect.right = groupRect.right - 123;
	textRadioCode2Rect.bottom = textRadioCode2Rect.top + 15;
	GetDlgItem(IDC_RADIO_CODE2)->MoveWindow(&textRadioCode2Rect);

    CRect textHexSwitchRect(groupRect);
    textHexSwitchRect.left = groupRect.right - 93;
    textHexSwitchRect.top += 20;
    textHexSwitchRect.right = groupRect.right - 13;
    textHexSwitchRect.bottom = textHexSwitchRect.top + 15;
    GetDlgItem(IDC_CHECK_HEXINTEGER_SWITCH)->MoveWindow(&textHexSwitchRect);
    ((CButton*)GetDlgItem(IDC_CHECK_HEXINTEGER_SWITCH))->SetCheck(BST_CHECKED);
	   
    CRect textPowerOnRect(groupRect);
    textPowerOnRect.left += 10;
    textPowerOnRect.top += 30;
    textPowerOnRect.right = textPowerOnRect.left + 50;
    textPowerOnRect.bottom = textPowerOnRect.top + 15;
    GetDlgItem(IDC_STATIC_POWERON_OPCODE)->MoveWindow(&textPowerOnRect);

    CRect editPowerOnRect(groupRect);
    editPowerOnRect.left += 10;
    editPowerOnRect.top += 48;
    editPowerOnRect.right = editPowerOnRect.left + 450;
    editPowerOnRect.bottom = editPowerOnRect.top + 23;
    m_editPowerOn.MoveWindow(&editPowerOnRect);
    m_editPowerOn.EnableHexSwitch(TRUE);

    CRect textPowerOffRect(groupRect);
    textPowerOffRect.left += 10;
    textPowerOffRect.top += 80;
    textPowerOffRect.right = textPowerOffRect.left + 50;
    textPowerOffRect.bottom = textPowerOffRect.top + 15;
    GetDlgItem(IDC_STATIC_POWEROFF_OPCODE)->MoveWindow(&textPowerOffRect);

    CRect editPowerOffRect(groupRect);
    editPowerOffRect.left += 10;
    editPowerOffRect.top += 98;
    editPowerOffRect.right = editPowerOffRect.left + 450;
    editPowerOffRect.bottom = editPowerOffRect.top + 23;
    m_editPowerOff.MoveWindow(&editPowerOffRect);
    m_editPowerOff.EnableHexSwitch(TRUE);

    CRect textRGBRect(groupRect);
    textRGBRect.left += 10;
    textRGBRect.top += 130;
    textRGBRect.right = textRGBRect.left + 50;
    textRGBRect.bottom = textRGBRect.top + 15;
    GetDlgItem(IDC_STATIC_RGB_OPCODE)->MoveWindow(&textRGBRect);

    CRect editRGBRect(groupRect);
    editRGBRect.left += 10;
    editRGBRect.top += 148;
    editRGBRect.right = editRGBRect.left + 450;
    editRGBRect.bottom = editRGBRect.top + 23;
    m_editRGB.MoveWindow(&editRGBRect);
    m_editRGB.EnableHexSwitch(TRUE);

    CRect textVIDEORect(groupRect);
    textVIDEORect.left += 10;
    textVIDEORect.top += 180;
    textVIDEORect.right = textVIDEORect.left + 50;
    textVIDEORect.bottom = textVIDEORect.top + 15;
    GetDlgItem(IDC_STATIC_VIDEO_OPCODE)->MoveWindow(&textVIDEORect);

    CRect editVIDEORect(groupRect);
    editVIDEORect.left += 10;
    editVIDEORect.top += 198;
    editVIDEORect.right = editVIDEORect.left + 450;
    editVIDEORect.bottom = editVIDEORect.top + 23;
    m_editVIDEO.MoveWindow(&editVIDEORect);
    m_editVIDEO.EnableHexSwitch(TRUE);

    CRect textDelayRect(groupRect);
    textDelayRect.left += 10;
    textDelayRect.top += 230;
    textDelayRect.right = textDelayRect.left + 50;
    textDelayRect.bottom = textDelayRect.top + 15;
    GetDlgItem(IDC_STATIC_DELAY_OPCODE)->MoveWindow(&textDelayRect);

    CRect editDelayRect(groupRect);
    editDelayRect.left += 10;
    editDelayRect.top += 248;
    editDelayRect.right = editDelayRect.left + 450;
    editDelayRect.bottom = editDelayRect.top + 23;
    m_editDelay.MoveWindow(&editDelayRect);
    m_editDelay.EnableHexSwitch(TRUE);

    CRect btnGetProjectorOpcodeRect(groupRect);
    btnGetProjectorOpcodeRect.left = groupRect.right - 155;
    btnGetProjectorOpcodeRect.top = groupRect.bottom - 28;
    btnGetProjectorOpcodeRect.right = groupRect.right - 85;
    btnGetProjectorOpcodeRect.bottom = groupRect.bottom - 5;
    GetDlgItem(IDC_BTN_GET_PROJECTOR_OPCODE)->MoveWindow(&btnGetProjectorOpcodeRect);

    CRect btnSetProjectorOpcodeRect(groupRect);
    btnSetProjectorOpcodeRect.left = groupRect.right - 75;
    btnSetProjectorOpcodeRect.top = groupRect.bottom - 28;
    btnSetProjectorOpcodeRect.right = groupRect.right - 5;
    btnSetProjectorOpcodeRect.bottom = groupRect.bottom - 5;
    GetDlgItem(IDC_BTN_SET_PROJECTOR_OPCODE)->MoveWindow(&btnSetProjectorOpcodeRect);
}

void CDlgProjectorConfig::SetProjectorTypeCtrlsPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.left = m_tabClientRect.right / 2 - 90;
    groupRect.top = m_tabClientRect.bottom - 90;
    groupRect.right = m_tabClientRect.right - 10;
    groupRect.bottom = m_tabClientRect.bottom - 30;
    GetDlgItem(IDC_STATIC_SAVE_PROJECTOR_CONFIG)->MoveWindow(&groupRect);

    CRect textBrandRect(groupRect);
    textBrandRect.left += 10;
    textBrandRect.top += 32;
    textBrandRect.right = textBrandRect.left + 25;
    textBrandRect.bottom = textBrandRect.top + 20;
    GetDlgItem(IDC_STATIC_BRAND)->MoveWindow(&textBrandRect);
    SetDlgItemText(IDC_STATIC_BRAND, _T("品牌"));

    CRect editBrandRect(groupRect);
    editBrandRect.left += 40;
    editBrandRect.top += 30;
    editBrandRect.right = editBrandRect.left + 70;
    editBrandRect.bottom = editBrandRect.top + 23;
    GetDlgItem(IDC_EDIT_BRAND)->MoveWindow(&editBrandRect);

    CRect textModelRect(groupRect);
    textModelRect.left += 130;
    textModelRect.top += 32;
    textModelRect.right = textModelRect.left + 25;
    textModelRect.bottom = textModelRect.top + 20;
    GetDlgItem(IDC_STATIC_MODEL)->MoveWindow(&textModelRect);
    SetDlgItemText(IDC_STATIC_MODEL, _T("型号"));

    CRect editModelRect(groupRect);
    editModelRect.left += 160;
    editModelRect.top += 30;
    editModelRect.right = editModelRect.left + 70;
    editModelRect.bottom = editModelRect.top + 23;
    GetDlgItem(IDC_EDIT_MODEL)->MoveWindow(&editModelRect);

    CRect btnSaveProjectorConfigRect(groupRect);
    btnSaveProjectorConfigRect.left = groupRect.right - 110;
    btnSaveProjectorConfigRect.top += 32;
    btnSaveProjectorConfigRect.right = groupRect.right - 5;
    btnSaveProjectorConfigRect.bottom = btnSaveProjectorConfigRect.top + 23;
    GetDlgItem(IDC_BTN_SAVE_PROJECTOR_CONFIG)->MoveWindow(&btnSaveProjectorConfigRect);
}

BOOL CDlgProjectorConfig::LoadProjectorConfig(const std::string& filename, ProjectorBrandList& brandList)
{
	if (-1 == _access(filename.c_str(), 0)) {
		return FALSE;
	}

	try {
		rapidxml::file<> file(filename.c_str());
		rapidxml::xml_document<> doc;
		doc.parse<0>(file.data());
		rapidxml::xml_node<>* root = doc.first_node();
		if (nullptr == root) {
			return FALSE;
		}

		rapidxml::xml_node<char>* element = root->first_node(PROJECTOR_CONFIG_NODE);
		while (element != nullptr) {
			auto pConfig = std::make_shared<ProjectorConfig>();
			std::string strBrand;
			rapidxml_helper::read_xml_element(element, PROJECTOR_BRAND_NODE, strBrand);
			pConfig->strBrand = strBrand;

			std::string strModel;
			rapidxml_helper::read_xml_element(element, PROJECTOR_MODEL_NODE, strModel);
			pConfig->strModel = strModel;

			int nBaudrate;
			rapidxml_helper::read_xml_element(element, PROJECTOR_BAUDRATE_NODE, nBaudrate);
			pConfig->byBaudrate = (BYTE)nBaudrate;

			std::string strParity;
			rapidxml_helper::read_xml_element(element, PROJECTOR_PARITY_NODE, strParity);
			pConfig->strParity = strParity;

			int nDatabits;
			rapidxml_helper::read_xml_element(element, PROJECTOR_DATABITS_NODE, nDatabits);
			pConfig->byDatabits = (BYTE)nDatabits;

			int nStopbits;
			rapidxml_helper::read_xml_element(element, PROJECTOR_STOPBITS_NODE, nStopbits);
			pConfig->byStopbits = (BYTE)nStopbits;

			std::string strPoweron;
			rapidxml_helper::read_xml_element(element, PROJECTOR_POWERON_NODE, strPoweron);
			pConfig->strPoweron = strPoweron;

			std::string strPoweroff;
			rapidxml_helper::read_xml_element(element, PROJECTOR_POWEROFF_NODE, strPoweroff);
			pConfig->strPoweroff = strPoweroff;

			std::string strRGB;
			rapidxml_helper::read_xml_element(element, PROJECTOR_RGB_NODE, strRGB);
			pConfig->strRGB = strRGB;

			std::string strVIDEO;
			rapidxml_helper::read_xml_element(element, PROJECTOR_VIDEO_NODE, strVIDEO);
			pConfig->strVIDEO = strVIDEO;

			std::string strDelay;
			rapidxml_helper::read_xml_element(element, PROJECTOR_DELAY_NODE, strDelay);
			pConfig->strDelay = strDelay;
			
            auto iter = brandList.find(strBrand);
            if (iter != brandList.end()) {
				auto modelList = iter->second;
                auto iter2 = modelList->find(strModel);
                if (iter2 == modelList->end()) {
                    modelList->insert(std::make_pair(strModel, pConfig));
				}
			}
			else {
                auto modelList = std::make_shared<ProjectorModelList>();
                modelList->insert(std::make_pair(strModel, pConfig));
                brandList[strBrand] = modelList;
			}

			element = element->next_sibling();
		}
	}
	catch (std::exception e) {
		CString strErr;
		strErr.Format(_T("加载投影机配置失败, Reason:%s"), e.what());
        MessageBox(strErr, _T("错误"), MB_ICONERROR);
	}

	return TRUE;
}

void CDlgProjectorConfig::SetCtrlDefaultValues()
{
    if (m_brandList.empty()) {
		return;
	}

    for (const auto& item : m_brandList) {
		std::string brand = item.first;
		int nSel = m_listBrand.AddString(brand.c_str());
		m_listBrand.SetItemHeight(nSel, 18);
	}
	m_listBrand.SetCurSel(0);

    ProjectorModelList projectorModelList = *m_brandList.begin()->second;
	for (const auto& item : projectorModelList) {
		std::string model = item.first;
		int nSel = m_listModel.AddString(model.c_str());
		m_listModel.SetItemHeight(nSel, 18);
	}
}

BOOL CDlgProjectorConfig::WriteFile(const std::vector<char>& data)
{
	FILE* pFile = NULL;
	errno_t err = _tfopen_s(&pFile, PROJECTOR_CONFIG_FILENAME, _T("wt"));
	CString strErr;
	if (err != 0) {
		strErr.Format(_T("open file failed, errno:%u"), errno);
		MessageBox(strErr, _T("错误"), MB_ICONERROR);
		return FALSE;
	}

	size_t uBytesWritten = fwrite((void*)data.data(), sizeof(char), data.size(), pFile);
	if (uBytesWritten != data.size()) {
		strErr.Format(_T("write file failed, errno:%u"), errno);
		MessageBox(strErr, _T("错误"), MB_ICONERROR);
		return FALSE;
	}

	if (fclose(pFile) != 0) {
		strErr.Format(_T("close file failed, errno:%u"), errno);
		MessageBox(strErr, _T("错误"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgProjectorConfig::UpdateProjectorConfig(const std::string& filename)
{
	rapidxml::xml_document<char> document;
	std::vector<char> data;
	try {
		auto root_node = document.allocate_node(rapidxml::node_element, CONFIG_LIST_NODE);
		document.append_node(root_node);
        for (const auto& item : m_brandList) {
            auto modelList = *item.second;
            for (const auto& item2 : modelList) {
                std::shared_ptr<ProjectorConfig> pConfig = item2.second;
                auto projector_config_node = document.allocate_node(rapidxml::node_element, PROJECTOR_CONFIG_NODE);

                auto brand_node = document.allocate_node(rapidxml::node_element, PROJECTOR_BRAND_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strBrand));
                projector_config_node->append_node(brand_node);

                auto model_node = document.allocate_node(rapidxml::node_element, PROJECTOR_MODEL_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strModel));
                projector_config_node->append_node(model_node);

                auto baudrate_node = document.allocate_node(rapidxml::node_element, PROJECTOR_BAUDRATE_NODE,
                    rapidxml_helper::uint32_to_xml_str(document, (uint32_t)pConfig->byBaudrate));
                projector_config_node->append_node(baudrate_node);

                auto parity_node = document.allocate_node(rapidxml::node_element, PROJECTOR_PARITY_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strParity));
                projector_config_node->append_node(parity_node);

                auto databits_node = document.allocate_node(rapidxml::node_element, PROJECTOR_DATABITS_NODE,
                    rapidxml_helper::uint32_to_xml_str(document, (uint32_t)pConfig->byDatabits));
                projector_config_node->append_node(databits_node);

                auto stopbits_node = document.allocate_node(rapidxml::node_element, PROJECTOR_STOPBITS_NODE,
                    rapidxml_helper::uint32_to_xml_str(document, (uint32_t)pConfig->byStopbits));
                projector_config_node->append_node(stopbits_node);

                auto poweron_node = document.allocate_node(rapidxml::node_element, PROJECTOR_POWERON_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strPoweron));
                projector_config_node->append_node(poweron_node);

                auto poweroff_node = document.allocate_node(rapidxml::node_element, PROJECTOR_POWEROFF_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strPoweroff));
                projector_config_node->append_node(poweroff_node);

                auto rgb_node = document.allocate_node(rapidxml::node_element, PROJECTOR_RGB_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strRGB));
                projector_config_node->append_node(rgb_node);

                auto video_node = document.allocate_node(rapidxml::node_element, PROJECTOR_VIDEO_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strVIDEO));
                projector_config_node->append_node(video_node);

                auto delay_node = document.allocate_node(rapidxml::node_element, PROJECTOR_DELAY_NODE,
                    rapidxml_helper::string_to_xml_str(document, pConfig->strDelay));
                projector_config_node->append_node(delay_node);

                root_node->append_node(projector_config_node);
            }
        }
		rapidxml::print(std::back_inserter(data), document);
	}
	catch (std::exception &e) {
		CString strErr;
		strErr.Format(_T("更新投影机配置失败, Reason: %s"), e.what());
        MessageBox(strErr, _T("错误"), MB_ICONERROR);
	}

	return WriteFile(data);
}

void CDlgProjectorConfig::OnClickedBtnDelBrand()
{
    // TODO: Add your control notification handler code here
    int nSel = ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_BRAND))->GetCurSel();
    if (LB_ERR == nSel) {
        MessageBox(_T("请选择要删除的品牌"), _T("错误"), MB_ICONERROR);
        return;
    }

    CString strBrand;
    auto iter = m_brandList.find(strBrand.GetBuffer());
    if (iter != m_brandList.end()) {
        m_brandList.erase(iter);
    }

    if (UpdateProjectorConfig(PROJECTOR_CONFIG_FILENAME)) {
        ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_BRAND))->DeleteString(nSel);
        ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_MODEL))->ResetContent();
    }
}

void CDlgProjectorConfig::OnClickedBtnDelModel()
{
    // TODO: Add your control notification handler code here
    int nSel = ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_MODEL))->GetCurSel();
    if (LB_ERR == nSel) {
        MessageBox(_T("请选择要删除的型号"), _T("错误"), MB_ICONERROR);
        return;
    }
    CString strModel;
    ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_MODEL))->GetText(nSel, strModel);

    nSel = ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_BRAND))->GetCurSel();
    CString strBrand;
    ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_BRAND))->GetText(nSel, strBrand);

    auto iter = m_brandList.find(strBrand.GetBuffer());
    if (iter != m_brandList.end()) {
        auto modelList = iter->second;
        auto iter2 = modelList->find(strModel.GetBuffer());
        if (iter2 != modelList->end()) {
            modelList->erase(iter2);
        }
    }

    if (UpdateProjectorConfig(PROJECTOR_CONFIG_FILENAME)) {
        ((CListBox*)GetDlgItem(IDC_LIST_PROJECTOR_MODEL))->DeleteString(nSel);
    }
}

int GetBaudRateCodeIndex(const BYTE byBaudRateCode)
{
	switch (byBaudRateCode) {
	case 0x01:
		return 5;
	case 0x02:
		return 4;
	case 0x03:
		return 3;
	case 0x04:
		return 2;
	case 0x05:
		return 1;
	case 0x06:
		return 0;
	default:
		return 0;
	}
}

int GetParityBitsCodeIndex(const BYTE byParityBitsCode)
{
	switch (byParityBitsCode) {
	case 0x00:
		return 0;
	case 0x01:
		return 1;
	case 0x02:
		return 2;
	default:
		return 0;
	}
}

int GetDataBitCodeIndex(const BYTE byDataBitCode)
{
	switch (byDataBitCode) {
	case 0x05:
		return 0;
	case 0x06:
		return 1;
	case 0x07:
		return 2;
	case 0x08:
		return 3;
	default:
		return 4;
	}
}

int GetStopBitCodeIndex(const BYTE byStopBitCode)
{
	switch (byStopBitCode) {
	case 0x01:
		return 0;
	case 0x02:
		return 1;
	case 0x03:
		return 2;
	default:
		return 0;
	}
}

void CDlgProjectorConfig::OnRecvGetProjectorSerialConfig(BYTE byErr, const BYTE* bySerialConfig, ULONG uSize)
{
    if (byErr != 0 || NULL == bySerialConfig || uSize != 4) {
		MessageBox(_T("获取投影机串口参数失败！"), _T("错误"), MB_ICONERROR);
		return;
	}

	CComboBox* comboBaudRate = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	comboBaudRate->SetCurSel(GetBaudRateCodeIndex(*bySerialConfig++));
    CComboBox* comboDataBit = (CComboBox*)GetDlgItem(IDC_COMBO_DATABIT);
    comboDataBit->SetCurSel(GetDataBitCodeIndex(*bySerialConfig++));
	CComboBox* comboParityBits = (CComboBox*)GetDlgItem(IDC_COMBO_PARITYBITS);
	comboParityBits->SetCurSel(GetParityBitsCodeIndex(*bySerialConfig++));
	CComboBox* comboStopBit = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT);
	comboStopBit->SetCurSel(GetStopBitCodeIndex(*bySerialConfig++));
}

void CDlgProjectorConfig::OnClickedBtnGetSerialConfig()
{
    // TODO: Add your control notification handler code here
    if (!m_pMainDlg->IsSerialCmomOpened()) {
        MessageBox(_T("串口未打开, 获取串口参数失败！"), _T("错误"), MB_ICONERROR);
        return;
    }

	BYTE byRequest[BUFFER_SIZE] = { 0 };
	ULONG uSize = 0;
	BuildSerialData(1, FUNCTION_RSERIAL, NULL, 0, byRequest, uSize);
	m_pMainDlg->SendSerialData(byRequest, uSize);
}

BYTE GetBaudRateCode(const CString& strBaudRate)
{
	if (0 == strBaudRate.Compare(_T("115200"))) {
		return 0x01;
	}
	else if (0 == strBaudRate.Compare(_T("57600"))) {
		return 0x02;
	}
	else if (0 == strBaudRate.Compare(_T("56000"))) {
		return 0x03;
	}
	else if (0 == strBaudRate.Compare(_T("38400"))) {
		return 0x04;
	}
	else if (0 == strBaudRate.Compare(_T("19200"))) {
		return 0x05;
	}
	else if (0 == strBaudRate.Compare(_T("9600"))) {
		return 0x06;
	}
	else {
		return 0x06;
	}
}

BYTE GetDataBitCode(const CString& strDataBit)
{
	if (0 == strDataBit.Compare(_T("5"))) {
		return 0x05;
	}
	else if (0 == strDataBit.Compare(_T("6"))) {
		return 0x06;
	}
	else if (0 == strDataBit.Compare(_T("7"))) {
		return 0x07;
	}
	else if (0 == strDataBit.Compare(_T("8"))) {
		return 0x08;
	}
	else {
		return 0x08;
	}
}

BYTE GetParityBitsCode(const CString& strParityBits)
{
	if (0 == strParityBits.Compare(_T("NONE"))) {
		return 0x00;
	}
	else if (0 == strParityBits.Compare(_T("ODD"))) {
		return 0x01;
	}
	else if (0 == strParityBits.Compare(_T("EVEN"))) {
		return 0x02;
	}
	else {
		return 0x00;
	}
}

BYTE GetStopBitCode(const CString& strStopBit)
{
	if (0 == strStopBit.Compare(_T("1"))) {
		return 0x01;
	}
	else if (0 == strStopBit.Compare(_T("1.5"))) {
		return 0x02;
	}
	else if (0 == strStopBit.Compare(_T("2"))) {
		return 0x03;
	}
	else {
		return 0x01;
	}
}

void CDlgProjectorConfig::OnRecvSetProjectorSerialConfig(BYTE byErr)
{
    if (byErr != 0) {
        MessageBox(_T("设置投影机串口参数失败！"), _T("错误"), MB_ICONERROR);
        return;
    }
}

void CDlgProjectorConfig::OnClickedBtnSetSerialConfig()
{
    // TODO: Add your control notification handler code here
    if (!m_pMainDlg->IsSerialCmomOpened()) {
        MessageBox(_T("串口未打开, 设置串口参数失败！"), _T("错误"), MB_ICONERROR);
        return;
    }

	CString strBaudRate, strDataBit, strParityBits, strStopBit;
	GetDlgItemText(IDC_COMBO_BAUDRATE, strBaudRate);
	GetDlgItemText(IDC_COMBO_DATABIT, strDataBit);
	GetDlgItemText(IDC_COMBO_PARITYBITS, strParityBits);
	GetDlgItemText(IDC_COMBO_STOPBIT, strStopBit);

	BYTE byInput[BUFFER_SIZE] = { 0 }, byRequest[BUFFER_SIZE] = { 0 };
	ULONG uLength = 0, uSize = 0;
	byInput[uLength++] = GetBaudRateCode(strBaudRate);
	byInput[uLength++] = GetDataBitCode(strDataBit);
	byInput[uLength++] = GetParityBitsCode(strParityBits);
	byInput[uLength++] = GetStopBitCode(strStopBit);
	BuildSerialData(1, FUNCTION_WSERIAL, byInput, uLength, byRequest, uSize);
    if (m_pMainDlg->SendSerialData(byRequest, uSize)) {
        MessageBox(_T("设置串口参数成功！"), _T("提示"), MB_ICONINFORMATION);
    }
}

BOOL CDlgProjectorConfig::GetProjectorOpcode(BYTE byIndex)
{
	BOOL bResult = TRUE;
	BYTE functions[] = { FUNCTION_RON, FUNCTION_ROFF, FUNCTION_RRGB, FUNCTION_RVIDEO, FUNCTION_RDELAY };
	for (const auto item : functions) {
		BYTE byRequest[100] = { 0 };
		ULONG uSize = 0;
		BuildSerialData(byIndex, item, NULL, 0, byRequest, uSize);
		bResult = m_pMainDlg->SendSerialData(byRequest, uSize);
        Sleep(100);

        {
            std::unique_lock<std::mutex> lock(m_getOpcodeCond.mtx);
            m_getOpcodeCond.cond.wait_for(lock, std::chrono::milliseconds(1000), [this] { return m_getOpcodeCond.bIsReady; });
            m_getOpcodeCond.bIsReady = false;
        }
    }

	m_bIsGettingProjectorOpcode = FALSE;
	GetDlgItem(IDC_BTN_GET_PROJECTOR_OPCODE)->EnableWindow(TRUE);
	CloseHandle(m_hSetProjectorOpcodeThread);
	m_hSetProjectorOpcodeThread = NULL;
	return bResult;
}

unsigned int WINAPI GetProjectorOpcodeThreadProc(void* pParam)
{
	CDlgProjectorConfig* pDlg = (CDlgProjectorConfig*)pParam;
    BYTE byIndex = 0;
    if (BST_CHECKED == ((CButton*)pDlg->GetDlgItem(IDC_RADIO_CODE1))->GetCheck()) {
        byIndex = 1;
    }
    else if (BST_CHECKED == ((CButton*)pDlg->GetDlgItem(IDC_RADIO_CODE2))->GetCheck()) {
        byIndex = 2;
    }

	pDlg->GetProjectorOpcode(byIndex);

	return 0;
}

void CDlgProjectorConfig::OnRecvGetProjectorPowerOnOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize)
{
    do {
        if (byErr != 0 || NULL == byOpcode || 0 == uSize) {
            MessageBox(_T("获取投影机开机码失败！"), _T("错误"), MB_ICONERROR);
            break;
        }

        CString strOpcode;
        ByteArray2String(byOpcode, uSize, strOpcode);
        SetDlgItemText(IDC_EDIT_POWERON_OPCODE, strOpcode);
    } while (0);

    {
        std::unique_lock<std::mutex> lock(m_getOpcodeCond.mtx);
        m_getOpcodeCond.bIsReady = true;
    }
    m_getOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvSetProjectorPowerOnOpcode(BYTE byErr)
{
    if (byErr != 0) {
        MessageBox(_T("设置投影机开机码失败！"), _T("错误"), MB_ICONERROR);
    }

    {
        std::unique_lock<std::mutex> lock(m_setOpcodeCond.mtx);
        m_setOpcodeCond.bIsReady = true;
    }
    m_setOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvGetProjectorPowerOffOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize)
{
    do {
        if (byErr != 0  || NULL == byOpcode || 0 == uSize) {
            MessageBox(_T("获取投影机关机码失败！"), _T("错误"), MB_ICONERROR);
            break;
        }

        CString strOpcode;
        ByteArray2String(byOpcode, uSize, strOpcode);
        SetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, strOpcode);
    } while (0);

    {
        std::unique_lock<std::mutex> lock(m_getOpcodeCond.mtx);
        m_getOpcodeCond.bIsReady = true;
    }
    m_getOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvSetProjectorPowerOffOpcode(BYTE byErr)
{
    if (byErr != 0) {
        MessageBox(_T("设置投影机关机码失败！"), _T("错误"), MB_ICONERROR);
    }

    {
        std::unique_lock<std::mutex> lock(m_setOpcodeCond.mtx);
        m_setOpcodeCond.bIsReady = true;
    }
    m_setOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvGetProjectorRGBOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize)
{
    do {
        if (byErr != 0 || NULL == byOpcode || 0 == uSize) {
            MessageBox(_T("获取投影机RGB码失败！"), _T("错误"), MB_ICONERROR);
            break;
        }

        CString strOpcode;
        ByteArray2String(byOpcode, uSize, strOpcode);
        SetDlgItemText(IDC_EDIT_RGB_OPCODE, strOpcode);
    } while (0);

    {
        std::unique_lock<std::mutex> lock(m_getOpcodeCond.mtx);
        m_getOpcodeCond.bIsReady = true;
    }
    m_getOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvSetProjectorRGBOpcode(BYTE byErr)
{
    if (byErr != 0) {
        MessageBox(_T("设置投影机RGB码失败！"), _T("错误"), MB_ICONERROR);
    }

    {
        std::unique_lock<std::mutex> lock(m_setOpcodeCond.mtx);
        m_setOpcodeCond.bIsReady = true;
    }
    m_setOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvGetProjectorVideoOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize)
{
    do {
        if (byErr != 0 || NULL == byOpcode || 0 == uSize) {
            MessageBox(_T("获取投影机VIDEO码失败！"), _T("错误"), MB_ICONERROR);
            break;
        }

        CString strOpcode;
        ByteArray2String(byOpcode, uSize, strOpcode);
        SetDlgItemText(IDC_EDIT_VIDEO_OPCODE, strOpcode);
    } while (false);

    {
        std::unique_lock<std::mutex> lock(m_getOpcodeCond.mtx);
        m_getOpcodeCond.bIsReady = true;
    }
    m_getOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvSetProjectorVideoOpcode(BYTE byErr)
{
    if (byErr != 0) {
        MessageBox(_T("设置投影机VIDEO码失败！"), _T("错误"), MB_ICONERROR);
    }

    {
        std::unique_lock<std::mutex> lock(m_setOpcodeCond.mtx);
        m_setOpcodeCond.bIsReady = true;
    }
    m_setOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvGetProjectorDelayOpcode(BYTE byErr, const BYTE* byOpcode, ULONG uSize)
{
    do {
        if (byErr != 0 || NULL == byOpcode || 0 == uSize) {
            MessageBox(_T("获取投影机休眠码失败！"), _T("错误"), MB_ICONERROR);
            break;
        }

        CString strOpcode;
        ByteArray2String(byOpcode, uSize, strOpcode);
        SetDlgItemText(IDC_EDIT_DELAY_OPCODE, strOpcode);
    } while (false);

    {
        std::unique_lock<std::mutex> lock(m_getOpcodeCond.mtx);
        m_getOpcodeCond.bIsReady = true;
    }
    m_getOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OnRecvSetProjectorDelayOpcode(BYTE byErr)
{
    if (byErr != 0) {
        MessageBox(_T("设置投影机休眠码失败！"), _T("错误"), MB_ICONERROR);
    }

    {
        std::unique_lock<std::mutex> lock(m_setOpcodeCond.mtx);
        m_setOpcodeCond.bIsReady = true;
    }
    m_setOpcodeCond.cond.notify_one();
}

void CDlgProjectorConfig::OpcodeTransform(UINT uEditCtrlResId)
{
    CString strProjectorOpcode;
    GetDlgItemText(uEditCtrlResId, strProjectorOpcode);
    if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_HEXINTEGER_SWITCH))->GetCheck()) {
        ((CHexIntegerEdit*)GetDlgItem(uEditCtrlResId))->EnableHexSwitch(TRUE);
        if (strProjectorOpcode.IsEmpty()) {
            return;
        }

        CString strHexSequence;
        CharacterSequence2HexSequence(strProjectorOpcode, strHexSequence);
        SetDlgItemText(uEditCtrlResId, strHexSequence);
    }
    else if (BST_UNCHECKED == ((CButton*)GetDlgItem(IDC_CHECK_HEXINTEGER_SWITCH))->GetCheck()) {
        ((CHexIntegerEdit*)GetDlgItem(uEditCtrlResId))->EnableHexSwitch(FALSE);
        if (strProjectorOpcode.IsEmpty()) {
            return;
        }

        CString strCharacterSequence;
        HexSequence2CharacterSequence(strProjectorOpcode, strCharacterSequence);
        SetDlgItemText(uEditCtrlResId, strCharacterSequence);
    }
}

void CDlgProjectorConfig::OnClickedCheckHexintegerSwitch()
{
    // TODO: Add your control notification handler code here
    OpcodeTransform(IDC_EDIT_POWERON_OPCODE);
    OpcodeTransform(IDC_EDIT_POWEROFF_OPCODE);
    OpcodeTransform(IDC_EDIT_RGB_OPCODE);
    OpcodeTransform(IDC_EDIT_VIDEO_OPCODE);
    OpcodeTransform(IDC_EDIT_DELAY_OPCODE);
}

void CDlgProjectorConfig::OnRadioCode1Selected()
{
	// TODO: Add your command handler code here
	GetDlgItemText(IDC_EDIT_POWERON_OPCODE, m_strTempPowerOnCodes[1]);
	GetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, m_strTempPowerOffCodes[1]);
	GetDlgItemText(IDC_EDIT_RGB_OPCODE, m_strTempPowerRGBCodes[1]);
	GetDlgItemText(IDC_EDIT_VIDEO_OPCODE, m_strTempPowerVIDEOCodes[1]);
	GetDlgItemText(IDC_EDIT_DELAY_OPCODE, m_strTempPowerDelayCodes[1]);

	SetDlgItemText(IDC_EDIT_POWERON_OPCODE, m_strTempPowerOnCodes[0]);
	SetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, m_strTempPowerOffCodes[0]);
	SetDlgItemText(IDC_EDIT_RGB_OPCODE, m_strTempPowerRGBCodes[0]);
	SetDlgItemText(IDC_EDIT_VIDEO_OPCODE, m_strTempPowerVIDEOCodes[0]);
	SetDlgItemText(IDC_EDIT_DELAY_OPCODE, m_strTempPowerDelayCodes[0]);
}


void CDlgProjectorConfig::OnRadioCode2Selected()
{
	// TODO: Add your command handler code here
	GetDlgItemText(IDC_EDIT_POWERON_OPCODE, m_strTempPowerOnCodes[0]);
	GetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, m_strTempPowerOffCodes[0]);
	GetDlgItemText(IDC_EDIT_RGB_OPCODE, m_strTempPowerRGBCodes[0]);
	GetDlgItemText(IDC_EDIT_VIDEO_OPCODE, m_strTempPowerVIDEOCodes[0]);
	GetDlgItemText(IDC_EDIT_DELAY_OPCODE, m_strTempPowerDelayCodes[0]);

	SetDlgItemText(IDC_EDIT_POWERON_OPCODE, m_strTempPowerOnCodes[1]);
	SetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, m_strTempPowerOffCodes[1]);
	SetDlgItemText(IDC_EDIT_RGB_OPCODE, m_strTempPowerRGBCodes[1]);
	SetDlgItemText(IDC_EDIT_VIDEO_OPCODE, m_strTempPowerVIDEOCodes[1]);
	SetDlgItemText(IDC_EDIT_DELAY_OPCODE, m_strTempPowerDelayCodes[1]);
}

void CDlgProjectorConfig::OnClickedBtnGetProjectorOpcode()
{
    // TODO: Add your control notification handler code here
    if (!m_pMainDlg->IsSerialCmomOpened()) {
        MessageBox(_T("串口未打开, 获取投影机码失败！"), _T("错误"), MB_ICONERROR);
        return;
    }

	if (!m_bIsGettingProjectorOpcode) {
		m_bIsGettingProjectorOpcode = TRUE;
		GetDlgItem(IDC_BTN_GET_PROJECTOR_OPCODE)->EnableWindow(FALSE);
		if (NULL == m_hSetProjectorOpcodeThread) {
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, GetProjectorOpcodeThreadProc, this, CREATE_SUSPENDED, NULL);
			if (NULL == hThread) {
				return;
			}

			m_hSetProjectorOpcodeThread = hThread;
			ResumeThread(hThread);
		}
	}
}

BOOL CDlgProjectorConfig::SetProjectorOpcode(BYTE byIndex)
{
	BOOL bResult = TRUE;
	do {
		CString strPowerOnOpcode;
		GetDlgItemText(IDC_EDIT_POWERON_OPCODE, strPowerOnOpcode);
		CString strPowerOffOpcode;
		GetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, strPowerOffOpcode);
		CString strRGBOpcode;
		GetDlgItemText(IDC_EDIT_RGB_OPCODE, strRGBOpcode);
		CString strVIDEOpcode;
		GetDlgItemText(IDC_EDIT_VIDEO_OPCODE, strVIDEOpcode);
		CString strDelayOpcode;
		GetDlgItemText(IDC_EDIT_DELAY_OPCODE, strDelayOpcode);
		if (0 == strPowerOnOpcode.GetLength() && 0 == strPowerOffOpcode.GetLength()) {
			MessageBox(_T("投影机码为空, 请输入投影机码！"), _T("错误"), MB_ICONERROR);
			break;
		}

		std::vector<std::tuple<BYTE, CString>> projectorOpcodes;
		projectorOpcodes.push_back(std::make_tuple(FUNCTION_WON, strPowerOnOpcode));
		projectorOpcodes.push_back(std::make_tuple(FUNCTION_WOFF, strPowerOffOpcode));
		projectorOpcodes.push_back(std::make_tuple(FUNCTION_WRGB, strRGBOpcode));
		projectorOpcodes.push_back(std::make_tuple(FUNCTION_WVIDEO, strVIDEOpcode));
		projectorOpcodes.push_back(std::make_tuple(FUNCTION_WDELAY, strDelayOpcode));
		for (const auto& item : projectorOpcodes) {
			BYTE byFunction = std::get<0>(item);
			CString strInput = std::get<1>(item);
			BYTE byInput[BUFFER_SIZE] = { 0 }, byRequest[BUFFER_SIZE] = { 0 };
			ULONG uLength = 0, uSize = 0;
			String2ByteArray(strInput, byInput, uLength);
			BuildSerialData(byIndex, byFunction, byInput, uLength, byRequest, uSize);
			bResult = m_pMainDlg->SendSerialData(byRequest, uSize);
            Sleep(100);
            {
                std::unique_lock<std::mutex> lock(m_setOpcodeCond.mtx);
                m_setOpcodeCond.cond.wait_for(lock, std::chrono::milliseconds(1000), [this] { return m_setOpcodeCond.bIsReady; });
                m_setOpcodeCond.bIsReady = false;
            }
		}

        MessageBox(_T("设置投影机码成功！"), _T("提示"), MB_ICONINFORMATION);
	} while (false);

	m_bIsSettingProjectorOpcode = FALSE;
	GetDlgItem(IDC_BTN_SET_PROJECTOR_OPCODE)->EnableWindow(TRUE);
	CloseHandle(m_hSetProjectorOpcodeThread);
	m_hSetProjectorOpcodeThread = NULL;

	return bResult;
}

unsigned int WINAPI SetProjectorOpcodeThreadProc(void* pParam)
{
	CDlgProjectorConfig* pDlg = (CDlgProjectorConfig*)pParam;
	BYTE byCodeIndex = 0;
	if (BST_CHECKED == ((CButton*)pDlg->GetDlgItem(IDC_RADIO_CODE1))->GetCheck()) {
		byCodeIndex = 1;
	}
	else if (BST_CHECKED == ((CButton*)pDlg->GetDlgItem(IDC_RADIO_CODE2))->GetCheck()) {
		byCodeIndex = 2;
	}
	pDlg->SetProjectorOpcode(byCodeIndex);

	return 0;
}

void CDlgProjectorConfig::OnClickedBtnSetProjectorOpcode()
{
    // TODO: Add your control notification handler code here
    if (!m_pMainDlg->IsSerialCmomOpened()) {
        MessageBox(_T("串口未打开, 设置投影机码失败！"), _T("错误"), MB_ICONERROR);
        return;
    }

	if (!m_bIsSettingProjectorOpcode) {
		m_bIsSettingProjectorOpcode = TRUE;
		GetDlgItem(IDC_BTN_SET_PROJECTOR_OPCODE)->EnableWindow(FALSE);
		if (NULL == m_hSetProjectorOpcodeThread) {
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, SetProjectorOpcodeThreadProc, this, CREATE_SUSPENDED, NULL);
			if (NULL == hThread) {
				return;
			}

			m_hSetProjectorOpcodeThread = hThread;
			ResumeThread(m_hSetProjectorOpcodeThread);
		}
	}
}

BOOL CDlgProjectorConfig::SaveProjectorOpcode()
{
	BOOL bResult = TRUE;
	do {
		CString strBrand;
		GetDlgItemText(IDC_EDIT_BRAND, strBrand);
		CString strModel;
		GetDlgItemText(IDC_EDIT_MODEL, strModel);
		if (0 == strBrand.GetLength() || 0 == strModel.GetLength()) {
			MessageBox(_T("请输入品牌和型号！"), _T("错误"), MB_ICONERROR);
			break;
		}

		CString strPowerOnOpcode;
		GetDlgItemText(IDC_EDIT_POWERON_OPCODE, strPowerOnOpcode);
		CString strPowerOffOpcode;
		GetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, strPowerOffOpcode);
		CString strRGBOpcode;
		GetDlgItemText(IDC_EDIT_RGB_OPCODE, strRGBOpcode);
		CString strVIDEOpcode;
		GetDlgItemText(IDC_EDIT_VIDEO_OPCODE, strVIDEOpcode);
		CString strDelayOpcode;
		GetDlgItemText(IDC_EDIT_DELAY_OPCODE, strDelayOpcode);
		if (0 == strPowerOnOpcode.GetLength() && 0 == strPowerOffOpcode.GetLength()) {
			MessageBox(_T("投影机码为空, 请输入投影机码！"), _T("错误"), MB_ICONERROR);
			break;
		}

		auto pConfig = std::make_shared<ProjectorConfig>();
		CString strBaudRate;
		GetDlgItemText(IDC_COMBO_BAUDRATE, strBaudRate);
		pConfig->byBaudrate = GetBaudRateCode(strBaudRate);
		CString strParityBits;
		GetDlgItemText(IDC_COMBO_PARITYBITS, strParityBits);
		pConfig->strParity = strParityBits;
		CString strDataBit;
		GetDlgItemText(IDC_COMBO_DATABIT, strDataBit);
        pConfig->byDatabits = GetDataBitCode(strDataBit);
		CString strStopBit;
		GetDlgItemText(IDC_COMBO_STOPBIT, strStopBit);
        pConfig->byStopbits = GetStopBitCode(strStopBit);
		pConfig->strBrand = strBrand;
		pConfig->strModel = strModel;
		pConfig->strPoweron = strPowerOnOpcode;
		pConfig->strPoweroff = strPowerOffOpcode;
		pConfig->strRGB = strRGBOpcode;
		pConfig->strVIDEO = strVIDEOpcode;
		pConfig->strDelay = strDelayOpcode;

        auto iter = m_brandList.find(strBrand.GetBuffer());
        if (iter != m_brandList.end()) {
            auto modelList = iter->second;
            (*modelList)[strModel.GetBuffer()] = pConfig;
        }
        else {
            auto pModelList = std::make_shared<ProjectorModelList>();
            pModelList->insert(std::make_pair(strModel.GetBuffer(), pConfig));
            m_brandList[strBrand.GetBuffer()] = pModelList;
        }

        if (UpdateProjectorConfig(PROJECTOR_CONFIG_FILENAME)) {
            if (LB_ERR == m_listBrand.FindString(-1, strBrand)) {
                int nSel = m_listBrand.AddString(strBrand);
                m_listBrand.SetCurSel(nSel);
            }

            if (LB_ERR == m_listModel.FindString(-1, strModel)) {
                int nSel = m_listModel.AddString(strModel);
                m_listModel.SetCurSel(nSel);
            }
        }

        MessageBox(_T("保存投影机码成功！"), _T("提示"), MB_ICONINFORMATION);
	} while (false);

	m_bIsSavingProjectorOpcode = FALSE;
	GetDlgItem(IDC_BTN_SAVE_PROJECTOR_CONFIG)->EnableWindow(TRUE);
	CloseHandle(m_hSaveProjectorOpcodeThread);
	m_hSaveProjectorOpcodeThread = NULL;

	return bResult;
}

unsigned int WINAPI SaveProjectorOpcodeThreadProc(void* pParam)
{
	CDlgProjectorConfig* pDlg = (CDlgProjectorConfig*)pParam;
	pDlg->SaveProjectorOpcode();

	return 0;
}

void CDlgProjectorConfig::OnClickedBtnSaveProjectorConfig()
{
    // TODO: Add your control notification handler code here
	if (!m_bIsSavingProjectorOpcode) {
		m_bIsSavingProjectorOpcode = TRUE;
		GetDlgItem(IDC_BTN_SAVE_PROJECTOR_CONFIG)->EnableWindow(FALSE);
		if (NULL == m_hSaveProjectorOpcodeThread) {
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, SaveProjectorOpcodeThreadProc, this, CREATE_SUSPENDED, NULL);
			if (NULL == hThread) {
				return;
			}

			m_hSaveProjectorOpcodeThread = hThread;
			ResumeThread(m_hSaveProjectorOpcodeThread);
		}
	}
}

void CDlgProjectorConfig::OnSelchangeListProjectorBrand()
{
	// TODO: Add your control notification handler code here
    CString strBrand;
	int nSel = m_listBrand.GetCurSel();
    if (LB_ERR == nSel) {
        return;
    }
	m_listBrand.GetText(nSel, strBrand);

    std::shared_ptr<ProjectorModelList> pModelList;
    auto iter = m_brandList.find(strBrand.GetBuffer());
    if (iter != m_brandList.end()) {
        pModelList = iter->second;
	}

	m_listModel.ResetContent();
    for (const auto& item : *pModelList) {
		int nSel = m_listModel.AddString(item.first.c_str());
		m_listModel.SetItemHeight(nSel, 18);
	}
	SetDlgItemText(IDC_EDIT_POWERON_OPCODE, _T(""));
	SetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, _T(""));
	SetDlgItemText(IDC_EDIT_RGB_OPCODE, _T(""));
	SetDlgItemText(IDC_EDIT_VIDEO_OPCODE, _T(""));
	SetDlgItemText(IDC_EDIT_DELAY_OPCODE, _T(""));
}


void CDlgProjectorConfig::OnSelchangeListProjectorModel()
{
	// TODO: Add your control notification handler code here
    CString strModel;
    int nSel = m_listModel.GetCurSel();
    if (LB_ERR == nSel) {
        return;
    }
    m_listModel.GetText(nSel, strModel);

    CString strBrand;
    nSel = m_listBrand.GetCurSel();
    if (LB_ERR == nSel) {
        return;
    }
    m_listBrand.GetText(nSel, strBrand);

    auto iter = m_brandList.find(strBrand.GetBuffer());
    if (iter != m_brandList.end()) {
        auto pModelList = iter->second;
        auto iter2 = pModelList->find(strModel.GetBuffer());
        if (iter2 != pModelList->end()) {
            auto projectorConfig = iter2->second;
            BYTE byBaudrate = projectorConfig->byBaudrate;
            CComboBox* comboBaudRate = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
            comboBaudRate->SetCurSel(GetBaudRateCodeIndex(byBaudrate));

            std::string strParity = projectorConfig->strParity;
            CComboBox* comboParity = (CComboBox*)GetDlgItem(IDC_COMBO_PARITYBITS);
            BYTE byParity = GetParityBitsCode(CString(strParity.c_str()));
            comboParity->SetCurSel(GetParityBitsCodeIndex(byParity));

            BYTE byDatabits = projectorConfig->byDatabits;
            CComboBox* comboDatabits = (CComboBox*)GetDlgItem(IDC_COMBO_DATABIT);
            comboDatabits->SetCurSel(GetDataBitCodeIndex(byDatabits));

            BYTE byStopbits = projectorConfig->byStopbits;
            CComboBox* comboStopbits = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBIT);
            comboStopbits->SetCurSel(GetStopBitCodeIndex(byStopbits));

            SetDlgItemText(IDC_EDIT_POWERON_OPCODE, projectorConfig->strPoweron.c_str());
            SetDlgItemText(IDC_EDIT_POWEROFF_OPCODE, projectorConfig->strPoweroff.c_str());
            SetDlgItemText(IDC_EDIT_RGB_OPCODE, projectorConfig->strRGB.c_str());
            SetDlgItemText(IDC_EDIT_VIDEO_OPCODE, projectorConfig->strVIDEO.c_str());
            SetDlgItemText(IDC_EDIT_DELAY_OPCODE, projectorConfig->strDelay.c_str());
            SetDlgItemText(IDC_EDIT_BRAND, projectorConfig->strBrand.c_str());
            SetDlgItemText(IDC_EDIT_MODEL, projectorConfig->strModel.c_str());
        }
    }
}
