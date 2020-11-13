
// BasicDemoDlg.cpp : implementation file
// 

#include "stdafx.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int number = 0;
int m_int_1 = 200;
int m_result = 0;
Mat img, dst,src;
CBasicDemoDlg* CBasicDemoDlg::pThis;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBasicDemoDlg dialog




CBasicDemoDlg::CBasicDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBasicDemoDlg::IDD, pParent)
    , m_pcMyCamera(NULL)
    , m_nDeviceCombo(0)
    , m_bOpenDevice(FALSE)
    , m_bStartGrabbing(false)
    , m_nTriggerMode(MV_TRIGGER_MODE_OFF)
    , m_dExposureEdit(0)
    , m_dGainEdit(0)
    , m_dFrameRateEdit(0)
    , m_bSoftWareTriggerCheck(FALSE)
    , m_nSaveImageType(MV_Image_Undefined)
    , m_nTriggerSource(MV_TRIGGER_SOURCE_SOFTWARE)
    , m_pBufForSaveImage(NULL)
    , m_nBufSizeForSaveImage(0)
    , m_pBufForDriver(NULL)
    , m_nBufSizeForDriver(0)
	, m_Result(_T(""))
{
	m_writeResult = _T("");
	m_readTs = _T("");
	m_ReadQulitay = _T("");

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pThis = this;
}

void CBasicDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
	DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
	DDX_Text(pDX, IDC_EXPOSURE_EDIT, m_dExposureEdit);
	DDX_Text(pDX, IDC_GAIN_EDIT, m_dGainEdit);
	DDX_Text(pDX, IDC_FRAME_RATE_EDIT, m_dFrameRateEdit);
	DDX_Check(pDX, IDC_SOFTWARE_TRIGGER_CHECK, m_bSoftWareTriggerCheck);
	DDX_Text(pDX, IDC_EDIT4, m_WriteValue);
	DDX_Text(pDX, IDC_EDIT2, m_writeResult);
	DDX_Text(pDX, IDC_EDIT3, m_readTs);
	DDX_Text(pDX, IDC_EDIT5, m_ReadQulitay);
	DDX_Text(pDX, IDC_EDIT1, m_readValue[0]);
	DDX_Text(pDX, IDC_EDIT8, m_readValue[1]);
	DDX_Text(pDX, IDC_EDIT9, m_readValue[2]);
	DDX_Text(pDX, IDC_EDIT10, m_readValue[3]);
	DDX_Text(pDX, IDC_EDIT11, m_readValue[4]);
	//DDX_Control(pDX, IDC_STATIC, m_Icon);
	DDX_Control(pDX, IDC_SLIDER1, m_slider_1);
	DDX_Text(pDX, IDC_EDIT7, m_Result);
	DDX_Control(pDX, IDC_COMBO2, n_cobDemo);
}

BEGIN_MESSAGE_MAP(CBasicDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// }}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_ENUM_BUTTON, &CBasicDemoDlg::OnBnClickedEnumButton)
    ON_BN_CLICKED(IDC_OPEN_BUTTON, &CBasicDemoDlg::OnBnClickedOpenButton)
    ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CBasicDemoDlg::OnBnClickedCloseButton)
    ON_BN_CLICKED(IDC_CONTINUS_MODE_RADIO, &CBasicDemoDlg::OnBnClickedContinusModeRadio)
    ON_BN_CLICKED(IDC_TRIGGER_MODE_RADIO, &CBasicDemoDlg::OnBnClickedTriggerModeRadio)
    ON_BN_CLICKED(IDC_START_GRABBING_BUTTON, &CBasicDemoDlg::OnBnClickedStartGrabbingButton)
    ON_BN_CLICKED(IDC_STOP_GRABBING_BUTTON, &CBasicDemoDlg::OnBnClickedStopGrabbingButton)
    ON_BN_CLICKED(IDC_GET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedGetParameterButton)
    ON_BN_CLICKED(IDC_SET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedSetParameterButton)
    ON_BN_CLICKED(IDC_SOFTWARE_TRIGGER_CHECK, &CBasicDemoDlg::OnBnClickedSoftwareTriggerCheck)
    ON_BN_CLICKED(IDC_SOFTWARE_ONCE_BUTTON, &CBasicDemoDlg::OnBnClickedSoftwareOnceButton)
    ON_BN_CLICKED(IDC_SAVE_BMP_BUTTON, &CBasicDemoDlg::OnBnClickedSaveBmpButton)
    ON_BN_CLICKED(IDC_SAVE_JPG_BUTTON, &CBasicDemoDlg::OnBnClickedSaveJpgButton)
    ON_WM_CLOSE()
	
	//ON_BN_CLICKED(IDC_BUTTON2, &CBasicDemoDlg::OnRead)
	//ON_BN_CLICKED(IDC_BUTTON3, &CBasicDemoDlg::OnWrite)

	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CBasicDemoDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CBasicDemoDlg message handlers

BOOL CBasicDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CBasicDemoDlg::OnButton1();
	//CBasicDemoDlg::OnRead();

	SetTimer(ETimerId_1, 0, NULL);//����OPCͬ����ȡ��ʱ��
	//�������ؼ���ʼ������
	m_slider_1.SetRange(0, 255);//���û�����Χ
	m_slider_1.SetTicFreq(15);//ÿ1����λ��һ�̶�
	m_slider_1.SetPos(m_int_1);
	CString Gray_threshold_1;
	Gray_threshold_1.Format(_T("%d"), m_int_1);
	SetDlgItemText(IDC_SLIDER1, Gray_threshold_1);
	SetDlgItemText(IDC_EDIT6, Gray_threshold_1);

	for (int i = 0; i < ITEM_NUMBER; i++)
	{
		
		n_cobDemo.AddString(szItemID[i]);
		n_cobDemo.SetCurSel(0);
	}

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

	DisplayWindowInitial();     // ch:��ʾ���ʼ�� | en:Display Window Initialization

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBasicDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBasicDemoDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBasicDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ch:��ťʹ�� | en:Enable control
int CBasicDemoDlg::EnableControls(BOOL bIsCameraReady)
{
    GetDlgItem(IDC_OPEN_BUTTON)->EnableWindow(m_bOpenDevice ? FALSE : (bIsCameraReady ? TRUE : FALSE));
    GetDlgItem(IDC_CLOSE_BUTTON)->EnableWindow((m_bOpenDevice && bIsCameraReady) ? TRUE : FALSE);
    GetDlgItem(IDC_START_GRABBING_BUTTON)->EnableWindow((m_bStartGrabbing && bIsCameraReady) ? FALSE : (m_bOpenDevice ? TRUE : FALSE));
    GetDlgItem(IDC_STOP_GRABBING_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SOFTWARE_TRIGGER_CHECK)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow((m_bStartGrabbing && m_bSoftWareTriggerCheck)? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_BMP_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_JPG_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_EXPOSURE_EDIT)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_GAIN_EDIT)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_FRAME_RATE_EDIT)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_GET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_SET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_CONTINUS_MODE_RADIO)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_TRIGGER_MODE_RADIO)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);

    return MV_OK;
}

// ch:�ʼʱ�Ĵ��ڳ�ʼ�� | en:Initial window initialization
int CBasicDemoDlg::DisplayWindowInitial(void)
{
    CWnd *pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
    if (NULL == pWnd)
    {
        return STATUS_ERROR;
    }
    m_hwndDisplay = pWnd->GetSafeHwnd();
    if (NULL == m_hwndDisplay)
    {
        return STATUS_ERROR;
    }
    EnableControls(FALSE);

    return MV_OK;
}

// ch:��ʾ������Ϣ | en:Show error message
void CBasicDemoDlg::ShowErrorMsg(CString csMessage, int nErrorNum)
{
    CString errorMsg;
    if (nErrorNum == 0)
    {
        errorMsg.Format(_T("%s"), csMessage);
    }
    else
    {
        errorMsg.Format(_T("%s: Error = %x: "), csMessage, nErrorNum);
    }

    switch(nErrorNum)
    {
    case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
    case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
    case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
    case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
    case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
    case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
    case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
    case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
    case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
    case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
    case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
    case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
    case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
    case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
    case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    }

    MessageBox(errorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
}

// ch:���豸 | en:Open Device
int CBasicDemoDlg::OpenDevice(void)
{
    if (TRUE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }
    UpdateData(TRUE);
    if(TRUE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }
    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowErrorMsg(TEXT("Please select device"), 0);
        return STATUS_ERROR;
    }

    // ch:���豸��Ϣ�����豸ʵ�� | en:Device instance created by device information
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        ShowErrorMsg(TEXT("Device does not exist"), 0);
        return STATUS_ERROR;
    }

    if (NULL != m_pcMyCamera)
    {
        return STATUS_ERROR;
    }

    m_pcMyCamera = new CMyCamera;
    if (NULL == m_pcMyCamera)
    {
        return STATUS_ERROR;
    }

    int nRet = m_pcMyCamera->Open(m_stDevList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        return nRet;
    }

    // ch:̽��������Ѱ���С(ֻ��GigE�����Ч) | en:Detection network optimal package size(It only works for the GigE camera)
    if (m_stDevList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
    {
        int nPacketSize = m_pcMyCamera->GetOptimalPacketSize();
        if (nPacketSize > 0)
        {
            nRet = m_pcMyCamera->SetIntValue("GevSCPSPacketSize",nPacketSize);
            if(nRet != MV_OK)
            {
                ShowErrorMsg(TEXT("Warning: Set Packet Size fail!"), nRet);
            }
        }
        else
        {
            ShowErrorMsg(TEXT("Warning: Get Packet Size fail!"), nPacketSize);
        }
    }

    m_bOpenDevice = TRUE;

    return MV_OK;
}

// ch:�ر��豸 | en:Close Device
int CBasicDemoDlg::CloseDevice(void)
{   
    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    m_bOpenDevice = FALSE;
    m_bStartGrabbing = FALSE;

    if (m_pBufForDriver)
    {
        free(m_pBufForDriver);
        m_pBufForDriver = NULL;
    }
    m_nBufSizeForDriver = 0;

    if (m_pBufForSaveImage)
    {
        free(m_pBufForSaveImage);
        m_pBufForSaveImage = NULL;
    }
    m_nBufSizeForSaveImage  = 0;

    return MV_OK;
}

// ch:��ȡ����ģʽ | en:Get Trigger Mode
int CBasicDemoDlg::GetTriggerMode(void)
{
    CString strFeature;
    unsigned int nEnumValue = 0;

    int nRet = m_pcMyCamera->GetEnumValue("TriggerMode", &nEnumValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    m_nTriggerMode = nEnumValue;
    if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode)
    {
        OnBnClickedTriggerModeRadio();
    }
    else if (MV_TRIGGER_MODE_OFF == m_nTriggerMode)
    {
        OnBnClickedContinusModeRadio();
    }
    else
    {
        return STATUS_ERROR;
    }

    return MV_OK;
}

// ch:���ô���ģʽ | en:Set Trigger Mode
int CBasicDemoDlg::SetTriggerMode(void)
{
    int nRet = m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    return MV_OK;
}

// ch:��ȡ�ع�ʱ�� | en:Get Exposure Time
int CBasicDemoDlg::GetExposureTime(void)
{
    float  fFloatValue = 0.0;
    int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &fFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    m_dExposureEdit = fFloatValue;

    return MV_OK;
}

// ch:�����ع�ʱ�� | en:Set Exposure Time
int CBasicDemoDlg::SetExposureTime(void)
{
    // ch:�����������ع�ģʽ���������ع�ʱ����Ч
    // en:Adjust these two exposure mode to allow exposure time effective
    int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

    nRet = m_pcMyCamera->SetFloatValue("ExposureTime", m_dExposureEdit);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    return MV_OK;
}

// ch:��ȡ���� | en:Get Gain
int CBasicDemoDlg::GetGain(void)
{
    float  fFloatValue = 0.0;

    int nRet = m_pcMyCamera->GetFloatValue("Gain", &fFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    m_dGainEdit = (int)fFloatValue;

    return MV_OK;
}

// ch:�������� | en:Set Gain
int CBasicDemoDlg::SetGain(void)
{
    // ch:��������ǰ�Ȱ��Զ�����رգ�ʧ�����践��
    //en:Set Gain after Auto Gain is turned off, this failure does not need to return
    int nRet = m_pcMyCamera->SetEnumValue("GainAuto", 0);

    return m_pcMyCamera->SetFloatValue("Gain", m_dGainEdit);
}

// ch:��ȡ֡�� | en:Get Frame Rate
int CBasicDemoDlg::GetFrameRate(void)
{

    float  fFloatValue = 0.0;

    int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &fFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    m_dFrameRateEdit = fFloatValue;

    return MV_OK;
}

// ch:����֡�� | en:Set Frame Rate
int CBasicDemoDlg::SetFrameRate(void)
{
    int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    return m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", m_dFrameRateEdit);
}

// ch:��ȡ����Դ | en:Get Trigger Source
int CBasicDemoDlg::GetTriggerSource(void)
{
    unsigned int nEnumValue = 0;

    int nRet = m_pcMyCamera->GetEnumValue("TriggerSource", &nEnumValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    if ((unsigned int)MV_TRIGGER_SOURCE_SOFTWARE == nEnumValue)
    {
        m_bSoftWareTriggerCheck = TRUE;
    }
    else
    {
        m_bSoftWareTriggerCheck = FALSE;
    }

    return MV_OK;
}

// ch:���ô���Դ | en:Set Trigger Source
int CBasicDemoDlg::SetTriggerSource(void)
{
    if (m_bSoftWareTriggerCheck)//������ťѡ�У������������ģʽΪ����
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
        int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            ShowErrorMsg(TEXT("Set Software Trigger Fail"), nRet);
            return nRet;
        }
        GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(TRUE);//���õ��δ�������
    }
    else//����ѡ��ΪӲ���ⲿ����������Ϊ���I/O����LINE0Ϊ����Դ
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_LINE0;
        int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            ShowErrorMsg(TEXT("Set Hardware Trigger Fail"), nRet);
            return nRet;
        }
        GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(FALSE);//���õ��δ�������
    }

    return MV_OK;
}

// ch:����ͼƬ | en:Save Image
int CBasicDemoDlg::SaveImage()
{
    if (FALSE == m_bStartGrabbing)
    {
        return STATUS_ERROR;
    }
    // ch:��ȡ1��ͼ | en:get one image
    unsigned int nRecvBufSize = 0;
    int nRet = MV_OK;

    // ch:���ڵ�һ�α���ͼ��ʱ���뻺�棬�� CloseDevice ʱ�ͷ�
    // en:Request buffer only when save image for first time, release after CloseDevice
    if (NULL == m_pBufForDriver)
    {
        // ch:������л�ȡһ֡ͼ���С | en:Get size of one frame from camera
        nRet = m_pcMyCamera->GetIntValue("PayloadSize", &nRecvBufSize);
        if (nRet != MV_OK)
        {
            ShowErrorMsg(TEXT("failed in get PayloadSize"), nRet);
            return nRet;
        }
        // ch:һ֡���ݴ�С
        // en:One frame size
        m_nBufSizeForDriver = nRecvBufSize;  
        m_pBufForDriver = (unsigned char *)malloc(m_nBufSizeForDriver);
        if (NULL == m_pBufForDriver)
        {
            ShowErrorMsg(TEXT("malloc m_pBufForDriver failed, run out of memory"), 0);
            return nRet;
        }
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    unsigned int nDataSize = nRecvBufSize;
    unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;

    while(nImageNum)
    {
        nRet = m_pcMyCamera->GetOneFrameTimeout(m_pBufForDriver, &nDataLen, m_nBufSizeForDriver, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            nImageNum--;

            // ch:���ڵ�һ�α���ͼ��ʱ���뻺�棬�� CloseDevice ʱ�ͷ�
            // en:Request buffer only when save image for first time, release after CloseDevice
            if (NULL == m_pBufForSaveImage)
            {
                // ch:BMPͼƬ��С��width * height * 3 + 2048(Ԥ��BMPͷ��С)
                // en:// BMP image size: width * height * 3 + 2048 (Reserved BMP header size)
                m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;

                m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);
                if (NULL == m_pBufForSaveImage)
                {
                    break;
                }
            }
            // ch:���ö�Ӧ��������� | en:Set camera parameter
            MV_SAVE_IMAGE_PARAM_EX stParam = {0};
            stParam.enImageType = m_nSaveImageType; // ch:��Ҫ�����ͼ������ | en:Image format to save
            stParam.enPixelType = stImageInfo.enPixelType;  // ch:�����Ӧ�����ظ�ʽ | en:Camera pixel type
            stParam.nWidth      = stImageInfo.nWidth;         // ch:�����Ӧ�Ŀ� | en:Width
            stParam.nHeight     = stImageInfo.nHeight;          // ch:�����Ӧ�ĸ� | en:Height
            stParam.nDataLen    = stImageInfo.nFrameLen;
            stParam.pData       = m_pBufForDriver;
            stParam.pImageBuffer = m_pBufForSaveImage;
            stParam.nBufferSize = m_nBufSizeForSaveImage;  // ch:�洢�ڵ�Ĵ�С | en:Buffer node size
            stParam.nJpgQuality     = 80;       // ch:jpg���룬���ڱ���Jpgͼ��ʱ��Ч������BMPʱSDK�ں��Ըò���
                                                // en:jpg encoding, only valid when saving as Jpg. SDK ignore this parameter when saving as BMP

            nRet = m_pcMyCamera->SaveImage(&stParam);
            if(MV_OK != nRet)
            {
                break;
            }

            char chImageName[IMAGE_NAME_LEN] = {0};
            if (MV_Image_Bmp == stParam.enImageType)
            {
                sprintf_s(chImageName, IMAGE_NAME_LEN, "Image_w%d_h%d_fn%03d.bmp", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
            }
            else if (MV_Image_Jpeg == stParam.enImageType)
            {
                sprintf_s(chImageName, IMAGE_NAME_LEN, "Image_w%d_h%d_fn%03d.jpg", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
            }
            
            FILE* fp = fopen(chImageName, "wb");
            if (NULL == fp)
            {
                ShowErrorMsg(TEXT("write image failed, maybe you have no privilege"), 0);
                return STATUS_ERROR;
            }
            fwrite(m_pBufForSaveImage, 1, stParam.nImageLen, fp);
            fclose(fp);
        }
        else
        {
            break;
        }
    }

    return nRet;
}


// ch:���²����豸��ť:ö�� | en:Click Find Device button:Enumeration 
void CBasicDemoDlg::OnBnClickedEnumButton()
{
    CString strMsg;

    // ch:����豸�б���е���Ϣ | en:Clear Device List Information
    m_ctrlDeviceCombo.ResetContent();

    // ch:��ʼ���豸��Ϣ�б� | en:Device Information List Initialization
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:ö�������������豸 | en:Enumerate all devices within subnet
    int nRet = CMyCamera::EnumDevices(&m_stDevList);
    if (MV_OK != nRet)
    {
        return;
    }

    // ch:��ֵ���뵽��Ϣ�б���в���ʾ���� | en:Add value to the information list box and display
    unsigned int i;
    int nIp1, nIp2, nIp3, nIp4;
    for (i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
            continue;
        }
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

            wchar_t* pUserName = NULL;
            if (strcmp("", (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
            {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else
            {
                char strUserName[256] = {0};
                sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, 
                pUserName, nIp1, nIp2, nIp3, nIp4);
            if (NULL != pUserName)
            {
                delete(pUserName);
                pUserName = NULL;
            }

        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            wchar_t* pUserName = NULL;

            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
            {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else
            {
                char strUserName[256];
                sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]UsbV3:  %s"), i, pUserName);
            if (NULL != pUserName)
            {
                delete(pUserName);
                pUserName = NULL;
            }

        }
        else
        {
            ShowErrorMsg(TEXT("Unknown device enumerated"), 0);;
        }
        m_ctrlDeviceCombo.AddString(strMsg);
    }

    if (0 == m_stDevList.nDeviceNum)
    {
        ShowErrorMsg(TEXT("No device"), 0);
        return;
    }
    m_ctrlDeviceCombo.SetCurSel(0);

    EnableControls(TRUE);

    return;
}

// ch:���´��豸��ť�����豸 | en:Click Open button: Open Device
void CBasicDemoDlg::OnBnClickedOpenButton()
{
    int nRet = OpenDevice();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Open Fail"), nRet);
        return;
    }

    OnBnClickedGetParameterButton(); // ch:��ȡ���� | en:Get Parameter
    EnableControls(TRUE);

    return;
}

// ch:���¹ر��豸��ť���ر��豸 | en:Click Close button: Close Device
void CBasicDemoDlg::OnBnClickedCloseButton()
{
    CloseDevice();
    EnableControls(TRUE);

    return;
}

// ch:��������ģʽ��ť | en:Click Continues button
void CBasicDemoDlg::OnBnClickedContinusModeRadio()
{
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(FALSE);
    m_nTriggerMode = MV_TRIGGER_MODE_OFF;
    int nRet = SetTriggerMode();//���ô���ģʽΪ����ģʽ
    if (MV_OK != nRet)
    {
        return;
    }
    GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow(FALSE);

    return;
}

// ch:���´���ģʽ��ť | en:Click Trigger Mode button
void CBasicDemoDlg::OnBnClickedTriggerModeRadio()
{
    UpdateData(TRUE);
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(FALSE);//���ò�ѡ������ģʽ
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(TRUE);//����ѡ�д���ģʽ
    m_nTriggerMode = MV_TRIGGER_MODE_ON;
    int nRet = SetTriggerMode();//���ô���ģʽΪ����ģʽ
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set Trigger Mode Fail"), nRet);
        return;
    }

    if (m_bStartGrabbing == TRUE)
    {
        if (TRUE == m_bSoftWareTriggerCheck)
        {
            GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(TRUE);//���õ��δ�������
        }
    }

    return;
}

// ch:���¿�ʼ�ɼ���ť | en:Click Start button
void CBasicDemoDlg::OnBnClickedStartGrabbingButton()
{
    if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing)
    {
        return;
    }

    int nRet = MV_OK;
	//ע�����ݻص�����
	nRet = m_pcMyCamera->RegisterImageCallBack(ImageCallBack, NULL);
	if (MV_OK != nRet)
	{
		printf("error: RegisterImageCallBack fail [%x]\n", nRet);
		return;
	}

    if (NULL != m_pcMyCamera)
    {
        nRet = m_pcMyCamera->StartGrabbing();
        if (nRet == MV_OK)
        {
            nRet = m_pcMyCamera->Display(m_hwndDisplay);
        }
    }
    else
    {
        return ;
    }

    if (MV_OK != nRet)
    {
        return ;
    }
    m_bStartGrabbing = TRUE;
    EnableControls(TRUE);

	//SetTimer(ETimerId_1, 0, NULL);//����OPCͬ����ȡ��ʱ��
	//SetTimer(ETimerId_2, 0, NULL);//����OPCͬ����ȡ��ʱ��

    return;
}

// ch:���½����ɼ���ť | en:Click Stop button
void CBasicDemoDlg::OnBnClickedStopGrabbingButton()
{
    if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing)
    {
        return;
    }

    int nRet = MV_OK;
    if (NULL != m_pcMyCamera)
    {
        nRet = m_pcMyCamera->StopGrabbing();
    }
    else
    {
        return ;
    }

    if (MV_OK != nRet)
    {
        return ;
    }
    m_bStartGrabbing = FALSE;
    EnableControls(TRUE);

    return;
}

// ch:���»�ȡ������ť | en:Click Get Parameter button
void CBasicDemoDlg::OnBnClickedGetParameterButton()
{
    int nRet = GetTriggerMode();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Trigger Mode Fail"), nRet);
    }

    nRet = GetExposureTime();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Exposure Time Fail"), nRet);
    }

    nRet = GetGain();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Gain Fail"), nRet);
    }

    nRet = GetFrameRate();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Frame Rate Fail"), nRet);
    }

    nRet = GetTriggerSource();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Trigger Source Fail"), nRet);
    }

    UpdateData(FALSE);
    return;
}

// ch:�������ò�����ť | en:Click Set Parameter button
void CBasicDemoDlg::OnBnClickedSetParameterButton()
{
    UpdateData(TRUE);

    bool bIsSetSucceed = true;
    int nRet = SetExposureTime();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Exposure Time Fail"), nRet);
    }
    nRet = SetGain();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Gain Fail"), nRet);
    }
    nRet = SetFrameRate();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Frame Rate Fail"), nRet);
    }
    
    if (true == bIsSetSucceed)
    {
        ShowErrorMsg(TEXT("Set Parameter Succeed"), nRet);
    }

    return;
}

// ch:����������ť | en:Click Software button
void CBasicDemoDlg::OnBnClickedSoftwareTriggerCheck()
{
    UpdateData(TRUE);

    int nRet = SetTriggerSource();
    if (nRet != MV_OK)
    {
        return;
    }
    return;
}

// ch:���µ��δ�����ť | en:Click Execute button
void CBasicDemoDlg::OnBnClickedSoftwareOnceButton()
{
	
    if (TRUE != m_bStartGrabbing)
    {
        return;
    }

    int nRet = m_pcMyCamera->CommandExecute("TriggerSoftware");
    return;
}

// ch:���±���bmpͼƬ��ť | en:Click Save BMP button
void CBasicDemoDlg::OnBnClickedSaveBmpButton()
{
    m_nSaveImageType = MV_Image_Bmp;
    int nRet = SaveImage();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Save bmp fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save bmp succeed"), nRet);

    return;
}

// ch:���±���jpgͼƬ��ť | en:Click Save JPG button
void CBasicDemoDlg::OnBnClickedSaveJpgButton()
{
    m_nSaveImageType = MV_Image_Jpeg;
    int nRet = SaveImage();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Save jpg fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save jpg succeed"), nRet);

    return;
}

// ch:���Ͻ��˳� | en:Exit from upper right corner
void CBasicDemoDlg::OnClose()
{
    PostQuitMessage(0);
    CloseDevice();
    CDialog::OnClose();
}


BOOL CBasicDemoDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
    {
        // ch:�����Ϣ�Ǽ��̰����¼�������Esc����ִ�����´��루ʲô��������������Լ������Ҫ�Ĵ��룩
        // en:If the message is a keyboard press event and a Esc key, execute the following code (nothing is done, you can add the code you want)
        return TRUE;
    }
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        // ch:�����Ϣ�Ǽ��̰����¼�������Entert����ִ�����´��루ʲô��������������Լ������Ҫ�Ĵ��룩
        // en:If the message is a keyboard press event and a Esc key, execute the following code (nothing is done, you can add the code you want)
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

//ͼ�����ݻص�����
void CBasicDemoDlg::ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
	CBasicDemoDlg *pDlg = (CBasicDemoDlg*) pThis;
	if (pFrameInfo)   
	{
		img = cv::Mat::zeros(cv::Size(pFrameInfo->nWidth,pFrameInfo->nHeight), CV_8U);
		memcpy(img.data, pData, pFrameInfo->nWidth*pFrameInfo->nHeight);
		dst = Mat::zeros(img.size(), CV_8UC1);
		//��������
		Mat canny_output;
		vector<vector<Point>> contours;
		vector<Vec4i> hierachy;
		Canny(img, canny_output, m_int_1, m_int_1*3, 3, false);
		findContours(canny_output, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		RNG rng(12345);
		for (size_t i = 0; i < contours.size(); i++) {
			//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(dst, contours, i, (255,255,255), 2, 8, hierachy, 0, Point(0, 0));
		}
		int i, j;
		int	number_left=0, number_right=0;
		int	direction_flag = 0;
		int width = dst.cols;
		int height = dst.rows;
		int pixel[CAMERA_WIDTH];//ÿ�к�ɫ���ظ���
		double ratio;
		Mat vertical(height, width, CV_8UC1);//������ֱͶӰͼ
		vector<Mat> ch;
		memset(pixel, 0, sizeof(int) * width);
		for (i = pThis->Left_Offset; i < (width- pThis->Right_Offset); i++)//����ÿ�����ص�
		{
			for (j = pThis->Upper_Offset; j < height- pThis->Lower_Offset; j++)
			{
				if (dst.at<uchar>(j, i) == 255)//����Ǻڵװ���
				{
					pixel[i]= pixel[i]+5;
					if (pixel[i]>(CAMERA_WIDTH-1))
					{
						pixel[i] = CAMERA_WIDTH;
					}
				}
			}
		}
		for (i = pThis->Left_Offset; i < width/2 ; i++)//�������ÿ�����ص�
		{
			for (j = pThis->Upper_Offset; j < height - pThis->Lower_Offset; j++)
			{
				if (dst.at<uchar>(j, i) == 255)
				{
					number_left++;
				}
			}
		}
		for (i = width / 2; i < (width - pThis->Right_Offset); i++)//�����Ҳ�ÿ�����ص�
		{
			for (j = pThis->Upper_Offset; j < height - pThis->Lower_Offset; j++)
			{
				if (dst.at<uchar>(j, i) == 255)
				{
					number_right++;
				}
			}
		}
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				vertical.at<uchar>(i, j) = 255;//���ñ���Ϊ��ɫ
			}
		}
		for (i = 0; i < width; i++)//��ֱͶӰֱ��ͼ
		{
			for (j = 0; j < pixel[i]; j++)
			{
				vertical.at<uchar>(height - 1 - j, i) = 0;//����ֱ��ͼΪ��ɫ
			}
		}
		if ((number_left-number_right)>4000)
		{
			direction_flag = 0;
			putText(dst, "LEFT", Point(1200, 400), FONT_HERSHEY_COMPLEX, 4.0, Scalar(255, 255, 255), 2, LINE_8);
			pThis->ShowResult(1);
		}
		else if ((number_right - number_left) > 4000)
		{
			direction_flag = 1;
			putText(dst, "RIGHT", Point(1200, 400), FONT_HERSHEY_COMPLEX, 4.0, Scalar(255, 255, 255), 2, LINE_8);
			pThis->ShowResult(2);
		}
		else
		{
			direction_flag = 3;
			putText(dst, "NONE", Point(1200, 400), FONT_HERSHEY_COMPLEX, 4.0, Scalar(255, 255, 255), 2, LINE_8);
			pThis->ShowResult(3);
		}
		number++;
		putText(img, "NO.", Point(50, 100), FONT_HERSHEY_COMPLEX, 2.0, Scalar(255, 255, 255), 2, LINE_8);
		putText(img, std::to_string(number), Point(170, 100), FONT_HERSHEY_COMPLEX, 2.0, Scalar(255, 255, 255), 2, LINE_8);
		putText(dst, "NO.", Point(50, 100), FONT_HERSHEY_COMPLEX, 2.0, Scalar(255, 255, 255), 2, LINE_8);
		putText(dst, std::to_string(number), Point(170, 100), FONT_HERSHEY_COMPLEX, 2.0, Scalar(255, 255, 255), 2, LINE_8);
		putText(vertical, "NO.", Point(50, 100), FONT_HERSHEY_COMPLEX, 2.0, Scalar(255, 255, 255), 2, LINE_8);
		putText(vertical, std::to_string(number), Point(170, 100), FONT_HERSHEY_COMPLEX, 2.0, Scalar(255, 255, 255), 2, LINE_8);
		memcpy(pData, img.data, pFrameInfo->nWidth*pFrameInfo->nHeight);
		src = vertical.clone();
		pThis->DrawMat(dst, IDC_STATIC_PROCESSING, true);
		pThis->DrawMat(src, IDC_STATIC_HISTOGRAM, true);
	}
}

//��������ͼ�񴰿�
void CBasicDemoDlg::DrawMat(cv::Mat & img, UINT nControlID, bool bRoomToControlSize)
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nControlID)->GetClientRect(&rect);  // ��ȡ�ؼ���С
	if (bRoomToControlSize)
	{

		cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// ��С��Ŵ�Mat������
	}
	else
	{
		img.copyTo(imgTmp);
	}

	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, COLOR_GRAY2BGRA); // GRAY��ͨ��
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, COLOR_BGR2BGRA);  // BGR��ͨ��
		break;
	default:
		break;
	}

	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1); // ����һ�����ض��ٸ��ֽ�
															 // ����bitmapinfo(����ͷ)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap����ͷ -> MFC
	CDC *pDC = GetDlgItem(nControlID)->GetDC();
	pDC->SetStretchBltMode(COLORONCOLOR);
	if (bRoomToControlSize)
	{
		::StretchDIBits(pDC->GetSafeHdc()
			, 0, 0, rect.Width(), rect.Height()
			, 0, 0, imgTmp.cols, imgTmp.rows,
			imgTmp.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	else
	{
		int minWidth = min(imgTmp.cols, rect.Width());
		int minHeight = min(imgTmp.rows, rect.Height());
		::StretchDIBits(
			pDC->GetSafeHdc(),
			0, 0, minWidth, minHeight,
			0, 0, minWidth, minHeight,
			imgTmp.data,
			&bitInfo,
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}
	ReleaseDC(pDC);
}

//�����ʾ����
void CBasicDemoDlg::ShowResult(int result)
{
	cv::Mat m_dst;
	cv::Mat srcImage;
	if (result==1)
	{
		srcImage = cv::imread("F:/C++ѧϰ����/VS - 0817/BasicDemo/OK.jpg");
		m_Result.Format(_T("%s"), _T("���"));
		SetDlgItemText(IDC_EDIT7, m_Result);
	}
	else if (result == 2)
	{
		srcImage = cv::imread("F:/C++ѧϰ����/VS - 0817/BasicDemo/OK.jpg");
		m_Result.Format(_T("%s"), _T("�Ҳ�"));
		SetDlgItemText(IDC_EDIT7, m_Result);
	}
	else if (result == 3)
	{
		srcImage = cv::imread("F:/C++ѧϰ����/VS - 0817/BasicDemo/NG.jpg");
		m_Result.Format(_T("%s"), _T("��"));
		SetDlgItemText(IDC_EDIT7, m_Result);
	}
	else if (result == 4)
	{
		srcImage = cv::imread("F:/C++ѧϰ����/VS - 0817/BasicDemo/GreenLight.jpg");
	}
	else if (result == 5)
	{
		srcImage = cv::imread("F:/C++ѧϰ����/VS - 0817/BasicDemo/GreyLight.jpg");
	}
	else
	{
		srcImage = cv::imread("F:/C++ѧϰ����/VS - 0817/BasicDemo/WAIT.jpg");
	}
	

	CRect rect;
	CStatic* pWnd;
	if (result == 5||4)
	{
		pWnd = (CStatic*)GetDlgItem(IDC_STATIC_OPC);
	}
	else
	{ 
		pWnd = (CStatic*)GetDlgItem(IDC_STATIC_RESULT);
	}
	
	pWnd->GetClientRect(&rect);
	cv::resize(srcImage, m_dst, cv::Size(rect.Width(), rect.Height()));
	cv::flip(m_dst, m_dst, 0);//��Ϊ�Ƿ��ŵ�ͼ���������½�Ϊԭ�����ʾ��������ʾ��ͼ��ԭ�������Ͻǣ��൱����X�ᷭ

	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 24;
	bitInfo.bmiHeader.biWidth = rect.Width();
	bitInfo.bmiHeader.biHeight = rect.Height();
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC * pDc = pWnd->GetWindowDC();
	StretchDIBits(pDc->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, rect.Width(), rect.Height(), m_dst.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(pDc);

}

/*******************************OPCͨѶʵ�ֳ���***********************************/
//�ؼ�--����������
void CBasicDemoDlg::OnButton1()
{
	HRESULT    hr;
	CLSID                clsid;
	LONG            TimeBias = 0;
	FLOAT  PercentDeadband = 0.0;
	DWORD      AA = 0;
	CString        szErrorText;



	// Make sure COM is initialized:
	hr = CoInitialize(NULL);

	if (hr != S_OK)
	{
		if (hr == S_FALSE)
		{
			AfxMessageBox(_T("com���Ѿ���ʼ��"));
		}
		else
		{
			
			AfxMessageBox(_T("com���Ѿ���ʼ��"));
			return;
		}
	}

	hr = CLSIDFromProgID(L"KEPware.KEPServerEx.V5", &clsid);
	if (hr != S_OK)
	{
		AfxMessageBox(_T("��ȡCLSIDʧ��"));
		CoUninitialize();
		return;
	}

	hr = CoCreateInstance(clsid,
		NULL,
		CLSCTX_SERVER,
		IID_IOPCServer,
		(void **)&m_IOPCServer);

	// If succeeded, enumerate registered components:
	if (hr != S_OK)
	{
		AfxMessageBox(_T("����OPC������ʧ��"));
		m_IOPCServer = NULL;
		CoUninitialize();
		return;
	}
	hr = m_IOPCServer->AddGroup(L"grp1",
		TRUE,
		500,
		1,
		&TimeBias,
		&PercentDeadband,
		LOCALE_ID,
		&m_GrpSrvHandle,
		&AA,
		IID_IOPCItemMgt,
		(LPUNKNOWN*)&m_IOPCItemMgt);
	if (hr == OPC_S_UNSUPPORTEDRATE)
	{
		AfxMessageBox(_T(" �����ˢ������ʵ�ʵ�ˢ�����ʲ�һ��"));
	}
	else
		if (FAILED(hr))
		{
			AfxMessageBox(_T("����λ���������GREAP ����"));
			m_IOPCServer->Release();
			m_IOPCServer = NULL;
			CoUninitialize();
			return;
		}
	for (int i = 0; i < ITEM_NUMBER; i++)
	{
		m_Items[i].szAccessPath = L"";
		m_Items[i].szItemID = szItemID[i];
		m_Items[i].bActive = TRUE;
		m_Items[i].hClient = 1;
		m_Items[i].dwBlobSize = 0;
		m_Items[i].pBlob = NULL;
		m_Items[i].vtRequestedDataType = 0;
	}
	hr = m_IOPCItemMgt->AddItems(ITEM_NUMBER,
		m_Items,
		&m_ItemResult,
		&m_pErrors);
	if ((hr != S_OK) && (hr != S_FALSE))
	{
		AfxMessageBox(_T("AddItemsʧ��"));
		m_IOPCItemMgt->Release();
		m_IOPCItemMgt = NULL;
		m_GrpSrvHandle = NULL;
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
		CoUninitialize();
		return;
	}
	else if (hr == S_OK)
	{
		AfxMessageBox(_T("AddItems�ɹ�"));
	}
	for (int i = 0; i < ITEM_NUMBER; i++)
	{
		if (m_ItemResult[i].dwAccessRights != (OPC_READABLE + OPC_WRITEABLE))
		{
			AfxMessageBox(_T("Item���ɶ���Ҳ����д���������������"));
		}
	}
	
	hr = m_IOPCItemMgt->QueryInterface(IID_IOPCSyncIO, (void **)&m_IOPCSyncIO);
	if (hr < 0)
	{
		AfxMessageBox( _T("IOPCSyncIOû�з��֣�����Ĳ�ѯ��"));
		CoTaskMemFree(m_ItemResult);
		m_IOPCItemMgt->Release();
		m_IOPCItemMgt = NULL;
		m_GrpSrvHandle = NULL;
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
		CoUninitialize();
		return;
	}
	
}

//�ؼ�--ͬ����
void CBasicDemoDlg::OnRead()
{
	// TODO: Add your control notification handler code here
	OPCHANDLE     *phServer;
	OPCITEMSTATE  *pItemValue;
	HRESULT       *pErrors;
	HRESULT       r1;
	UINT          qnr;
	if (m_pErrors[0] != S_OK)
	{
		AfxMessageBox(_T("OPC Item�����ã�������ͬ�������ܣ�"));
		return;
	}
	phServer = new OPCHANDLE[ITEM_NUMBER];
	for (int i = 0; i < ITEM_NUMBER; i++)
	{
		phServer[i] = m_ItemResult[i].hServer;
	}
	
	r1 = m_IOPCSyncIO->Read(OPC_DS_CACHE, ITEM_NUMBER, phServer, &pItemValue, &pErrors);
	delete[] phServer;
	if (r1 == S_OK)
	{
		int j = 0;
		for (j = 0; j < 2; j++)
		{
			m_readValue[j] = pItemValue[j].vDataValue.intVal;
		}
		for (j = 2; j < ITEM_NUMBER; j++)
		{
			m_readValue_float[j-2] = pItemValue[j].vDataValue.fltVal;
		}
		qnr = pItemValue[0].wQuality;
		switch (qnr)
		{
		case OPC_QUALITY_BAD:    m_ReadQulitay = "BAD";
			break;
		case OPC_QUALITY_UNCERTAIN:    m_ReadQulitay = "UNCERTAIN";
			break;
		case OPC_QUALITY_GOOD:    m_ReadQulitay = "GOOD";
			break;
		case OPC_QUALITY_NOT_CONNECTED:    m_ReadQulitay = "NOT_CONNECTED";
			break;
		case OPC_QUALITY_DEVICE_FAILURE:    m_ReadQulitay = "DEVICE_FAILURE";
			break;
		case OPC_QUALITY_SENSOR_FAILURE:    m_ReadQulitay = "SENSOR_FAILURE";
			break;
		case OPC_QUALITY_LAST_KNOWN:    m_ReadQulitay = "LAST_KNOWN";
			break;
		case OPC_QUALITY_COMM_FAILURE:    m_ReadQulitay = "COMM_FAILURE";
			break;
		case OPC_QUALITY_OUT_OF_SERVICE:    m_ReadQulitay = "OUT_OF_SERVICE";
			break;
		case OPC_QUALITY_LAST_USABLE:    m_ReadQulitay = "LAST_USABLE";
			break;
		case OPC_QUALITY_SENSOR_CAL:    m_ReadQulitay = "SENSOR_CAL";
			break;
		case OPC_QUALITY_EGU_EXCEEDED:    m_ReadQulitay = "EGU_EXCEEDED";
			break;
		case OPC_QUALITY_SUB_NORMAL:    m_ReadQulitay = "SUB_NORMAL";
			break;
		case OPC_QUALITY_LOCAL_OVERRIDE:    m_ReadQulitay = "LOCAL_OVERRIDE";
			break;
		default:
			m_ReadQulitay = "UNKNOWN ERROR";
		}
		m_readTs = COleDateTime(pItemValue[0].ftTimeStamp).Format();

		for (int i = 0; i < ITEM_NUMBER; i++)//ʵ�ֶ�ȡ�ؼ�ֵˢ��
		{
			CString str;
			if (i < 2)
			{
				str.Format(_T("%u"), m_readValue[i]);
			}
			else
			{
				str.Format(_T("%0.3f"), m_readValue_float[i-2]);
			}
			switch (i)
			{
			case 0:SetDlgItemText(IDC_EDIT1, str); break;
			case 1:SetDlgItemText(IDC_EDIT8, str); break;
			case 2:SetDlgItemText(IDC_EDIT9, str); break;
			case 3:SetDlgItemText(IDC_EDIT10, str); break;
			case 4:SetDlgItemText(IDC_EDIT11, str); break;
			default:
				break;
			}

		}
		//UpdateData(FALSE);
	}
	if (r1 == S_FALSE)
	{
		AfxMessageBox(_T(" read()����"));
	}
	if (FAILED(r1))
	{
		AfxMessageBox(_T(" ͬ����ʧ�ܣ�"));
	}
	else
	{
		CoTaskMemFree(pItemValue);
	}
}

//�ؼ�--ͬ��д
void CBasicDemoDlg::OnWrite(UINT i)
{
	OPCHANDLE    *phServer;
	HRESULT      *pErrors;
	VARIANT      values[1];
	HRESULT       r1;
	LPWSTR        ErrorStr;
	CString       szOut;
	if (m_pErrors[0] != S_OK)
	{
		AfxMessageBox(_T("OPC Item�����ã�������ͬ�������ܣ�"));
		return;
	}

	phServer = new OPCHANDLE[1];

	//�ӶԻ���������

	switch (i)
	{
	case 0:phServer[0] = m_ItemResult[0].hServer; UpdateData(TRUE);  break;
	case 1:phServer[0] = m_ItemResult[1].hServer; UpdateData(TRUE);  break; 
	case 2:phServer[0] = m_ItemResult[2].hServer; UpdateData(TRUE);  break; 
	case 3:phServer[0] = m_ItemResult[3].hServer; UpdateData(TRUE);  break; 
	case 4:phServer[0] = m_ItemResult[4].hServer; UpdateData(TRUE);  break; 
		break;
	}
	values[0].vt = VT_R4; 
	values[0].fltVal = m_writeValue[i];
	r1 = m_IOPCSyncIO->Write(1, phServer, values, &pErrors);
	delete[] phServer;

	if (FAILED(r1))
	{
		AfxMessageBox(_T(" ͬ��дItemʧ�ܣ�"));
		
	}
	else
	{
		m_IOPCServer->GetErrorString(pErrors[0], LOCALE_ID, &ErrorStr);
		m_writeResult = ErrorStr;
		m_writeResult.Remove('\r');
		m_writeResult.Remove('\n');
		//UpdateData(FALSE);
		CoTaskMemFree(pErrors);
	}
}

//�ؼ�--�رշ�����
void CBasicDemoDlg::OnStop()
{
	HRESULT     r1;
	OPCHANDLE   *phServer;
	HRESULT     *pErrors;

	phServer = new OPCHANDLE[ITEM_NUMBER];
	for (int i = 0; i < ITEM_NUMBER; i++)
	{
		phServer[i] = m_ItemResult[i].hServer;
	}
	r1 = m_IOPCItemMgt->RemoveItems(ITEM_NUMBER, phServer, &pErrors);
	if ((r1 != S_OK) && (r1 != S_FALSE))
	{
		AfxMessageBox(_T("RemoveItemsʧ��"));
	}
	else if (r1 == S_OK)
	{
		AfxMessageBox(_T("RemoveItems�ɹ�"));
	}
	delete[] phServer;
	CoTaskMemFree(pErrors);
	CoTaskMemFree(m_ItemResult);
	m_ItemResult = NULL;

	m_IOPCSyncIO->Release();
	m_IOPCSyncIO = NULL;

	m_IOPCItemMgt->Release();
	m_IOPCItemMgt = NULL;
	r1 = m_IOPCServer->RemoveGroup(m_GrpSrvHandle, TRUE);
	if (r1 != S_OK)
	{

		AfxMessageBox(_T("RemoveGroupʧ��"));
	}
	else
	{
		AfxMessageBox(_T("RemoveGroup�ɹ�"));
	}
	m_GrpSrvHandle = NULL;

	m_IOPCServer->Release();
	m_IOPCServer = NULL;
	CoUninitialize();
}

//�ؼ�-ͬ��д�뵥��ָ��ITEM��ֵ
void CBasicDemoDlg::OnBnClickedButton3()
{
	OPCHANDLE    *phServer;
	HRESULT      *pErrors;
	VARIANT      values[1];
	HRESULT       r1;
	LPWSTR        ErrorStr;
	CString       szOut;
	CString strInput;
	
	m_writeValue[0] = 1;
	m_writeValue[1] = 2;
	m_writeValue_float[0] = 3.4;
	m_writeValue_float[1] = -5.6;
	m_writeValue_float[2] = 7.891;
	if (m_pErrors[0] != S_OK)
	{
		AfxMessageBox(_T("OPC Item�����ã�������ͬ�������ܣ�"));
		return;
	}
	phServer = new OPCHANDLE[1];
	for (int i = 0; i < ITEM_NUMBER; i++)
	{
		if (OPC_NUMBER[i]==1)
		{
			if (i<2)
			{
				phServer[0] = m_ItemResult[i].hServer;
				values[0].vt = VT_I4;
				values[0].intVal = m_writeValue[i];
				r1 = m_IOPCSyncIO->Write(1, phServer, values, &pErrors);
			}
			else
			{
				phServer[0] = m_ItemResult[i].hServer;
				values[0].vt = VT_R4;
				values[0].fltVal = m_writeValue_float[i-2];
				r1 = m_IOPCSyncIO->Write(1, phServer, values, &pErrors);
			}
		}
	}
	delete[] phServer;

	if (FAILED(r1))
	{
		AfxMessageBox(_T(" ͬ��дItemʧ�ܣ�"));

	}
	else
	{
		m_IOPCServer->GetErrorString(pErrors[0], LOCALE_ID, &ErrorStr);
		m_writeResult = ErrorStr;
		m_writeResult.Remove('\r');
		m_writeResult.Remove('\n');
		//UpdateData(FALSE);
		CoTaskMemFree(pErrors);
	}
	/*int nIndex = n_cobDemo.GetCurSel();
	if (GetDlgItemTextW(IDC_EDIT4, strInput) != 0)
	{
		if (-1 < nIndex)
		{
			//n_cobDemo.DeleteString(nIndex);
			if (nIndex < n_cobDemo.GetCount())
				n_cobDemo.SetCurSel(nIndex);
			else
				n_cobDemo.SetCurSel(0);

			if (m_pErrors[0] != S_OK)
			{
				AfxMessageBox(_T("OPC Item�����ã�������ͬ�������ܣ�"));
				return;
			}

			phServer = new OPCHANDLE[1];

			switch (nIndex)
			{
			case 0:phServer[0] = m_ItemResult[0].hServer; UpdateData(TRUE);  break;
			case 1:phServer[0] = m_ItemResult[1].hServer; UpdateData(TRUE);  break;
			case 2:phServer[0] = m_ItemResult[2].hServer; UpdateData(TRUE);  break;
			case 3:phServer[0] = m_ItemResult[3].hServer; UpdateData(TRUE);  break;
			}
			m_writeValue[nIndex] = m_WriteValue;
			values[0].vt = VT_R4;
			values[0].fltVal = m_writeValue[nIndex];
			r1 = m_IOPCSyncIO->Write(1, phServer, values, &pErrors);
			delete[] phServer;

			if (FAILED(r1))
			{
				AfxMessageBox(_T(" ͬ��дItemʧ�ܣ�"));

			}
			else
			{
				m_IOPCServer->GetErrorString(pErrors[0], LOCALE_ID, &ErrorStr);
				m_writeResult = ErrorStr;
				m_writeResult.Remove('\r');
				m_writeResult.Remove('\n');
				//UpdateData(FALSE);
				CoTaskMemFree(pErrors);
			}
		}
		else
		{
			MessageBoxW(L"δѡ�д��޸ĵ��");
		}
	}
	else
	{
		MessageBoxW(L"δ����������ӵ���ֵ��");
	}*/


}

//��ʱ��ʵ�ֺ���
void CBasicDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case ETimerId_1: {                        //��ʱ��ID
		static DWORD dwLastUpdate = GetTickCount(); //��¼����ʱ��
		if (GetTickCount() - dwLastUpdate >= 500)//�ж�ʱ����
		{
			OnRead();//����ͬ����
			AutoMain();
			dwLastUpdate = GetTickCount();//��¼ʱ����
		}
		//����ʲô������
	}
					 break;
	case ETimerId_2: {                        //��ʱ��ID
		static DWORD dwLastUpdate = GetTickCount(); //��¼����ʱ��
		if (GetTickCount() - dwLastUpdate >= 1000)//�ж�ʱ����
		{
			if (m_readValue[4]==1)
			{
				ShowResult(4);
				m_writeValue[4] = 2;
				OnWrite(4);//����ͬ����
				count = 0;
			}
			else if (m_readValue[4] == 2)
			{
				ShowResult(5);
				count++;
			}
			if (count>10)
			{
				//MessageBox(_T("OPCͨѶ���ӳ�ʱ!"));
				count = 0;
				KillTimer(ETimerId_2);
			}
			dwLastUpdate = GetTickCount();//��¼ʱ����
		}
		//����ʲô������
	}
					 break;
	default:
		break;
	}
}

void CBasicDemoDlg::AutoMain()
{
	switch (m_readValue[0]) {
	case 0:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 1:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 2:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 3:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 4:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 5:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 6:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 7:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 8:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 9:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 10:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	case 11:Left_Offset = 200; Right_Offset = 200; Upper_Offset = 200; Lower_Offset = 200; break;
	default:
		break;
	}
	switch (step) {
	case 0: if (m_readValue[1]==3)
	{
		step = 1;
	}
		 break;
	case 1: OnBnClickedSoftwareOnceButton(); m_writeValue[2] = 3; OnWrite(2); OnWrite(3); step = 2; break;
	case 2: if (m_readValue[1] == 4)
	{
		step = 3;
	}
			break;
	case 3: OnBnClickedSoftwareOnceButton(); m_writeValue[2] = 4; OnWrite(2); OnWrite(3); step = 0; break;
	default:
		break;
	}
}

void CBasicDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CSliderCtrl   *pSlidCtrl_1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	m_int_1 = pSlidCtrl_1->GetPos();//ȡ�õ�ǰλ��ֵ  
	CString Gray_threshold_1;
	Gray_threshold_1.Format(_T("%d"), m_int_1);
	SetDlgItemText(IDC_EDIT6, Gray_threshold_1);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


