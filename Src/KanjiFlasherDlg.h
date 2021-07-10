// KanjiFlasherDlg.h : header file
//

#include "WndNotifier.h"
#include "DataMgr.h"
#include "afxcmn.h"
#include "afxwin.h"
#if !defined(AFX_KanjiFlasherDLG_H__D75C39CB_4663_43B6_9CF5_FE8ACA666BE4__INCLUDED_)
#define AFX_KanjiFlasherDLG_H__D75C39CB_4663_43B6_9CF5_FE8ACA666BE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MYWM_NOTIFYICON (WM_USER+1)
/////////////////////////////////////////////////////////////////////////////
// CKanjiFlasherDlg dialog

class CKanjiFlasherDlg : public CDialog
{
// Construction
public:
	CKanjiFlasherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKanjiFlasherDlg)
	enum { IDD = IDD_KanjiFlasher_DIALOG };
	CSliderCtrl	m_ctrlAlertTransperancy;
	CSliderCtrl m_ctrlKanjiHiraSlider;
	CSliderCtrl m_ctrlHiraEngSlider;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanjiFlasherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON			m_hIcon;
	CMenu			m_trayMenu;  //Displaying menu when right clicked on the system tray.
    NOTIFYICONDATA	m_iconData;  //Icon Data

	// Generated message map functions
	//{{AFX_MSG(CKanjiFlasherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT onTrayNotify(WPARAM, LPARAM);  //Function to handle the notifications from the system tray menu
	afx_msg void OnClose();
	afx_msg void OnKanjiFlasherExit();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSettings();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnHidden(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDispWindowRightClicked(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_dispOrder;
	CWndNotifier m_wndTaskbarNotifier;
	CDataMgr m_dataMgr;
	DWORD m_dKanjiColor;
	DWORD m_dHiraganaColor;
	DWORD m_dEnglishColor;
public:
	CButton			m_chkKanji;
	CButton			m_chkHiragana;
	CButton			m_chkEnglish;
	CButton			m_chkAudioAlert;

	bool			m_blSerialRadio;
	CButton			m_ctrlSerial;

	CRichEditCtrl	m_ctrlKanjiColor;
	CRichEditCtrl	m_ctrlHiraganaColor;
	CRichEditCtrl	m_ctrlEnglishColor;

	CString m_strAppPath;

	afx_msg void OnBnClickedOk();
	afx_msg void OnKanjiFlasherAbout();
	afx_msg void OnBnClickedSelKanjiColor();
	afx_msg void OnBnClickedSelHirColor();
	afx_msg void OnBnClickedSelEngColor();	
	afx_msg void OnBnClickedCheckKanji();	
	afx_msg void OnBnClickedCheckHir();
	afx_msg void OnBnClickedCheckEng();	
	afx_msg void OnBnClickedAudioAlert();	
	afx_msg void OnEnSetfocusRichedit1();
	afx_msg void OnEnSetfocusRichedit2();
	afx_msg void OnEnSetfocusRichedit3();
	BOOL ReadFromINI();
	BOOL WriteToINI();
	BOOL InitializeControls();
	CSliderCtrl m_ctrlDurationSlider;
	CComboBox m_ctrlSkinSelector;
	BOOL SetPlaying(LPCTSTR sound, HMODULE hmod, DWORD fdwsound);
	CButton m_ctrlRandom;

	int m_nFromRange;
	CEdit m_ctrlFromRange;
	CEdit m_ctrlToRange;
	int m_nExcelCount;
	int m_nWidth;
	int m_nHeight;
	afx_msg void OnPlayPause();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	CListCtrl m_List;
	CImageList	m_Image;
	BOOL		m_bDragging;
	int			m_nDragIndex;

public:
	afx_msg void OnLvnBeginrdragList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult);
	int GetListOrder(void);
public:
	BOOL SetListOrder(int dispOrder);
	BOOL VerifyOrder(CPoint& pp1,CPoint& pp2, CPoint& pp3);
public:
	BOOL ShowDisplayWindow(void);
	CButton m_chkAutoResize;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KanjiFlasherDLG_H__D75C39CB_4663_43B6_9CF5_FE8ACA666BE4__INCLUDED_)
