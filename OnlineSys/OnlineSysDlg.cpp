
// OnlineSysDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "OnlineSys.h"
#include "OnlineSysDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// COnlineSysDlg dialog
CString msg, cam_dir;
int chk_cam, chk_eject, chk_prev;
bool run = false;
bool previewOnline = false;

// Ejector global variables
WORD wRtn;
WORD wBoardNo;
WORD wTotalBoards;
WORD boardNo = 0;
vector<DWORD> _cmd;
vector<int> i_PortNo;
vector<int> delay;
double delay_factor = 0.1;

IXUD_DEVICE_INFO sDevInfo[MAX_BOARD_NUMBER];
IXUD_CARD_INFO sCardInfo[MAX_BOARD_NUMBER];
char szModelName[20] = "Unknow Device";

// inference environment init variables
const vector<Scalar> colors = { Scalar(152,63,144),Scalar(0,255,0), Scalar(29,147,247), Scalar(0, 0, 255) };
const float Im_W = 512;
const float Im_H = 512;
const float Score_threshold = 0.48;
const float NMS_threshold = 0.35;
const float Conf_threshold = 0.45;


Mat frame, Img_A, Img_B, s_frame;
int thresh = 240;
vector<Detection> output;
String my_label;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
int n_count = 0, total_count = 0, count_3x = 0;
vector<Point> centroids;
BITMAPINFO* m_pBitmapInfo_on;
Mat Ken = getStructuringElement(MorphShapes(MORPH_ELLIPSE), Size(5, 5));

COnlineSysDlg::COnlineSysDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ONLINESYS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_Channel_A = 0; m_Channel_B = 1;
	m_pCurrent_A = NULL; m_pCurrent_B = NULL;
	m_SizeX = 0;
	m_SizeY = 0;
	n_Lines = 200;
	exp = 48;
	m_BufferPitch_A = m_SizeX; m_BufferPitch_B = m_SizeX;
	//m_bScreenRefreshCompleted = TRUE;

	// thread stuff
	DispThread = NULL;
	grabData = NULL;
	seedCheck = NULL;
	m_bDisp = false;
	m_bGrab = false;
	m_bSeedChk = false;

	// Build bitmap info Y8
	m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = 24;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;
	m_pBitmapInfo->bmiHeader.biWidth = 0;
	m_pBitmapInfo->bmiHeader.biHeight = 0;

	m_pBitmapInfo_on = m_pBitmapInfo;

	m_ChannelState_A = MC_ChannelState_ORPHAN;
	m_ChannelState_B = MC_ChannelState_ORPHAN;
}


void COnlineSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_CAM, m_check);
	DDX_Control(pDX, IDC_CHECK_EJECT, e_check);
	DDX_Control(pDX, IDC_CHECK_PREVIEW, p_check);
	DDX_Control(pDX, IDC_BUTTON_IND_CAM, Ind_c);
	DDX_Control(pDX, IDC_BUTTON_IND_EJEC, Ind_e);
	DDX_Control(pDX, IDC_BUTTON_ONLINE, go_on);

	DDX_Control(pDX, IDC_BUTTON_EJ_1, Ind_1);
	DDX_Control(pDX, IDC_BUTTON_EJ_2, Ind_2);
	DDX_Control(pDX, IDC_BUTTON_EJ_3, Ind_3);
	DDX_Control(pDX, IDC_BUTTON_EJ_4, Ind_4);
	DDX_Control(pDX, IDC_BUTTON_EJ_5, Ind_5);
	DDX_Control(pDX, IDC_BUTTON_EJ_6, Ind_6);
	DDX_Control(pDX, IDC_BUTTON_EJ_7, Ind_7);
	DDX_Control(pDX, IDC_BUTTON_EJ_8, Ind_8);
	DDX_Control(pDX, IDC_BUTTON_EJ_9, Ind_9);
	DDX_Control(pDX, IDC_BUTTON_EJ_10, Ind_10);
	DDX_Control(pDX, IDC_BUTTON_EJ_11, Ind_11);
	DDX_Control(pDX, IDC_BUTTON_EJ_12, Ind_12);
	DDX_Control(pDX, IDC_BUTTON_EJ_13, Ind_13);
	DDX_Control(pDX, IDC_BUTTON_EJ_14, Ind_14);
	DDX_Control(pDX, IDC_BUTTON_EJ_15, Ind_15);
	DDX_Control(pDX, IDC_BUTTON_EJ_16, Ind_16);
}


BEGIN_MESSAGE_MAP(COnlineSysDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MDL, &COnlineSysDlg::OnBnClickedButtonMdl)
	ON_BN_CLICKED(IDCANCEL, &COnlineSysDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_EJECT, &COnlineSysDlg::OnBnClickedCheckEject)
	ON_BN_CLICKED(IDC_CHECK_CAM, &COnlineSysDlg::OnBnClickedCheckCam)
	ON_BN_CLICKED(IDC_BUTTON_ONLINE, &COnlineSysDlg::OnBnClickedButtonOnline)
	ON_BN_CLICKED(IDC_BUTTON_SET, &COnlineSysDlg::OnBnClickedButtonSet)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COnlineSysDlg message handlers

BOOL COnlineSysDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void COnlineSysDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void COnlineSysDlg::OnPaint()
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


HCURSOR COnlineSysDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COnlineSysDlg::displayStatusMsg(CString s) {
	s.Format(_T(R"(Status: %s)"), s);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextW(s);
}


void COnlineSysDlg::OnBnClickedButtonMdl()
{
	// load model and labels
	CString mdl_dir, class_dir;
	CString mdl_dir_str = _T("Select Model file *.onnx||*.ONNX");
	CFileDialog FileDlg(TRUE, _T(".onnx"), NULL, 0, mdl_dir_str);

	if (FileDlg.DoModal() == IDOK) {
		mdl_dir = FileDlg.GetPathName();

		CT2CA str_buf(mdl_dir);
		string mdl_str(str_buf);
		my_Net = dnn::readNet(mdl_str);
		my_Net.setPreferableBackend(dnn::DNN_BACKEND_CUDA);
		my_Net.setPreferableTarget(dnn::DNN_TARGET_CUDA_FP16);
		bool is_net = my_Net.empty();

		if (!is_net) {
			msg.Format(_T("Model is loaded Succesfully with CUDA"));
		}
		else {
			msg.Format(_T("Model NOT loaded"));
		}
		
		displayStatusMsg(msg);

	}

	CString cls_dir_str = _T("Select Classes file *.txt||*.TXT");
	CFileDialog FileDlg2(TRUE, _T(".txt"), NULL, 0, cls_dir_str);

	if (FileDlg2.DoModal() == IDOK) {
		class_dir = FileDlg2.GetPathName();

		CT2CA fil_buf(class_dir);
		string class_str(fil_buf);

		ifstream ifs(class_str);
		string line;
		while (getline(ifs, line)) {
			class_list.push_back(line);
			cout << 1000;
		}
		cout << class_list[0] << endl;
		msg.Format(_T("Classes loaded Succesfully = [%s, %s, %s]"), CString(class_list[0].c_str()), CString(class_list[1].c_str()), CString(class_list[2].c_str()));
		displayStatusMsg(msg);
	}
}


void COnlineSysDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_bDisp = false;
	m_bGrab = false;
	Sleep(500);

	CDialogEx::OnCancel();
	try {
		McSetParamInt(m_Channel_A, MC_ChannelState, MC_ChannelState_IDLE);
		McSetParamInt(m_Channel_B, MC_ChannelState, MC_ChannelState_IDLE);

		// Delete the channel
		McDelete(m_Channel_A);
		McDelete(m_Channel_B);
	}
	catch (...) {}
}


void COnlineSysDlg::OnBnClickedCheckEject()
{
	changeIndicatorColor(Ind_e, 'y');

	chk_eject = e_check.GetCheck();

	if (chk_eject == BST_CHECKED) {
		// 1. LOAD, SETUP EJECTORS
		// 
		// Initalize ejector
		char model_name[20] = "Unknow Device";
		WORD PortIdx;

		wRtn = Ixud_DriverInit(&wTotalBoards); // load drivers
		if (wRtn != 0){
			MessageBox(_T("Can NOT Load ejectors !"), _T("Ejector Error"), MB_OK);
			changeIndicatorColor(Ind_e, 'r');
		}
		for (WORD Idx = 0; Idx < wTotalBoards; Idx++) {
			wRtn = Ixud_GetCardInfo(Idx, &sDevInfo[Idx], &sCardInfo[Idx], szModelName);
			boardNo = Idx;
		}
		
		static const int nLen = 35;
		char str[nLen];

		WORD portNo = sCardInfo[boardNo].wDIOPortWidth; // get card width
		for (PortIdx = 0; PortIdx < (sCardInfo[boardNo].wDIOPorts + sCardInfo[boardNo].wDOPorts); PortIdx++) {//
			_ltoa_s(PortIdx, str, 35);
		}

		CString modelName(szModelName);
		msg.Format(_T("Ejector board [ %s ] loaded SUCCESFULLY [#ports = %d]"), modelName, 4*portNo); //model_name
		displayStatusMsg(msg);

		// 2. TEST EJECTORS
		changeIndicatorColor(Ind_e, 'g');

		/*CString command_value = _T("00000001");
		int store = _wtoi(command_value);
		DWORD do_command = (DWORD)store;*/

		WORD wOutPortNo = 3;
		//configure port
		if (sCardInfo[boardNo].wDIOPorts > 1) {
			wRtn = Ixud_SetDIOModes32(boardNo, 1 << wOutPortNo);
		}

		//DWORD hexv;
		//// write Digital output value
		//for (int i = 0; i < 7; i++) { // loop turn on first 7 ejectors
		//	hexv = 1 << i;
		//	wRtn = Ixud_WriteDO(boardNo, wOutPortNo, hexv); // send signal to ejector #1
		//	Sleep(1);
		//}
		//wRtn = Ixud_WriteDO(boardNo, wOutPortNo, 0x00);
		//Sleep(200);

		vector<Point> test;
		test.push_back(Point(176, 30));
		test.push_back(Point(476, 90));

		blowEjector(test);

	}
	else {
		// UNLOAD EJECTOR
		Ixud_DriverClose();
		changeIndicatorColor(Ind_e, 'n');
	}
}


void WINAPI GlobalCallback_A(PMCSIGNALINFO SigInfo) {
	if (SigInfo && SigInfo->Context) {
		COnlineSysDlg* pDlg = (COnlineSysDlg*)SigInfo->Context;
		pDlg->Callback_A(SigInfo);
	}
}


void WINAPI GlobalCallback_B(PMCSIGNALINFO SigInfo) {
	if (SigInfo && SigInfo->Context) {
		COnlineSysDlg* pDlg = (COnlineSysDlg*)SigInfo->Context;
		pDlg->Callback_B(SigInfo);
	}
}


void COnlineSysDlg::Callback_A(PMCSIGNALINFO SigInfo) {
	switch (SigInfo->Signal)
	{
		case MC_SIG_SURFACE_PROCESSING:
			McGetParamPtr(SigInfo->SignalInfo, MC_SurfaceAddr, &m_pCurrent_A);
			/*if (m_bScreenRefreshCompleted) {

			}*/
			break;
		case MC_SIG_ACQUISITION_FAILURE:
			MessageBox(_T("Acquisition Failure !"), _T("GrablinkWeb_CAMERA_A"), MB_OK);
			break;
		default:
			break;
	}
}


void COnlineSysDlg::Callback_B(PMCSIGNALINFO SigInfo) {
	switch (SigInfo->Signal)
	{
	case MC_SIG_SURFACE_PROCESSING:
		McGetParamPtr(SigInfo->SignalInfo, MC_SurfaceAddr, &m_pCurrent_B);
		break;
	case MC_SIG_ACQUISITION_FAILURE:
		MessageBox(_T("Acquisition Failure !"), _T("GrablinkWeb_CAMERA_B"), MB_OK);
		break;
	default:
		break;
	}
}


UINT DisplayCam(LPVOID param) {
	COnlineSysDlg* pMain = (COnlineSysDlg*)param;

	int cam_int = 1;// 1 for A, 2 for B
	//DWORD dwresult = 0;
	while (pMain->m_bDisp) {
		//dwresult = WaitForSingleObject(pMain->DispThread.getha)
		//McSetParamInt(pMain->m_Channel_A, MC_ChannelState, MC_ChannelState_ACTIVE);
		pMain->OnDisplay(cam_int);
	}

	return 0;
}


void COnlineSysDlg::OnDisplay(int _cam) {
	CRect rect;
	CClientDC dc(GetDlgItem(IDC_STATIC_DISPLAY));
	GetDlgItem(IDC_STATIC_DISPLAY)->GetClientRect(&rect);

	BYTE* pBuffer_A;
	BYTE* pBuffer_B;

	switch (_cam) {
		case 1: // cam A
			// Configure the bitmap info according to the image size
			m_pBitmapInfo->bmiHeader.biWidth = m_BufferPitch_A / (m_pBitmapInfo->bmiHeader.biBitCount / 8);  // Width = Pitch(bytes) 
			m_pBitmapInfo->bmiHeader.biHeight = -m_SizeY;

			pBuffer_A = (BYTE*)m_pCurrent_A;

			SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
			StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_SizeX, m_SizeY, pBuffer_A, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		case 2: //cam B
			// Configure the bitmap info according to the image size
			m_pBitmapInfo->bmiHeader.biWidth = m_BufferPitch_B / (m_pBitmapInfo->bmiHeader.biBitCount / 8);  
			m_pBitmapInfo->bmiHeader.biHeight = -m_SizeY;

			pBuffer_B = (BYTE*)m_pCurrent_B;

			SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
			StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_SizeX, m_SizeY, pBuffer_B, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	
	//McGetParamInt(m_Channel_A, MC_ChannelState, &m_ChannelState_A);
	
}


void COnlineSysDlg::OnBnClickedCheckCam()
{
	changeIndicatorColor(Ind_c, 'y'); // change color of btn to yellow

	chk_cam = m_check.GetCheck();
	if (chk_cam == BST_CHECKED) {

		// LOAD, SETUP CAMERAS ////////////////////////////////////////////////////////////////////
		
		// Create a channel and associate it with the first connector on the first board
		McCreate(MC_CHANNEL, &m_Channel_A);
		McCreate(MC_CHANNEL, &m_Channel_B);

		McSetParamInt(m_Channel_A, MC_DriverIndex, 0);
		McSetParamInt(m_Channel_B, MC_DriverIndex, 0);

		// For Grablink DualBase
		McSetParamStr(m_Channel_A, MC_Connector, "A");
		McSetParamStr(m_Channel_B, MC_Connector, "B");

		// Load CAM file
		CString cam_str_filt = _T("Camera Files *.cam|*.CAM");
		CFileDialog FileDlg(TRUE, _T(".cam"), NULL, 0, cam_str_filt);
		if (FileDlg.DoModal() == IDOK)
		{
			cam_dir = FileDlg.GetPathName();
			CT2CA cam_buf(cam_dir);
			string cam_str(cam_buf);
			McSetParamStr(m_Channel_A, MC_CamFile, cam_buf);
			McSetParamStr(m_Channel_B, MC_CamFile, cam_buf);

			msg.Format(_T("%s is loaded "), FileDlg.GetFileName());
			displayStatusMsg(msg);
		}

		//set params
		McSetParamInt(m_Channel_A, MC_Expose_us, exp); McSetParamInt(m_Channel_B, MC_Expose_us, exp); //exposure
		McSetParamInt(m_Channel_A, MC_Period_us, 50); McSetParamInt(m_Channel_B, MC_Period_us, 50); // period
		McSetParamInt(m_Channel_A, MC_PeriodTrim, 0); McSetParamInt(m_Channel_B, MC_PeriodTrim, 0);
		McSetParamInt(m_Channel_A, MC_ColorFormat, MC_ColorFormat_RGB24); McSetParamInt(m_Channel_B, MC_ColorFormat, MC_ColorFormat_RGB24); // color format
		McSetParamInt(m_Channel_A, MC_AcquisitionMode, MC_AcquisitionMode_WEB); McSetParamInt(m_Channel_B, MC_AcquisitionMode, MC_AcquisitionMode_WEB);// Acqusition mode
		McSetParamInt(m_Channel_A, MC_PageCaptureMode, MC_PageCaptureMode_FIRST_LINE); McSetParamInt(m_Channel_B, MC_PageCaptureMode, MC_PageCaptureMode_FIRST_LINE);
		McSetParamInt(m_Channel_A, MC_StrobeMode, MC_StrobeMode_NONE); McSetParamInt(m_Channel_B, MC_StrobeMode, MC_StrobeMode_NONE);
		McSetParamInt(m_Channel_A, MC_GrabWindow, MC_GrabWindow_NOBLACK); McSetParamInt(m_Channel_B, MC_GrabWindow, MC_GrabWindow_NOBLACK);
		McSetParamInt(m_Channel_A, MC_PageLength_Ln, n_Lines); McSetParamInt(m_Channel_B, MC_PageLength_Ln, n_Lines); // Choose the number of lines per page
		McSetParamInt(m_Channel_A, MC_TrigMode, MC_TrigMode_IMMEDIATE); McSetParamInt(m_Channel_B, MC_TrigMode, MC_TrigMode_IMMEDIATE);
		McSetParamInt(m_Channel_A, MC_NextTrigMode, MC_NextTrigMode_REPEAT); McSetParamInt(m_Channel_B, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
		McSetParamInt(m_Channel_A, MC_SeqLength_Ln, MC_INDETERMINATE); McSetParamInt(m_Channel_B, MC_SeqLength_Ln, MC_INDETERMINATE);
		McSetParamInt(m_Channel_A, MC_BreakEffect, MC_BreakEffect_FINISH); McSetParamInt(m_Channel_B, MC_BreakEffect, MC_BreakEffect_FINISH);
		McSetParamInt(m_Channel_A, MC_TapConfiguration, MC_TapConfiguration_BASE_1T24); McSetParamInt(m_Channel_B, MC_TapConfiguration, MC_TapConfiguration_BASE_1T24);
		McSetParamInt(m_Channel_A, MC_InterleavedAcquisition, MC_InterleavedAcquisition_OFF); McSetParamInt(m_Channel_B, MC_InterleavedAcquisition, MC_InterleavedAcquisition_OFF);
		McSetParamInt(m_Channel_A, MC_RedBlueSwap, MC_RedBlueSwap_ENABLE); McSetParamInt(m_Channel_B, MC_RedBlueSwap, MC_RedBlueSwap_ENABLE);
		McSetParamInt(m_Channel_A, MC_LineCaptureMode, MC_LineCaptureMode_ALL); McSetParamInt(m_Channel_B, MC_LineCaptureMode, MC_LineCaptureMode_ALL);
		McSetParamInt(m_Channel_A, MC_WBO_Mode, MC_WBO_Mode_NONE); McSetParamInt(m_Channel_B, MC_WBO_Mode, MC_WBO_Mode_NONE);
		McGetParamInt(m_Channel_A, MC_ImageSizeX, &m_SizeX); // Retrieve image dimensions
		McGetParamInt(m_Channel_A, MC_ImageSizeY, &m_SizeY);
		McGetParamInt(m_Channel_A, MC_BufferPitch, &m_BufferPitch_A); McGetParamInt(m_Channel_B, MC_BufferPitch, &m_BufferPitch_B);
		McSetParamInt(m_Channel_A, MC_SurfaceCount, EURESYS_SURFACE_COUNT); McSetParamInt(m_Channel_B, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

		// Enable MultiCam signals
		McSetParamInt(m_Channel_A, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON); McSetParamInt(m_Channel_B, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
		McSetParamInt(m_Channel_A, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON); McSetParamInt(m_Channel_B, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

		// Register the callback function
		McRegisterCallback(m_Channel_A, GlobalCallback_A, this); 
		//McRegisterCallback(m_Channel_B, GlobalCallback_B, this);

		// Start an acquisition sequence by activating the channel
		MCSTATUS result1 = McSetParamInt(m_Channel_A, MC_ChannelState, MC_ChannelState_ACTIVE); 
		//MCSTATUS result2 = McSetParamInt(m_Channel_B, MC_ChannelState, MC_ChannelState_ACTIVE);
		
		// PREVIEW CAMERA and INDICATOR
		//if ((result1 == MC_OK)&&((result2 == MC_OK))) {
		if (result1 == MC_OK) {
			changeIndicatorColor(Ind_c, 'g'); // camera is good
			// display camera in a thread
			m_bDisp = true;
			DispThread = AfxBeginThread(DisplayCam, this);
			SetTimer(100, 30, NULL);

			msg.Format(_T("Camera preview <SUCCESS>"));
			displayStatusMsg(msg);
		}
		else { 
			changeIndicatorColor(Ind_c, 'r');
			MessageBox(_T("Acquisition Failure !"), _T("GrablinkWeb_CAMERA_A"), MB_OK);
		}
			
	}
	else {
		// 1. STOP DISPLAY and Change indicator
		// stop camera display thread
		m_bDisp = true;

		// 2. UNLOAD CAMERAS
		McSetParamInt(m_Channel_A, MC_ChannelState, MC_ChannelState_IDLE); McSetParamInt(m_Channel_B, MC_ChannelState, MC_ChannelState_IDLE);

		McDelete(m_Channel_A);
		McDelete(m_Channel_B);

		changeIndicatorColor(Ind_c, 'n');
	}
}


void COnlineSysDlg::changeIndicatorColor(CMFCButton& btn, char color) {
		btn.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
		btn.m_bTransparent = false;

		switch (color)
		{
		case 'r':
			btn.SetFaceColor(RGB(255, 0, 0));
			break;

		case 'g':
			btn.SetFaceColor(RGB(0, 255, 0));
			break;

		case 'y':
			btn.SetFaceColor(RGB(255, 255, 0));
			break;

		case 'n':
			DWORD d = GetSysColor(COLOR_BTNSHADOW);
			btn.SetFaceColor(d);
			break;
		}

	UpdateWindow();
}


void COnlineSysDlg::OnTimer(UINT_PTR nIDEvent) // update fps every 100ms
{
	if ((nIDEvent == 100)&& (chk_cam == BST_CHECKED))
	{
		double fps = 0;
		MCSTATUS result = 0;
		try {
			result = McGetParamNmFloat(m_Channel_A, "PerSecond_Fr", &fps);
		}
		catch (...) {}
		
		CString tmp = _T("");
		tmp.Format(L"%.3f fps", fps);
		SetDlgItemText(IDC_STATIC_FPS, tmp);
	}
	else
	{
		CString tmp = _T("");
		tmp.Format(L"%.3f fps", 0.00);
		SetDlgItemText(IDC_STATIC_FPS, tmp);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void COnlineSysDlg::blowEjector(vector<Point> pixels) {
	_cmd = pix2cmd(pixels, i_PortNo, delay);
	for (int i = 0; i<pixels.size();i++){
			wRtn = Ixud_WriteDO(boardNo, WORD(i_PortNo[i]), _cmd[i]);
			Sleep(1);
			wRtn = Ixud_WriteDO(boardNo, WORD(i_PortNo[i]), 0x00);
	}
	msg.Format(_T("Delay time 1 = %d, 2 = %d "), delay[0], delay[1]);
	displayStatusMsg(msg);

	
}


vector<DWORD> COnlineSysDlg::pix2cmd(vector<Point> pixels, vector<int>& PortNo, vector<int>& delay) {
	int _num = pixels.size();
	int _port, _nozzle, _delay, _bin,_noz;
	string binary;
	DWORD hexv;
	vector<DWORD> out;
	
	for (int i = 0; i < _num; i++) {
		int Pix = ceil(pixels[i].x / 16);
		_port = ceil(Pix / 8.0);
		_nozzle = Pix % 8;
		_delay = int(round(delay_factor * pixels[i].y));
		_bin = 1 << _nozzle;

		out.push_back(DWORD(_bin));
		PortNo.push_back(_port-1);
		delay.push_back(_delay);
		_noz = Pix / 2;

		// display
		switch (_noz) {
			case 1:
				changeIndicatorColor(Ind_1, 'g');
				break;
			case 2:
				changeIndicatorColor(Ind_2, 'g');
				break;
			case 3:
				changeIndicatorColor(Ind_3, 'g');
				break;
			case 4:
				changeIndicatorColor(Ind_4, 'g');
				break;
			case 5:
				changeIndicatorColor(Ind_5, 'g');
				break;
			case 6:
				changeIndicatorColor(Ind_6, 'g');
				break;
			case 7:
				changeIndicatorColor(Ind_7, 'g');
				break;
			case 8:
				changeIndicatorColor(Ind_8, 'g');
				break;
			case 9:
				changeIndicatorColor(Ind_9, 'g');
				break;
			case 10:
				changeIndicatorColor(Ind_10, 'g');
				break;
			case 11:
				changeIndicatorColor(Ind_11, 'g');
				break;
			case 12:
				changeIndicatorColor(Ind_12, 'g');
				break;
			case 13:
				changeIndicatorColor(Ind_13, 'g');
				break;
			case 14:
				changeIndicatorColor(Ind_14, 'g');
				break;
			case 15:
				changeIndicatorColor(Ind_15, 'g');
				break;
			case 16:
				changeIndicatorColor(Ind_16, 'g');
				break;
		}

		//UpdateData(TRUE);
		Sleep(1);

		changeIndicatorColor(Ind_1, 'n'); changeIndicatorColor(Ind_2, 'n'); changeIndicatorColor(Ind_3, 'n'); changeIndicatorColor(Ind_4, 'n');

		changeIndicatorColor(Ind_5, 'n'); changeIndicatorColor(Ind_6, 'n'); changeIndicatorColor(Ind_7, 'n'); changeIndicatorColor(Ind_8, 'n');

		changeIndicatorColor(Ind_9, 'n'); changeIndicatorColor(Ind_10, 'n'); changeIndicatorColor(Ind_11, 'n'); changeIndicatorColor(Ind_12, 'n');

		changeIndicatorColor(Ind_13, 'n'); changeIndicatorColor(Ind_14, 'n'); changeIndicatorColor(Ind_15, 'n'); changeIndicatorColor(Ind_16, 'n');

	}
	return out;
}


void COnlineSysDlg::OnBnClickedCheckPreview()
{
	chk_prev = p_check.GetCheck();

	if (chk_prev == BST_CHECKED) {
		previewOnline = true;
		msg.Format(_T("Online preview turned on..."));
		displayStatusMsg(msg);
	}
	else{ previewOnline = false; }
	msg.Format(_T("Online preview turned off..."));
	displayStatusMsg(msg);
}


UINT GrabData(LPVOID param) {
	COnlineSysDlg* pMain = (COnlineSysDlg*)param;
	int k = 0;
	while (pMain->m_bGrab) {
		/*msg.Format(_T("Iteration = %d"), k);
		pMain->displayStatusMsg(msg);*/
		//Sleep(10);
		pMain->OnGrabData(previewOnline);
		k++;
	}
	return 0;
}


void COnlineSysDlg::OnGrabData(bool previewOn) {
	
	BYTE* pBuffer_A = (BYTE*)m_pCurrent_A;
	//BYTE* pBuffer_B = (BYTE*)m_pCurrent_B;

	// convert image to mat
	Mat ImgA = Mat(m_SizeY, m_SizeX, CV_8UC3, pBuffer_A);
	//Mat ImgB = Mat(m_SizeY, m_SizeX, CV_8UC3, pBuffer_B);
	
	// detect if frame has seed
	detectSeed(ImgA, n_count);
	total_count = total_count + n_count;

	int k = 0;

	if (n_count>0){

		// resize
		resize(ImgA, Img_A, Size(512, 100));
		//resize(ImgB, Img_B, Size(512, 100));

		// concatenate
		//vconcat(Img_A, Img_B, frame);

		//format to yolo
		//frame = format_yolov5(frame);
		frame = format_yolov5(Img_A);

		// predict
		detect(frame, my_Net, output, class_list);
		int kk = 0;

		if (previewOnline) {
			CRect rect;
			CClientDC dco(GetDlgItem(IDC_STATIC_DISPLAY));
			GetDlgItem(IDC_STATIC_DISPLAY)->GetClientRect(&rect);

			// Configure the bitmap info according to the image size
			m_pBitmapInfo_on->bmiHeader.biWidth = m_BufferPitch_A / (m_pBitmapInfo_on->bmiHeader.biBitCount / 8);  // Width = Pitch(bytes) divided by the number of bytes per pixel
			m_pBitmapInfo_on->bmiHeader.biHeight = -m_SizeY;

			
			// insert predictions
			for (int i = 0; i < output.size(); i++) {

				auto detection = output[i];
				auto box = detection.box;
				auto classId = detection.class_id;
				auto cent = detection.cent;

				const auto color = colors[classId % colors.size()];
				rectangle(frame, box, color, 3);

				rectangle(frame, Point(box.x, box.y - 20), Point(box.x + box.width, box.y), color, FILLED);
				my_label = (class_list[classId] + ": 0." + to_string(int(detection.confidence * 100))).c_str();
				putText(frame, my_label, Point(box.x, box.y - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));

				if (classId == 1) { // 3x seed
					centroids.push_back(cent);
					kk++;
				}
			}
			//display image
			SetStretchBltMode(dco.GetSafeHdc(), COLORONCOLOR);
			StretchDIBits(dco.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, frame.cols, frame.rows, frame.data, m_pBitmapInfo_on, DIB_RGB_COLORS, SRCCOPY);
			//UpdateData(TRUE);
			
			//imshow("my_result", frame);
			//waitKey(1);
			//Sleep(2);
		}
		else {
			for (int i = 0; i < output.size(); i++) {
				auto detection = output[i];
				if (detection.class_id == 1) { // 3x seed
					centroids.push_back(detection.cent);
					kk++;
				}
			}
		}
		// blow ejector if neccesary
		if (!centroids.empty()) {
			blowEjector(centroids);
		}
		count_3x = count_3x + kk; // update seed count
	}
	centroids.clear();
	msg.Format(_T("%d / %d"), count_3x, total_count);
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowTextW(msg);
	
	//msg.Format(_T("detected frame dims [%d, %d]"), frame.cols, frame.rows);
	//displayStatusMsg(msg);
}


void COnlineSysDlg::OnBnClickedButtonOnline()
{
	// Change btn color and name
	changeIndicatorColor(go_on, 'y');

	// disable display thread
	m_bDisp = false;

	CString on_text;
	go_on.GetWindowText(on_text);

	if (on_text == _T("GO ONLINE")){
		go_on.SetWindowTextW(_T("STOP ONLINE"));
		
		//
		msg.Format(_T("Setting up inference environment"));
		displayStatusMsg(msg);
		// setup inference environment

		run = true;
		count_3x = 0;
		total_count = 0;

		MCSTATUS result = McSetParamInt(m_Channel_A, MC_ChannelState, MC_ChannelState_ACTIVE); 
		//McSetParamInt(m_Channel_B, MC_ChannelState, MC_ChannelState_ACTIVE);

		msg.Format(_T("Channel State Activated..."));
		displayStatusMsg(msg);
		
		// setup image grabbing thread
		if (result == MC_OK){

			// display camera in a thread
			msg.Format(_T("Online..."));
			displayStatusMsg(msg);
			//namedWindow("my_result", WINDOW_AUTOSIZE);
			m_bGrab = true;
			grabData = AfxBeginThread(GrabData, this);
			SetTimer(100, 30, NULL);
		}


		changeIndicatorColor(go_on, 'g');
	}
	else {
		// stop online
		m_bGrab = false;
		go_on.SetWindowTextW(_T("GO ONLINE"));

		msg.Format(_T("Paused online process"));
		displayStatusMsg(msg);

		changeIndicatorColor(go_on, 'n');

	}
	
}


Mat COnlineSysDlg::format_yolov5(const Mat& source) { // Format image to yolov5 input format
	int col = source.cols;
	int row = source.rows;
	int _max = MAX(col, row);
	Mat Res = Mat::zeros(_max, _max, CV_8UC3);
	source.copyTo(Res(Rect(0, 0, col, row)));

	return Res;
}


void COnlineSysDlg::detect(Mat& Image, dnn::Net& net, vector<Detection>& output, const vector<string>& className) { // detection function

	Mat blob; // reserve meomory if not reserved
	vector<Mat> outputs;
	Mat input_img = format_yolov5(Image);

	dnn::blobFromImage(input_img, blob, 1. / 255., Size(Im_W, Im_H), Scalar(), true, false);
	net.setInput(blob);
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	float x_factor = input_img.cols / Im_W;
	float y_factor = input_img.rows / Im_H;

	float* data = (float*)outputs[0].data;

	const int dimension = 8;
	const int rows = 16128; //////////////////////////////////////////////////////////////////////////////////////////////

	vector<int> class_ids;
	vector<float> confidences;
	vector<Rect> boxes;
	Point class_id;
	double max_class_score;

	// reserve memory
	boxes.reserve(300);
	confidences.reserve(300);
	class_ids.reserve(300);


	#pragma omp parallel for
	for (int i = 0; i < rows; i++) {
		float confidence = data[4];
		switch (confidence >= Conf_threshold) {
		case true:
		{
			float* classes_scores = data + 5;

			Mat scores(1, className.size(), CV_32FC1, classes_scores);

			minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

			switch ((max_class_score > Score_threshold) && (data[2] < 70)) {
			case true:
			{
				confidences.push_back(confidence);
				class_ids.push_back(class_id.x);

				boxes.push_back(Rect(int((data[0] - 0.5 * data[2]) * x_factor),
					int((data[1] - 0.5 * data[3]) * y_factor),
					int(data[2] * x_factor), int(data[3] * y_factor)));
			}
			}
		}
		}
		data += 8;
	}
	vector<int> nms_result;
	dnn::NMSBoxes(boxes, confidences, Score_threshold, NMS_threshold, nms_result);
	output.clear();
	Detection result;
	for (int i = 0; i < nms_result.size(); i++) { // put in GPU
		int idx = nms_result[i];

		result.class_id = class_ids[idx];
		result.confidence = confidences[idx];
		result.box = boxes[idx];
		result.cent = Point((boxes[idx].x + 0.5 * boxes[idx].width), (boxes[idx].y + 0.5 * boxes[idx].height));
		output.push_back(result);
	}
}


void COnlineSysDlg::OnBnClickedButtonSet()
{
	// TODO: Add your control notification handler code here
}


void COnlineSysDlg::detectSeed(Mat Img, int& count) { // detect seed
	Mat rgb[3];
	split(Img, rgb);
	Mat b_band = rgb[0];
	Mat res = b_band < thresh;

	morphologyEx(res, res, MORPH_CLOSE, Ken);
	findContours(res, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	count = contours.size();
}
