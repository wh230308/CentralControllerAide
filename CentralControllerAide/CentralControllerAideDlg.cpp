
// CentralControllerAideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CentralControllerAide.h"
#include "CentralControllerAideDlg.h"
#include "DlgProjectorConfig.h"
#include "DlgCtrlPreset.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\SerialLib_d.lib")
#else
#pragma comment(lib, "..\\Release\\SerialLib.lib")
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCentralControllerAideDlg dialog



CCentralControllerAideDlg::CCentralControllerAideDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCentralControllerAideDlg::IDD, pParent)
    , m_bIsOpened(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCentralControllerAideDlg::~CCentralControllerAideDlg()
{
    CloseSerialCmom();
	delete m_pProjectorConfig;
	delete m_pCtrlPreset;
}

void CCentralControllerAideDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_CTRL_CONFIG, m_tabCtrlConfig);
    DDX_Control(pDX, IDC_BTN_OPEN_SERIALCOMM, m_editOpenSerialComm);
}

BEGIN_MESSAGE_MAP(CCentralControllerAideDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CTRL_CONFIG, &CCentralControllerAideDlg::OnSelchangeTabCtrlConfig)
	ON_WM_SERIAL(OnRecvSerialMsg)
    ON_BN_CLICKED(IDC_BTN_OPEN_SERIALCOMM, &CCentralControllerAideDlg::OnClickedBtnOpenSerialcomm)
END_MESSAGE_MAP()


// CCentralControllerAideDlg message handlers

BOOL CCentralControllerAideDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    // 设置主对话框大小，并居中
    MoveWindow(&CRect(0, 0, 800, 650));
    CenterWindow();  
    CRect mainClientRect;
    GetClientRect(&mainClientRect);
    InitTabCtrl(mainClientRect);
    InitSerialCtrl(mainClientRect);
    //TraverseSerialComm();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCentralControllerAideDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
        return;
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCentralControllerAideDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCentralControllerAideDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCentralControllerAideDlg::InitTabCtrl(const CRect& rect)
{
    // 设置TAB CTRL位置，填充主对话框
    CRect mainClientRect(rect);
    mainClientRect.DeflateRect(0, 0, 0, 50);
    m_tabCtrlConfig.MoveWindow(&mainClientRect);

    // TAB添加ITEM，初始化对话框
    m_tabCtrlConfig.InsertItem(0, _T("投影机配置"));
    m_tabCtrlConfig.InsertItem(1, _T("中控预设配置"));

    CRect tabClientRect;
    m_tabCtrlConfig.GetClientRect(&tabClientRect);
    m_pProjectorConfig = new CDlgProjectorConfig(NULL, this, tabClientRect);
    m_pCtrlPreset = new CDlgCtrlPreset(NULL, this, tabClientRect);
    tabClientRect.top += 23;
    m_pProjectorConfig->Create(IDD_DLG_PROJECTOR_CONFIG, &m_tabCtrlConfig);
    m_pProjectorConfig->MoveWindow(&tabClientRect);
    m_pProjectorConfig->ShowWindow(SW_SHOW);
    m_pCtrlPreset->Create(IDD_DLG_CTRL_PRESET, &m_tabCtrlConfig);
    m_pCtrlPreset->MoveWindow(&tabClientRect);
    m_pCtrlPreset->ShowWindow(SW_HIDE);
}

void CCentralControllerAideDlg::InitSerialCtrl(const CRect& rect)
{
    CRect textSerialCommRect(rect);
    textSerialCommRect.left += 10;
    textSerialCommRect.top = rect.bottom - 28;
    textSerialCommRect.right = textSerialCommRect.left + 40;
    textSerialCommRect.bottom = rect.bottom - 8;
    GetDlgItem(IDC_STATIC_SERIALCOMM_NAME)->MoveWindow(&textSerialCommRect);

    CRect comboSerialCommRect(rect);
    textSerialCommRect.left += 35;
    textSerialCommRect.top = rect.bottom - 30;
    textSerialCommRect.right = textSerialCommRect.left + 100;
    textSerialCommRect.bottom = rect.bottom - 10;
    GetDlgItem(IDC_COMBO_SERIALCOMM_LIST)->MoveWindow(&textSerialCommRect); 

    std::vector<CString> serialCommList;
    if (TraverseSerialComm(serialCommList)) {
        for (const auto& item : serialCommList) {
            ((CComboBox*)GetDlgItem(IDC_COMBO_SERIALCOMM_LIST))->AddString(item);
        }
    }

    CRect btnSerialCommRect(rect);
    textSerialCommRect.left += 120;
    textSerialCommRect.top = rect.bottom - 31;
    textSerialCommRect.right = textSerialCommRect.left + 70;
    textSerialCommRect.bottom = rect.bottom - 8;
    GetDlgItem(IDC_BTN_OPEN_SERIALCOMM)->MoveWindow(&textSerialCommRect);
}

BOOL CCentralControllerAideDlg::TraverseSerialComm(std::vector<CString>& serialCommList)
{
    LPCTSTR lpSubKey = _T("Hardware\\DeviceMap\\SerialComm");
    HKEY hKey;
    LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, NULL, KEY_QUERY_VALUE, &hKey);
    if (lResult != ERROR_SUCCESS) {
        MessageBox(_T("打开注册表失败！"), _T("错误"), MB_ICONERROR);
        return FALSE;
    }

#define VALUE_SIZE  100
    DWORD dwIndex = 0;
    TCHAR szValueName[VALUE_SIZE] = { 0 };
    DWORD dwValueNameSize = VALUE_SIZE;
    DWORD dwType = 0;
    BYTE lpData[VALUE_SIZE] = { 0 };
    DWORD dwDataSize = VALUE_SIZE;
    do {
        lResult = RegEnumValue(hKey, dwIndex++, szValueName, &dwValueNameSize, NULL, &dwType, lpData, &dwDataSize);
        if (lResult != ERROR_SUCCESS) {
            if (ERROR_NO_MORE_ITEMS == lResult) {
                break;
            }

            MessageBox(_T("获取串口名失败！"), _T("错误"), MB_ICONERROR);
            continue;
        }

        serialCommList.push_back(CString((LPTSTR)lpData));
        dwValueNameSize = VALUE_SIZE;
        dwDataSize = VALUE_SIZE;
    } while (TRUE);

    return TRUE;
}

void CCentralControllerAideDlg::OnSelchangeTabCtrlConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    *pResult = 0;

    CRect tabClientRect;
    m_tabCtrlConfig.GetClientRect(&tabClientRect);
    tabClientRect.top += 23;
    if (0 == m_tabCtrlConfig.GetCurSel()) {
        m_pProjectorConfig->MoveWindow(&tabClientRect);
        m_pProjectorConfig->ShowWindow(SW_SHOW);
        m_pCtrlPreset->ShowWindow(SW_HIDE);
    }
    else if (1 == m_tabCtrlConfig.GetCurSel()) {
        m_pCtrlPreset->MoveWindow(&tabClientRect);
        m_pCtrlPreset->ShowWindow(SW_SHOW);
        m_pProjectorConfig->ShowWindow(SW_HIDE);
    }
}

BOOL CCentralControllerAideDlg::OpenSerialCmom(const CString& strName)
{
	//尝试打开串口
    if (m_serial.Open(strName, this, 0, 0, 0, 0) != ERROR_SUCCESS) {
		return FALSE;
	}

	//设置串口参数
	if (m_serial.Setup(
		CSerial::EBaudrate::EBaud9600,
		CSerial::EDataBits::EData8,
		CSerial::EParity::EParNone,
		CSerial::EStopBits::EStop1) != ERROR_SUCCESS) {
		return FALSE;
	}

	if (m_serial.SetupHandshaking(CSerial::EHandshakeOff) != ERROR_SUCCESS) {
		return FALSE;
	}

	return TRUE;
}

void CCentralControllerAideDlg::CloseSerialCmom()
{
    m_bIsOpened = FALSE;
    if (m_serial.IsOpen()) {
        m_serial.Close();
    }
}

LRESULT CCentralControllerAideDlg::OnRecvSerialMsg(WPARAM wParam, LPARAM lParam)
{
	CSerial::EEvent eEvent = CSerial::EEvent(LOWORD(wParam));
	CSerial::EError eError = CSerial::EError(HIWORD(wParam));

	if (eEvent & CSerial::EEventRecv) {
		DWORD dwErrors, dwBytesRead = 0, dwBytesToRead = BUFFER_SIZE;
		COMSTAT comStat;
		BYTE byData[BUFFER_SIZE] = { 0 };
		do {
			ClearCommError(m_serial.GetCommHandle(), &dwErrors, &comStat);
			dwBytesToRead = min(dwBytesToRead, comStat.cbInQue);
			if (dwBytesToRead == 0) {
				break;
			}

			if (m_serial.Read(byData, dwBytesToRead, &dwBytesRead) != ERROR_SUCCESS) {
				break;
			}

			if (dwBytesRead > 0) {
				ParseRecvSerialData(byData, dwBytesRead);
			}
		} while (0);
	}

	return 0;
}

BOOL CCentralControllerAideDlg::SendSerialData(BYTE* byData, ULONG uSize)
{
	if (NULL == byData || 0 == uSize) {
		MessageBox(_T("设置串口参数失败，数据为空！"), _T("错误"), MB_ICONERROR);
		return FALSE;
	}

	if (ERROR_SUCCESS != m_serial.Write(byData, uSize)) {
		CString strError;
		strError.Format(_T("设置串口参数失败，err:%u！"), GetLastError());
		MessageBox(strError, _T("错误"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

void CCentralControllerAideDlg::ParseRecvSerialData(BYTE* byData, ULONG uSize)
{
    static BYTE lpRecvBuffer[BUFFER_SIZE] = { 0 };
    static ULONG uRecvSize = 0;
    static BOOL bIsRecvFullPacket = TRUE;  
    if (bIsRecvFullPacket) {
        memset(lpRecvBuffer, 0, sizeof(lpRecvBuffer));
        uRecvSize = 0;
        bIsRecvFullPacket = TRUE;
    }

    memcpy(lpRecvBuffer + uRecvSize, byData, uSize);
    uRecvSize += uSize;
    if (uRecvSize < PACKET_HEADER_SIZE) {
        // 未获取到完整包头
        bIsRecvFullPacket = FALSE;
        return;
    }

    BYTE byFunction = lpRecvBuffer[5];
    BYTE byErr = lpRecvBuffer[6];
    BYTE byLength = lpRecvBuffer[7];
    if (PACKET_HEADER_SIZE + byLength <= uRecvSize) {
        // 已获取到完整包头和完整包体，但可能有多余的数据，多余部分丢弃
        BYTE* lpSerialData = new BYTE[BUFFER_SIZE];
        memcpy(lpSerialData, lpRecvBuffer, PACKET_HEADER_SIZE + byLength);
        DispatchSerialMsg(byFunction, byErr, lpSerialData, PACKET_HEADER_SIZE + byLength);
        bIsRecvFullPacket = TRUE;
    }
    else if (PACKET_HEADER_SIZE + byLength > uRecvSize) {
        // 已获取到完整包头，但未获取到完整包体
        bIsRecvFullPacket = FALSE;
    }
}

void CCentralControllerAideDlg::DispatchSerialMsg(BYTE byFunction, BYTE byErr, const BYTE* lpSerialData, ULONG uSize)
{
    switch (byFunction) {
    case FUNCTION_RSET:
    {
        m_pCtrlPreset->OnRecvGetPresetCode(byErr, lpSerialData + PACKET_HEADER_SIZE, uSize - PACKET_HEADER_SIZE);
        break;
    }
    case FUNCTION_RON:
    {
        m_pProjectorConfig->OnRecvGetProjectorPowerOnOpcode(byErr, lpSerialData + PACKET_HEADER_SIZE, uSize - PACKET_HEADER_SIZE);
        break;
    }
    case FUNCTION_ROFF:
    {
        m_pProjectorConfig->OnRecvGetProjectorPowerOffOpcode(byErr, lpSerialData + PACKET_HEADER_SIZE, uSize - PACKET_HEADER_SIZE);
        break;
    }
    case FUNCTION_RRGB:
    {
        m_pProjectorConfig->OnRecvGetProjectorRGBOpcode(byErr, lpSerialData + PACKET_HEADER_SIZE, uSize - PACKET_HEADER_SIZE);
        break;
    }
    case FUNCTION_RVIDEO:
    {
        m_pProjectorConfig->OnRecvGetProjectorVideoOpcode(byErr, lpSerialData + PACKET_HEADER_SIZE, uSize - PACKET_HEADER_SIZE);
        break;
    }
    case FUNCTION_RDELAY:
    {
        m_pProjectorConfig->OnRecvGetProjectorDelayOpcode(byErr, lpSerialData + PACKET_HEADER_SIZE, uSize - PACKET_HEADER_SIZE);
        break;
    }
    case FUNCTION_RSERIAL:
    {
        m_pProjectorConfig->OnRecvGetProjectorSerialConfig(byErr, lpSerialData + PACKET_HEADER_SIZE, uSize - PACKET_HEADER_SIZE);
        break;
    }
    case FUNCTION_WSET:
    {
        m_pCtrlPreset->OnRecvSetPresetCode(byErr);
        break;
    }
    case FUNCTION_WON:
    {
        m_pProjectorConfig->OnRecvSetProjectorPowerOnOpcode(byErr);
        break;
    }
    case FUNCTION_WOFF:
    {
        m_pProjectorConfig->OnRecvSetProjectorPowerOffOpcode(byErr);
        break;
    }
    case FUNCTION_WRGB:
    {
        m_pProjectorConfig->OnRecvSetProjectorRGBOpcode(byErr);
        break;
    }
    case FUNCTION_WVIDEO:
    {
        m_pProjectorConfig->OnRecvSetProjectorVideoOpcode(byErr);
        break;
    }
    case FUNCTION_WDELAY:
    {
        m_pProjectorConfig->OnRecvSetProjectorDelayOpcode(byErr);
        break;
    }
    case FUNCTION_WSERIAL:
    {
        m_pProjectorConfig->OnRecvSetProjectorSerialConfig(byErr);
        break;
    }
    default:
    {
        MessageBox(_T("未知功能码！"), _T("错误"), MB_ICONERROR);
    }
    }

    delete[] lpSerialData;
}

void CCentralControllerAideDlg::OnClickedBtnOpenSerialcomm()
{
    // TODO: Add your control notification handler code here
    CString strSerialCommName;
    GetDlgItemText(IDC_COMBO_SERIALCOMM_LIST, strSerialCommName);
    if (0 == strSerialCommName.GetLength()) {
        MessageBox(_T("请选择串口！"), _T("错误"), MB_ICONERROR);
    }

    GetDlgItem(IDC_BTN_OPEN_SERIALCOMM)->EnableWindow(FALSE);
    if (!m_bIsOpened) {

        if (OpenSerialCmom(strSerialCommName)) {
            m_bIsOpened = TRUE;
            SetDlgItemText(IDC_BTN_OPEN_SERIALCOMM, _T("关闭"));
        }
    }
    else {
        CloseSerialCmom();
        SetDlgItemText(IDC_BTN_OPEN_SERIALCOMM, _T("打开"));
    }

    GetDlgItem(IDC_BTN_OPEN_SERIALCOMM)->EnableWindow(TRUE);
}
