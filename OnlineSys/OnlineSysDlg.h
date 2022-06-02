
// OnlineSysDlg.h : header file
//

#pragma once

#include "UniDAQ.h"
#pragma comment(lib, "UniDAQ.lib")

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <Windows.h>
#include <afxbutton.h>
#include <vector>
#include <bitset>

#include "multicam.h"

using namespace std;
using namespace cv;

struct Detection {
	int class_id;
	float confidence;
	Rect box;
	Point cent;
};

// COnlineSysDlg dialog
class COnlineSysDlg : public CDialogEx
{
// Construction
public:
	COnlineSysDlg(CWnd* pParent = nullptr);	// standard constructor


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ONLINESYS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	// initalizing camera params
	MCHANDLE m_Channel_A, m_Channel_B;
	void Callback_A(PMCSIGNALINFO SigInfo);
	void Callback_B(PMCSIGNALINFO SigInfo);
	PVOID m_pCurrent_A, m_pCurrent_B;
	int m_SizeX;
	int m_SizeY;
	int m_BufferPitch_A, m_BufferPitch_B;
	BITMAPINFO* m_pBitmapInfo;
	CString m_strChannelStatus;
	int m_ChannelState_A, m_ChannelState_B;

	// thread parameters
	bool m_bDisp, m_bGrab, m_bSeedChk;
	CWinThread* DispThread, *grabData, *seedCheck;

	// personal functions
	void changeIndicatorColor(CMFCButton& btn, char color);
	void OnDisplay(int _cam);
	void displayStatusMsg(CString s);
	void blowEjector(vector<Point> pixels);
	vector<DWORD> pix2cmd(vector<Point> pixels, vector<int>& portNo, vector<int>& delay);
	Mat format_yolov5(const Mat& source);
	void detect(Mat& Image, dnn::Net& net, vector<Detection>& output, const vector<string>& className);
	void OnGrabData(bool previewOn);
	void detectSeed(Mat Img, int& count);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CButton m_check, e_check, p_check;
	CMFCButton Ind_c, Ind_e, go_on;
	CMFCButton Ind_1, Ind_2, Ind_3, Ind_4, Ind_5, Ind_6, Ind_7, Ind_8, Ind_9, Ind_10,
		Ind_11, Ind_12, Ind_13, Ind_14, Ind_15, Ind_16;

public:
	int n_Lines = 200;
	int exp = 48;
	dnn::Net my_Net;
	vector<string> class_list;
	Mat Img;
	afx_msg void OnBnClickedButtonMdl();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckEject();
	afx_msg void OnBnClickedCheckCam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckPreview();
	afx_msg void OnBnClickedButtonOnline();
	afx_msg void OnBnClickedButtonSet();
};
