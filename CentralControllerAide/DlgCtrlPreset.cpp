// DlgCtrlPreset.cpp : implementation file
//

#include "stdafx.h"
#include "CentralControllerAide.h"
#include "DlgCtrlPreset.h"

#include "afxdialogex.h"
#include "DataUtil.h"
#include "CentralControllerAideDlg.h"

// CDlgCtrlPreset dialog

IMPLEMENT_DYNAMIC(CDlgCtrlPreset, CDialogEx)

CDlgCtrlPreset::CDlgCtrlPreset(
CWnd* pParent /*=NULL*/, CCentralControllerAideDlg* pMainDlg, const CRect& rect)
	: CDialogEx(CDlgCtrlPreset::IDD, pParent)
	, m_pMainDlg(pMainDlg)
    , m_tabClientRect(rect)
{
}

CDlgCtrlPreset::~CDlgCtrlPreset()
{
}

void CDlgCtrlPreset::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCtrlPreset, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_GET_PRESET, &CDlgCtrlPreset::OnClickedBtnGetPreset)
    ON_BN_CLICKED(IDC_BTN_SET_PRESET, &CDlgCtrlPreset::OnClickedBtnSetPreset)
END_MESSAGE_MAP()


// CDlgCtrlPreset message handlers


BOOL CDlgCtrlPreset::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    SetPowerOnSignalCtrlsPos();
    SetDelayPowerOffCtrlsPos();
    SetPresetItemCtrlsPos();
    SetBtnCtrlPos();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCtrlPreset::SetPowerOnSignalCtrlsPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.left += 125;
    groupRect.top += 30;
    groupRect.right -= 125;
    groupRect.bottom = groupRect.top + 65;
    GetDlgItem(IDC_STATIC_POWER_SIGNAL)->MoveWindow(&groupRect);

    LONG nItemWidth = groupRect.right - groupRect.left;
    LONG nItemOffset = (nItemWidth / 4 - 70) / 2;

    CRect platformRadioRect(groupRect);
    platformRadioRect.top += 25;
    platformRadioRect.left += nItemOffset;
    platformRadioRect.right = platformRadioRect.left + 70;
    platformRadioRect.bottom = platformRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_PLATFORM)->MoveWindow(&platformRadioRect);
    ((CButton*)GetDlgItem(IDC_RADIO_PLATFORM))->SetCheck(BST_CHECKED);

    CRect notebookRadioRect(groupRect);
    notebookRadioRect.top += 25;
    notebookRadioRect.left += (nItemWidth / 4) + nItemOffset;
    notebookRadioRect.right = notebookRadioRect.left + 70;
    notebookRadioRect.bottom = notebookRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_NOTEBOOK)->MoveWindow(&notebookRadioRect);

    CRect docCameraRadioRect(groupRect);
    docCameraRadioRect.top += 25;
    docCameraRadioRect.left += (nItemWidth / 2) + nItemOffset;
    docCameraRadioRect.right = docCameraRadioRect.left + 70;
    docCameraRadioRect.bottom = docCameraRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_DOC_CAMERA)->MoveWindow(&docCameraRadioRect);

    CRect dvdRadioRect(groupRect);
    dvdRadioRect.top += 25;
    dvdRadioRect.left += (nItemWidth / 4) * 3 + nItemOffset;
    dvdRadioRect.right = dvdRadioRect.left + 70;
    dvdRadioRect.bottom = dvdRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_DVD)->MoveWindow(&dvdRadioRect);
}

void CDlgCtrlPreset::SetDelayPowerOffCtrlsPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.top += 130;
    groupRect.left += 125;
    groupRect.right -= 125;
    groupRect.bottom = groupRect.top + 65;
    GetDlgItem(IDC_STATIC_DELAY_POWEROFF)->MoveWindow(&groupRect);

    LONG nItemWidth = groupRect.right - groupRect.left;
    LONG nItemOffset = (nItemWidth / 4 - 70) / 2;

    CRect zeroMinRadioRect(groupRect);
    zeroMinRadioRect.top += 25;
    zeroMinRadioRect.left += nItemOffset;
    zeroMinRadioRect.right = zeroMinRadioRect.left + 70;
    zeroMinRadioRect.bottom = zeroMinRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_0MIN)->MoveWindow(&zeroMinRadioRect);

    CRect oneMinRadioRect(groupRect);
    oneMinRadioRect.top += 25;
    oneMinRadioRect.left += (nItemWidth / 4) + nItemOffset;
    oneMinRadioRect.right = oneMinRadioRect.left + 70;
    oneMinRadioRect.bottom = oneMinRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_1MIN)->MoveWindow(&oneMinRadioRect);
    ((CButton*)GetDlgItem(IDC_RADIO_1MIN))->SetCheck(BST_CHECKED);

    CRect towMinsRadioRect(groupRect);
    towMinsRadioRect.top += 25;
    towMinsRadioRect.left += (nItemWidth / 2) + nItemOffset;
    towMinsRadioRect.right = towMinsRadioRect.left + 70;
    towMinsRadioRect.bottom = towMinsRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_2MINS)->MoveWindow(&towMinsRadioRect);

    CRect threeMinsRadioRect(groupRect);
    threeMinsRadioRect.top += 25;
    threeMinsRadioRect.left += (nItemWidth / 4) * 3 + nItemOffset;
    threeMinsRadioRect.right = threeMinsRadioRect.left + 70;
    threeMinsRadioRect.bottom = threeMinsRadioRect.top + 20;
    GetDlgItem(IDC_RADIO_3MINS)->MoveWindow(&threeMinsRadioRect);
}

void CDlgCtrlPreset::SetPresetItemCtrlsPos()
{
    CRect groupRect(m_tabClientRect);
    groupRect.top += 230;
    groupRect.left += 125;
    groupRect.right -= 125;
    groupRect.bottom = groupRect.top + 140;
    GetDlgItem(IDC_STATIC_PRESET_ITEM)->MoveWindow(&groupRect);

    LONG nItemWidth = groupRect.right - groupRect.left;
    LONG nItemOffset = (nItemWidth / 3 - 100) / 2;

    CRect presetItem1Rect(groupRect);
    presetItem1Rect.top += 25;
    presetItem1Rect.left += nItemOffset;
    presetItem1Rect.right = presetItem1Rect.left + 100;
    presetItem1Rect.bottom = presetItem1Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM1)->MoveWindow(&presetItem1Rect);

    CRect presetItem2Rect(groupRect);
    presetItem2Rect.top += 25;
    presetItem2Rect.left += (nItemWidth / 3) + nItemOffset;
    presetItem2Rect.right = presetItem2Rect.left + 100;
    presetItem2Rect.bottom = presetItem2Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM2)->MoveWindow(&presetItem2Rect);

    CRect presetItem3Rect(groupRect);
    presetItem3Rect.top += 25;
    presetItem3Rect.left += (nItemWidth / 3) * 2 + nItemOffset;
    presetItem3Rect.right = presetItem3Rect.left + 100;
    presetItem3Rect.bottom = presetItem3Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM3)->MoveWindow(&presetItem3Rect);

    CRect presetItem4Rect(groupRect);
    presetItem4Rect.top += 65;
    presetItem4Rect.left += nItemOffset;
    presetItem4Rect.right = presetItem4Rect.left + 100;
    presetItem4Rect.bottom = presetItem4Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM4)->MoveWindow(&presetItem4Rect);

    CRect presetItem5Rect(groupRect);
    presetItem5Rect.top += 65;
    presetItem5Rect.left += (nItemWidth / 3) + nItemOffset;
    presetItem5Rect.right = presetItem5Rect.left + 100;
    presetItem5Rect.bottom = presetItem5Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM5)->MoveWindow(&presetItem5Rect);

    CRect presetItem6Rect(groupRect);
    presetItem6Rect.top += 65;
    presetItem6Rect.left += (nItemWidth / 3) * 2 + nItemOffset;
    presetItem6Rect.right = presetItem6Rect.left + 100;
    presetItem6Rect.bottom = presetItem6Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM6)->MoveWindow(&presetItem6Rect);

    CRect presetItem7Rect(groupRect);
    presetItem7Rect.top += 105;
    presetItem7Rect.left += nItemOffset;
    presetItem7Rect.right = presetItem7Rect.left + 100;
    presetItem7Rect.bottom = presetItem7Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM7)->MoveWindow(&presetItem7Rect);

    CRect presetItem8Rect(groupRect);
    presetItem8Rect.top += 105;
    presetItem8Rect.left += (nItemWidth / 3) + nItemOffset;
    presetItem8Rect.right = presetItem8Rect.left + 100;
    presetItem8Rect.bottom = presetItem8Rect.top + 20;
    GetDlgItem(IDC_CHECK_PRESET_ITEM8)->MoveWindow(&presetItem8Rect);
}

void CDlgCtrlPreset::SetBtnCtrlPos()
{
    LONG nItemWidth = m_tabClientRect.right - m_tabClientRect.left;
    LONG nItemOffset = (nItemWidth / 2 - 70) / 2;

    CRect getPresetRect(m_tabClientRect);
    getPresetRect.top = m_tabClientRect.bottom - 63;
    getPresetRect.left += nItemWidth / 2 - 75;
    getPresetRect.right = getPresetRect.left + 70;
    getPresetRect.bottom = m_tabClientRect.bottom - 40;
    GetDlgItem(IDC_BTN_GET_PRESET)->MoveWindow(&getPresetRect);

    CRect setPresetRect(m_tabClientRect);
    setPresetRect.top = m_tabClientRect.bottom - 63;
    setPresetRect.left += nItemWidth / 2 + 5;
    setPresetRect.right = setPresetRect.left + 70;
    setPresetRect.bottom = m_tabClientRect.bottom - 40;
    GetDlgItem(IDC_BTN_SET_PRESET)->MoveWindow(&setPresetRect);
}

void CDlgCtrlPreset::OnRecvGetPresetCode(BYTE byErr, const BYTE* lpSerialData, ULONG uSize)
{
    if (byErr != 0 || NULL == lpSerialData || uSize != 2) {
        MessageBox(_T("获取预设功能码失败！"), _T("错误"), MB_ICONERROR);
        return;
    }

    BYTE byPresetCode1 = *lpSerialData++;
    BYTE byPresetCode2 = *lpSerialData++;
	// 设置丢失报警状态
	if ((byPresetCode1 & 0x80) >> 7 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM1))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode1 & 0x80) >> 7 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM1))->SetCheck(BST_UNCHECKED);
	}

	// 设置开机静音状态
	if ((byPresetCode1 & 0x40) >> 6 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM2))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode1 & 0x40) >> 6 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM2))->SetCheck(BST_UNCHECKED);
	}

	// 设置开机联动投影机状态
	if ((byPresetCode1 & 0x20) >> 5 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM3))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode1 & 0x20) >> 5 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM3))->SetCheck(BST_UNCHECKED);
	}

	// 设置关机联动投影机状态
	if ((byPresetCode1 & 0x10) >> 4 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM4))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode1 & 0x10) >> 4 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM4))->SetCheck(BST_UNCHECKED);
	}

	// 设置开机信号源
	if ((byPresetCode1 & 0x0c) >> 2 == 0x00) {
		((CButton*)GetDlgItem(IDC_RADIO_PLATFORM))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_NOTEBOOK))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DOC_CAMERA))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DVD))->SetCheck(BST_UNCHECKED);
	}
	else if ((byPresetCode1 & 0x0c) >> 2 == 0x01) {
        ((CButton*)GetDlgItem(IDC_RADIO_PLATFORM))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_NOTEBOOK))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DOC_CAMERA))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DVD))->SetCheck(BST_UNCHECKED);
	}
	else if ((byPresetCode1 & 0x0c) >> 2 == 0x02) {
        ((CButton*)GetDlgItem(IDC_RADIO_PLATFORM))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_NOTEBOOK))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DOC_CAMERA))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DVD))->SetCheck(BST_UNCHECKED);
	}
	else if ((byPresetCode1 & 0x0c) >> 2 == 0x03) {
        ((CButton*)GetDlgItem(IDC_RADIO_PLATFORM))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_NOTEBOOK))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DOC_CAMERA))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_DVD))->SetCheck(BST_CHECKED);
	}

	// 设置关机延时时间
	if ((byPresetCode1 & 0x03) == 0x00) {
		((CButton*)GetDlgItem(IDC_RADIO_0MIN))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_1MIN))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_2MINS))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_3MINS))->SetCheck(BST_UNCHECKED);
	}
	else if ((byPresetCode1 & 0x03) == 0x01) {
        ((CButton*)GetDlgItem(IDC_RADIO_0MIN))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_1MIN))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_2MINS))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_3MINS))->SetCheck(BST_UNCHECKED);
	}
	else if ((byPresetCode1 & 0x03) == 0x02) {
        ((CButton*)GetDlgItem(IDC_RADIO_0MIN))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_1MIN))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_2MINS))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_3MINS))->SetCheck(BST_UNCHECKED);
	}
	else if ((byPresetCode1 & 0x03) == 0x03) {
        ((CButton*)GetDlgItem(IDC_RADIO_0MIN))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_1MIN))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_2MINS))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_RADIO_3MINS))->SetCheck(BST_CHECKED);
	}
	
	// 	设置开机联动电脑状态
	if ((byPresetCode2 & 0x80) >> 7 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM5))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode2 & 0x80) >> 7 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM5))->SetCheck(BST_UNCHECKED);
	}

	// 设置关机联动电脑状态
	if ((byPresetCode2 & 0x40) >> 6 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM6))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode2 & 0x40) >> 6 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM6))->SetCheck(BST_UNCHECKED);
	}

	// 设置开机联动门控状态
	if ((byPresetCode2 & 0x20) >> 5 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM7))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode2 & 0x20) >> 5 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM7))->SetCheck(BST_UNCHECKED);
	}

	// 设置关机联动门控状态
	if ((byPresetCode2 & 0x10) >> 4 == 0x01) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM8))->SetCheck(BST_CHECKED);
	}
	else if ((byPresetCode2 & 0x10) >> 4 == 0x00) {
		((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM8))->SetCheck(BST_UNCHECKED);
	}
}

void CDlgCtrlPreset::OnClickedBtnGetPreset()
{
    // TODO: Add your control notification handler code here
    if (!m_pMainDlg->IsSerialCmomOpened()) {
        MessageBox(_T("串口未打开, 获取预设功能码失败！"), _T("错误"), MB_ICONERROR);
        return;
    }

	BYTE byRequest[BUFFER_SIZE] = { 0 };
	ULONG uSize = 0;
	BuildSerialData(FUNCTION_RSET, NULL, 0, byRequest, uSize);
	m_pMainDlg->SendSerialData(byRequest, uSize);
}

void CDlgCtrlPreset::OnRecvSetPresetCode(BYTE byErr)
{
    if (byErr != 0) {
        MessageBox(_T("设置预设功能码失败！"), _T("错误"), MB_ICONERROR);
        return;
    }
}

void CDlgCtrlPreset::OnClickedBtnSetPreset()
{
    // TODO: Add your control notification handler code here
    if (!m_pMainDlg->IsSerialCmomOpened()) {
        MessageBox(_T("串口未打开, 设置预设功能码失败！"), _T("错误"), MB_ICONERROR);
        return;
    }

	BYTE byInput[] = { GetPresetCode1(), GetPresetCode2() };
	BYTE byRequest[BUFFER_SIZE] = { 0 };
	ULONG uSize = 0;
	BuildSerialData(FUNCTION_WSET, byInput, sizeof(byInput), byRequest, uSize);
    if (m_pMainDlg->SendSerialData(byRequest, uSize)) {
        MessageBox(_T("设置预设功能码成功！"), _T("提示"), MB_ICONINFORMATION);
    }
}

BYTE CDlgCtrlPreset::GetPresetCode1()
{
	BYTE byPresetCode1 = 0x00;
	// 获取丢失报警状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM1))->GetCheck()) {
		byPresetCode1 |= 0X80;
	}

	// 获取开机静音状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM2))->GetCheck()) {
		byPresetCode1 |= 0X40;
	}

	// 获取开机联动投影机状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM3))->GetCheck()) {
		byPresetCode1 |= 0X20;
	}

	// 获取关机联动投影机状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM4))->GetCheck()) {
		byPresetCode1 |= 0X10;
	}

	// 获取开机信号源
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_PLATFORM))->GetCheck()) {
		byPresetCode1 |= 0X00;
	}

	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_NOTEBOOK))->GetCheck()) {
		byPresetCode1 |= 0X04;
	}

	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_DOC_CAMERA))->GetCheck()) {
		byPresetCode1 |= 0X08;
	}

	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_DVD))->GetCheck()) {
		byPresetCode1 |= 0X0C;
	}

	// 获取关机延时时间
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_0MIN))->GetCheck()) {
		byPresetCode1 |= 0X10;
	}

	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_1MIN))->GetCheck()) {
		byPresetCode1 |= 0X01;
	}

	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_2MINS))->GetCheck()) {
		byPresetCode1 |= 0X02;
	}

	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_RADIO_3MINS))->GetCheck()) {
		byPresetCode1 |= 0X03;
	}

	return byPresetCode1;
}

BYTE CDlgCtrlPreset::GetPresetCode2()
{
	BYTE byPresetCode2 = 0X00;
	// 获取开机联动电脑状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM5))->GetCheck()) {
		byPresetCode2 |= 0X80;
	}

	// 获取关机联动电脑状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM6))->GetCheck()) {
		byPresetCode2 |= 0X40;
	}

	// 获取开机联动门控状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM7))->GetCheck()) {
		byPresetCode2 |= 0X20;
	}

	// 获取关机联动门控状态
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_PRESET_ITEM8))->GetCheck()) {
		byPresetCode2 |= 0X10;
	}

	return byPresetCode2;
}