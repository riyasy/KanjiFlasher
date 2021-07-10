// KanjiFlasherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KanjiFlasher.h"
#include "KanjiFlasherDlg.h"
#include "Entry.h"
#include ".\KanjiFlasherdlg.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDT_POPUP  10001


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanjiFlasherDlg dialog

CKanjiFlasherDlg::CKanjiFlasherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKanjiFlasherDlg::IDD, pParent)
	, m_blSerialRadio(false)
	, m_nFromRange(0)
	, m_nExcelCount(0)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_bDragging(FALSE)
	, m_dispOrder(0)
{
	//{{AFX_DATA_INIT(CKanjiFlasherDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKanjiFlasherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKanjiFlasherDlg)
	DDX_Control(pDX, IDC_SLIDER_TRANSPERANCY, m_ctrlAlertTransperancy);
	DDX_Control(pDX, IDC_SLIDER_KAN_HIR_DELAY, m_ctrlKanjiHiraSlider);
	DDX_Control(pDX, IDC_SLIDER_HIR_ENG_DELAY, m_ctrlHiraEngSlider);
	DDX_Control(pDX, IDC_CHECKKANJI, m_chkKanji);
	DDX_Control(pDX, IDC_CHECKHIR, m_chkHiragana);
	DDX_Control(pDX, IDC_CHECKENG, m_chkEnglish);
	DDX_Control(pDX, IDC_CHECKAUDIO, m_chkAudioAlert);
	DDX_Control(pDX, IDC_RADIO_SERIAL, m_ctrlSerial);
	DDX_Control(pDX, IDC_RICHEDIT1, m_ctrlKanjiColor);
	DDX_Control(pDX, IDC_RICHEDIT2, m_ctrlHiraganaColor);
	DDX_Control(pDX, IDC_RICHEDIT3, m_ctrlEnglishColor);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SLIDER_CARD_DURATION, m_ctrlDurationSlider);
	DDX_Control(pDX, IDC_RADIO_RANDOM, m_ctrlRandom);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlSkinSelector);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlFromRange);
	DDX_Control(pDX, IDC_EDIT2, m_ctrlToRange);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_CHECK1, m_chkAutoResize);
}

BEGIN_MESSAGE_MAP(CKanjiFlasherDlg, CDialog)
	//{{AFX_MSG_MAP(CKanjiFlasherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(MYWM_NOTIFYICON,onTrayNotify)
	ON_WM_CLOSE()
	ON_COMMAND(ID_KanjiFlasherEXIT, OnKanjiFlasherExit)
	ON_WM_DESTROY()
	ON_COMMAND(IDSETTINGS, OnSettings)
	ON_WM_TIMER()	
	ON_MESSAGE(WM_HIDDEN,OnHidden)
	ON_MESSAGE(WM_DISPWINDOWRIGHTCLICKED, OnDispWindowRightClicked)	
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_COMMAND(ID_KanjiFlasher_ABOUT, OnKanjiFlasherAbout)
	ON_BN_CLICKED(IDC_SEL_KANJI_COLOR, OnBnClickedSelKanjiColor)
	ON_BN_CLICKED(IDC_SEL_HIR_COLOR, OnBnClickedSelHirColor)
	ON_BN_CLICKED(IDC_SEL_ENG_COLOR, OnBnClickedSelEngColor)
	ON_BN_CLICKED(IDC_CHECKKANJI, OnBnClickedCheckKanji)
	ON_BN_CLICKED(IDC_CHECKHIR, OnBnClickedCheckHir)
	ON_BN_CLICKED(IDC_CHECKENG, OnBnClickedCheckEng)
	ON_BN_CLICKED(IDC_CHECKAUDIO, OnBnClickedAudioAlert)
	ON_EN_SETFOCUS(IDC_RICHEDIT1, OnEnSetfocusRichedit1)
	ON_EN_SETFOCUS(IDC_RICHEDIT2, OnEnSetfocusRichedit2)
	ON_EN_SETFOCUS(IDC_RICHEDIT3, OnEnSetfocusRichedit3)
	//}}AFX_MSG_MAP
	
	ON_COMMAND(ID_PLAYPAUSE, OnPlayPause)
	ON_WM_HSCROLL()
	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LIST1, &CKanjiFlasherDlg::OnLvnBeginrdragList1)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, &CKanjiFlasherDlg::OnLvnBegindragList1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanjiFlasherDlg message handlers

BOOL CKanjiFlasherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);

	m_nWidth = wp.rcNormalPosition.right;
	m_nHeight = wp.rcNormalPosition.bottom;

	SetWindowPos(NULL,0,0,0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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


	//Get the application path
	TCHAR strFilePathTemp[MAX_PATH];
	GetModuleFileName(NULL ,strFilePathTemp, MAX_PATH);
	m_strAppPath = strFilePathTemp;
	m_strAppPath = m_strAppPath.Left(m_strAppPath.ReverseFind('\\')); 

	//Get Entry Count from kanjis.xls
	m_nExcelCount = m_dataMgr.InitializeXL(m_strAppPath + _T("\\kanjis.xls"));

	if(m_nExcelCount == 0)
	{
		AfxMessageBox(_T("kanjis.xls does not contain any entries."));
		ExitProcess(0);
	}


	m_wndTaskbarNotifier.Create(IDD_DIALOG2);

	//return TRUE;

	CString strTip(_T("KanjiFlasher")); //Tip to be displayed for the icon on system tray
  
	m_iconData.cbSize = sizeof(NOTIFYICONDATA);
	m_iconData.hWnd = this->GetSafeHwnd();
	m_iconData.uID = IDR_MAINFRAME;   //ICON RESOURCE ID

	m_iconData.uFlags = NIF_MESSAGE|NIF_ICON;
	m_iconData.uCallbackMessage = MYWM_NOTIFYICON;
	m_iconData.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP; 
	m_iconData.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDR_MAINFRAME));

	lstrcpyn(m_iconData.szTip, (LPCTSTR)strTip, sizeof(m_iconData.szTip));
  
	DWORD dwMessage = NIM_ADD;
	Shell_NotifyIcon(dwMessage, &m_iconData);    
  
	m_trayMenu.LoadMenu(IDR_KanjiFlasherMENU);  //MENU RESOURCE ID.
	




	//Initialising the scrollbars
	m_ctrlAlertTransperancy.SetRange(25, 255);
	m_ctrlDurationSlider.SetRange(1000,180000);
	m_ctrlKanjiHiraSlider.SetRange(0, 5);
	m_ctrlHiraEngSlider.SetRange(0, 5);


	//Initializing the window
	//m_wndTaskbarNotifier.Create(IDD_DIALOG1);
	
	//Get the Skins path
	CString strSkinPath = m_strAppPath;
	strSkinPath.Append(_T("\\Skins\\*.*"));

	//Recursively find the directories present with Skins
	m_ctrlSkinSelector.AddString(_T("default"));
	m_ctrlSkinSelector.AddString(_T("woody grey"));





	CString strDisplay;
	strDisplay.Format(_T("Total number of entries : %d"),m_nExcelCount);
	SetDlgItemText(IDC_DISPLAY_TOTAL, strDisplay);



	VERIFY (m_Image.Create (23, 23, TRUE, 0, 1));
	m_List.SetIconSpacing(90,10);
	CBitmap bm;
	VERIFY (bm.LoadBitmap (IDB_BMP_LIST));
	VERIFY (-1 != m_Image.Add (&bm, RGB (192, 192, 192)));
	// create a column in the CListCtrl
	VERIFY (-1 != m_List.InsertColumn (0, _T("Button Label"), LVCFMT_LEFT, 100));
	m_List.SetImageList (&m_Image, LVSIL_NORMAL);
	TCHAR *szLabels[] = {	_T(""),
							_T(""),
							_T("")	};
	// insert items into CListCtrl
	for (int iCntr = 0; iCntr < 3; iCntr++)
		VERIFY (-1 != m_List.InsertItem (LVIF_IMAGE | LVIF_TEXT, iCntr, szLabels[iCntr], 0, 0, iCntr, 0L));


	//Initial settings from INI
	if(FALSE ==	ReadFromINI())
	{
		InitializeControls();
		WriteToINI();
	}


	int nPos;
	CString strLabel;
	nPos = m_ctrlAlertTransperancy.GetPos();
	float transparency = static_cast<float>(280 - nPos)/255;
	int nPercentage = static_cast<int>(100 - (transparency*100));
	strLabel.Format(_T("%d %% transparent"), nPercentage);
	SetDlgItemText(IDC_TRANSPARENCY,strLabel);
	nPos = m_ctrlDurationSlider.GetPos();
	strLabel.Format(_T("%d seconds"), nPos/1000);
	SetDlgItemText(IDC_DURATION, strLabel);
	nPos = m_ctrlKanjiHiraSlider.GetPos();
	strLabel.Format(_T("%d seconds"), nPos);
	SetDlgItemText(IDC_DELAY_KH, strLabel);
	nPos = m_ctrlHiraEngSlider.GetPos();
	strLabel.Format(_T("%d seconds"), nPos);
	SetDlgItemText(IDC_DELAY_HE,strLabel);

	UpdateData(0);

	PostMessage(WM_SYSCOMMAND, SC_CLOSE);
	

	//SetTimer(IDT_POPUP,m_ctrlDurationSlider.GetPos(),NULL);	
	//ShowDisplayWindow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKanjiFlasherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKanjiFlasherDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKanjiFlasherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CKanjiFlasherDlg::onTrayNotify(WPARAM wParam,LPARAM lParam)
 {
    UINT uMsg = (UINT) lParam; 
    switch ( uMsg ) 
    { 
    case WM_LBUTTONDBLCLK:	//Display the settings dialog on double click
		KillTimer(IDT_POPUP);
		SetWindowPos(NULL,30,50,m_nWidth, m_nHeight, SWP_SHOWWINDOW);
		this->ShowWindow(SW_SHOW);
		m_wndTaskbarNotifier.HideWindow();		
    break;
    case WM_RBUTTONUP:		//Display the menu on right mouse click
    CPoint pt;    
    GetCursorPos(&pt);
    m_trayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, this);
    break;
    } 
    return TRUE;
}

void CKanjiFlasherDlg::OnClose() 
{
	ShowWindow(SW_HIDE);	//To minimize to system tray
	ReadFromINI();
	ShowDisplayWindow();
}

void CKanjiFlasherDlg::OnKanjiFlasherExit() 
{
	PostQuitMessage(0);	//Quit the application
}

void CKanjiFlasherDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	Shell_NotifyIcon(NIM_DELETE, &m_iconData);	//Delete the icon from system tray
}

void CKanjiFlasherDlg::OnOK() 
{
	//Save the settings and minimize to the system tray
	int nFrom = 1, nTo = 0;
	CString str;
	m_ctrlFromRange.GetWindowText(str);
	if(0 == _tcscmp(str,_T("")))
	{
		AfxMessageBox( _T("Please enter a value  for From Range!"));
	}
	else
	{
		nFrom = _ttoi(str);
		m_ctrlToRange.GetWindowText(str);
		if(0 == _tcscmp(str,_T("")))
		{
			AfxMessageBox( _T("Please enter a value for To Range!"));
		}
		else
		{
			nTo = _ttoi(str);
		

			if((nFrom == 0) || (nTo == 0) ||(nFrom > nTo) || (nFrom > m_nExcelCount) || (nTo > m_nExcelCount))
			{
				AfxMessageBox(_T("Please re-enter the values."));
			}
			else
			{		
				WriteToINI();
				ReadFromINI();
				ShowWindow(SW_HIDE); //To minimize to system tray	
				//SetTimer(IDT_POPUP,m_ctrlDurationSlider.GetPos(),NULL);
				ShowDisplayWindow();
			}
		}
	}
	
}

void CKanjiFlasherDlg::OnCancel() 
{
	//Cancel the settings and minimize to the system tray
	ShowWindow(SW_HIDE); //To minimize to system tray
	ReadFromINI();	
	//SetTimer(IDT_POPUP,m_ctrlDurationSlider.GetPos(),NULL);
	ShowDisplayWindow();
}

void CKanjiFlasherDlg::OnSettings() 
{
//On clicking the settings on menu, the settings dialog should be displayed
	KillTimer(IDT_POPUP);	
	SetWindowPos(NULL,30,50,m_nWidth, m_nHeight, SWP_SHOWWINDOW);
	ShowWindow(SW_SHOW)	;
	m_wndTaskbarNotifier.HideWindow();
}



void CKanjiFlasherDlg::OnTimer(UINT nIDEvent)
{
	CString str;
	m_trayMenu.GetMenuString(ID_PLAYPAUSE, str, NULL);

	switch (nIDEvent)
	{
	case IDT_POPUP:		
		if(IsWindowVisible() != TRUE && str.Compare(_T("Pause")) == 0)
		{

			ShowDisplayWindow();
			KillTimer(IDT_POPUP);
			
		}		
		break;	

	}
	
	CWnd::OnTimer(nIDEvent);
}

LRESULT CKanjiFlasherDlg::OnHidden(WPARAM wParam,LPARAM lParam)
{
	if(!this->IsWindowVisible())
	{
		SetTimer(IDT_POPUP,m_ctrlDurationSlider.GetPos(),NULL);
	}
	return 0;
}


void CKanjiFlasherDlg::OnBnClickedOk()
{
	OnOK();
}

void CKanjiFlasherDlg::OnKanjiFlasherAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CKanjiFlasherDlg::OnBnClickedSelKanjiColor()
{
	COLORREF stSelColor;
	CColorDialog frmKanjiColor;
	CFont fonts;
	if(IDOK ==frmKanjiColor.DoModal())
	{
		stSelColor = frmKanjiColor.GetColor();
		m_dKanjiColor = stSelColor;
		UpdateData(1);
		m_ctrlKanjiColor.SetBackgroundColor(FALSE, stSelColor);
		UpdateData(0);
	}
}



void CKanjiFlasherDlg::OnBnClickedSelHirColor()
{
	// TODO: Add your control notification handler code here
	COLORREF stSelColor;
	CColorDialog frmHiraganaColor;
	if(IDOK ==frmHiraganaColor.DoModal())
	{
		stSelColor = frmHiraganaColor.GetColor();
		m_dHiraganaColor = stSelColor;
		UpdateData(1);
		m_ctrlHiraganaColor.SetBackgroundColor(FALSE, stSelColor);
		UpdateData(0);
	}
}

void CKanjiFlasherDlg::OnBnClickedSelEngColor()
{
	// TODO: Add your control notification handler code here
	COLORREF stSelColor;
	CColorDialog frmEnglishColor;
	if(IDOK == frmEnglishColor.DoModal())
	{
		stSelColor = frmEnglishColor.GetColor();
		m_dEnglishColor = stSelColor;
		UpdateData(1);
		m_ctrlEnglishColor.SetBackgroundColor(FALSE, stSelColor);
		UpdateData(0);
	}
}

void CKanjiFlasherDlg::OnBnClickedCheckKanji()
{/*
	// TODO: Add your control notification handler code here
	UpdateData(1);
	if(m_chkKanji.GetCheck())
	{
		if(m_chkHiragana.GetCheck())
		{
			m_ctrlKanjiHiraSlider.EnableWindow(TRUE);
		}
		else
		{
			m_ctrlKanjiHiraSlider.EnableWindow(FALSE);
		}
	    
		if(m_chkEnglish.GetCheck())
		{
			m_ctrlKanjiHiraSlider.EnableWindow(TRUE);
			m_ctrlHiraEngSlider.EnableWindow(TRUE);
		}
		else
		{
			m_ctrlHiraEngSlider.EnableWindow(FALSE);
		}
	
	}
	else
	{
		m_ctrlKanjiHiraSlider.EnableWindow(FALSE);
		if(! m_chkHiragana.GetCheck())
		{
			m_ctrlHiraEngSlider.EnableWindow(FALSE);
		}
	}

	UpdateData(0);*/
}

void CKanjiFlasherDlg::OnBnClickedCheckHir()
{/*
	// TODO: Add your control notification handler code here
	UpdateData(1);
	if(m_chkHiragana.GetCheck())
	{
		if(m_chkKanji.GetCheck())
		{
			m_ctrlKanjiHiraSlider.EnableWindow(TRUE);
		}
		else
		{
			m_ctrlKanjiHiraSlider.EnableWindow(FALSE);
		}

		if(m_chkEnglish.GetCheck())
		{
			m_ctrlHiraEngSlider.EnableWindow(TRUE);
		}
		else
		{
			m_ctrlHiraEngSlider.EnableWindow(FALSE);
		}

	}
	else
	{
		if(!m_chkKanji.GetCheck() || !m_chkEnglish.GetCheck())
		{
			m_ctrlKanjiHiraSlider.EnableWindow(FALSE);
			m_ctrlHiraEngSlider.EnableWindow(FALSE);
		}

	}

	UpdateData(0);*/
}

void CKanjiFlasherDlg::OnBnClickedCheckEng()
{/*
	// TODO: Add your control notification handler code here
	UpdateData(1);
	if(m_chkEnglish.GetCheck())
	{
		if(m_chkHiragana.GetCheck())
		{
			m_ctrlHiraEngSlider.EnableWindow(TRUE);
		}
		else
		{
			m_ctrlHiraEngSlider.EnableWindow(FALSE);
		}
		
		if(m_chkKanji.GetCheck())
		{
			m_ctrlKanjiHiraSlider.EnableWindow(TRUE);
			m_ctrlHiraEngSlider.EnableWindow(TRUE);
		}
		else
		{
			m_ctrlKanjiHiraSlider.EnableWindow(FALSE);
			if(! m_chkHiragana.GetCheck())
			{
				m_ctrlHiraEngSlider.EnableWindow(FALSE);
			}
		}
	}
	else
	{
		m_ctrlHiraEngSlider.EnableWindow(FALSE);
		if(!m_chkHiragana.GetCheck())
		{
			m_ctrlKanjiHiraSlider.EnableWindow(FALSE);
		}
	}

	UpdateData(0);*/
}

void CKanjiFlasherDlg::OnBnClickedAudioAlert()
{
	// TODO: Add your control notification handler code here
	if(m_chkAudioAlert.GetCheck())
	{
		//TODO:: Audio Alert is enabled
	}
	else
	{
		//TODO:: AudioAlert is disabled
	}
}


// To avoid cursor positioning at rich edit control
void CKanjiFlasherDlg::OnEnSetfocusRichedit1()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SEL_KANJI_COLOR)->SetFocus();
}
void CKanjiFlasherDlg::OnEnSetfocusRichedit2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SEL_HIR_COLOR)->SetFocus();
}

void CKanjiFlasherDlg::OnEnSetfocusRichedit3()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SEL_ENG_COLOR)->SetFocus();
}

BOOL CKanjiFlasherDlg::ReadFromINI()
{
	FILE *pFile;
	pFile = _tfopen(m_strAppPath + _T("\\KanjiFlasher.ini"), _T("r"));

	if (NULL == pFile)
	{
		return FALSE;
	}
	fclose(pFile);

	TCHAR strValueFromINI[100];

	::GetPrivateProfileString(_T("Settings"), _T("DelayBetweenFlashCards"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_ctrlDurationSlider.SetPos(_ttoi(strValueFromINI));
	int nn = _ttoi(strValueFromINI)/1000;
	CString sss; sss.Format(_T("%d seconds"),nn);
	SetDlgItemText(IDC_DURATION,sss);


	::GetPrivateProfileString(_T("Settings"), _T("Transparency"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_ctrlAlertTransperancy.SetPos(280 - _ttoi(strValueFromINI));
	*(m_wndTaskbarNotifier.GetTransparency()) = _ttoi(strValueFromINI);

	CString strLabel;
	
	float transparency = static_cast<float>(_ttoi(strValueFromINI))/255;
	int nPercentage = static_cast<int>(100 - (transparency*100));
	strLabel.Format(_T("%d %% transparent"), nPercentage);
	SetDlgItemText(IDC_TRANSPARENCY,strLabel);

	::GetPrivateProfileString(_T("Settings"), _T("DelayBetweenKanjiHiragana"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_ctrlKanjiHiraSlider.SetPos(_ttoi(strValueFromINI));
	_tcscat(strValueFromINI,  _T(" seconds"));
	SetDlgItemText(IDC_DELAY_KH, strValueFromINI);

	::GetPrivateProfileString(_T("Settings"), _T("DelayBetweenHiraganaEnglish"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_ctrlHiraEngSlider.SetPos(_ttoi(strValueFromINI));
	_tcscat(strValueFromINI,  _T(" seconds"));
	SetDlgItemText(IDC_DELAY_HE, strValueFromINI);

	::GetPrivateProfileString(_T("Settings"), _T("AudioAlert"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_chkAudioAlert.SetCheck(_ttoi(strValueFromINI));

	::GetPrivateProfileString(_T("Settings"), _T("AutoResize"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_chkAutoResize.SetCheck(_ttoi(strValueFromINI));

	::GetPrivateProfileString(_T("Settings"), _T("KanjiDisplay"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_chkKanji.SetCheck(_ttoi(strValueFromINI));

	::GetPrivateProfileString(_T("Settings"), _T("HiraganaDisplay"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_chkHiragana.SetCheck(_ttoi(strValueFromINI));

	::GetPrivateProfileString(_T("Settings"), _T("EnglishDisplay"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_chkEnglish.SetCheck(_ttoi(strValueFromINI));

	/**********************************/
	//m_wndTaskbarNotifier.SetBMPStatus(m_chkKanji.GetCheck(), m_chkHiragana.GetCheck(), m_chkEnglish.GetCheck());
	/**********************************/
	 
	::GetPrivateProfileString(_T("Settings"), _T("SerialWordSelection"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));

	if(_ttoi(strValueFromINI) == 0)
	{		
		m_ctrlSerial.SetCheck(0);
		m_ctrlRandom.SetCheck(1);
	}
	else
	{
		m_ctrlSerial.SetCheck(1);
		m_ctrlRandom.SetCheck(0);
	}

	::GetPrivateProfileString(_T("Settings"), _T("KanjiFontColor"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_dKanjiColor = _ttoi(strValueFromINI);
	m_ctrlKanjiColor.SetBackgroundColor(FALSE, _ttoi(strValueFromINI));

	::GetPrivateProfileString(_T("Settings"), _T("HiraganaFontColor"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_dHiraganaColor = _ttoi(strValueFromINI);
	m_ctrlHiraganaColor.SetBackgroundColor(FALSE, _ttoi(strValueFromINI));

	::GetPrivateProfileString(_T("Settings"), _T("EnglishFontColor"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_dEnglishColor = _ttoi(strValueFromINI);
	m_ctrlEnglishColor.SetBackgroundColor(FALSE, _ttoi(strValueFromINI));

	::GetPrivateProfileString(_T("Settings"), _T("DisplayOrder"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	m_dispOrder = _ttoi(strValueFromINI);
	SetListOrder(m_dispOrder);

	m_wndTaskbarNotifier.SetTextColors(m_dKanjiColor, m_dHiraganaColor, m_dEnglishColor);

	::GetPrivateProfileString(_T("Settings"), _T("CurrentSkin"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	int kk = m_ctrlSkinSelector.FindString(-1, strValueFromINI);
	m_ctrlSkinSelector.SetCurSel(kk);

	m_wndTaskbarNotifier.SetSkin(strValueFromINI);

	OnBnClickedCheckKanji();
	OnBnClickedCheckHir();
	OnBnClickedCheckEng();

	
	::GetPrivateProfileString(_T("Settings"), _T("FromRange"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));

    int nFrom = _ttoi(strValueFromINI);

	::GetPrivateProfileString(_T("Settings"), _T("ToRange"),NULL,strValueFromINI, sizeof(strValueFromINI), m_strAppPath + _T("\\KanjiFlasher.ini"));
	int nTo   = _ttoi(strValueFromINI);

	if(nFrom == 0 || nTo == 0 || nTo > m_nExcelCount || nFrom>nTo)
	{
		nTo = m_nExcelCount;
		nFrom = 1;
	}


	m_dataMgr.SetRange(nFrom,nTo);

	CString ssss;
	ssss.Format(_T("%d"),nFrom);
	m_ctrlFromRange.SetWindowText(ssss);
	ssss.Format(_T("%d"),nTo);
	m_ctrlToRange.SetWindowText(ssss);	

	UpdateData(0);

	return TRUE;
}

BOOL CKanjiFlasherDlg::WriteToINI()
{
	CString strValueToINI;
	UpdateData(1);

	strValueToINI.Format(_T("%d"), m_ctrlDurationSlider.GetPos());
	::WritePrivateProfileString(_T("Settings"), _T("DelayBetweenFlashCards"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));


	strValueToINI.Format(_T("%d"), 280 - m_ctrlAlertTransperancy.GetPos());
	::WritePrivateProfileString(_T("Settings"), _T("Transparency"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_ctrlKanjiHiraSlider.GetPos());
	::WritePrivateProfileString(_T("Settings"), _T("DelayBetweenKanjiHiragana"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_ctrlHiraEngSlider.GetPos());
	::WritePrivateProfileString(_T("Settings"), _T("DelayBetweenHiraganaEnglish"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));


	strValueToINI.Format(_T("%d"), m_chkAudioAlert.GetCheck());
	::WritePrivateProfileString(_T("Settings"), _T("AudioAlert"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_chkAutoResize.GetCheck());
	::WritePrivateProfileString(_T("Settings"), _T("AutoResize"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));


	strValueToINI.Format(_T("%d"), m_chkKanji.GetCheck());
	::WritePrivateProfileString(_T("Settings"), _T("KanjiDisplay"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_chkHiragana.GetCheck());
	::WritePrivateProfileString(_T("Settings"), _T("HiraganaDisplay"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_chkEnglish.GetCheck());
	::WritePrivateProfileString(_T("Settings"), _T("EnglishDisplay"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_ctrlSerial.GetCheck());
	::WritePrivateProfileString(_T("Settings"), _T("SerialWordSelection"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_dKanjiColor);
	::WritePrivateProfileString(_T("Settings"), _T("KanjiFontColor"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	strValueToINI.Format(_T("%d"), m_dHiraganaColor);
	::WritePrivateProfileString(_T("Settings"), _T("HiraganaFontColor"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));	

	strValueToINI.Format(_T("%d"), m_dEnglishColor);
	::WritePrivateProfileString(_T("Settings"), _T("EnglishFontColor"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	m_dispOrder = GetListOrder();
	strValueToINI.Format(_T("%d"), m_dispOrder);
	::WritePrivateProfileString(_T("Settings"), _T("DisplayOrder"),strValueToINI,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	CString str;
	m_ctrlSkinSelector.GetLBText(m_ctrlSkinSelector.GetCurSel(),str );
	::WritePrivateProfileString(_T("Settings"), _T("CurrentSkin"),str,  m_strAppPath + _T("\\KanjiFlasher.ini"));
	
	
	m_ctrlFromRange.GetWindowText(str);
	::WritePrivateProfileString(_T("Settings"), _T("FromRange"),str,  m_strAppPath + _T("\\KanjiFlasher.ini"));

	m_ctrlToRange.GetWindowText(str);
	::WritePrivateProfileString(_T("Settings"), _T("ToRange"),str,  m_strAppPath + _T("\\KanjiFlasher.ini"));


	return TRUE;
}

BOOL CKanjiFlasherDlg::InitializeControls()
{	
	m_ctrlDurationSlider.SetPos(3000);
	SetDlgItemText(IDC_DURATION,_T("3 seconds"));


	m_ctrlAlertTransperancy.SetPos(25);
	SetDlgItemText(IDC_TRANSPARENCY,_T("0% transparent"));


	*(m_wndTaskbarNotifier.GetTransparency()) = 255;
	m_ctrlKanjiHiraSlider.SetPos(2);
	SetDlgItemText(IDC_DELAY_KH,_T("2 seconds"));
	m_ctrlHiraEngSlider.SetPos(2);	
	SetDlgItemText(IDC_DELAY_HE,_T("2 seconds"));
	m_chkAudioAlert.SetCheck(1);	
	m_chkAutoResize.SetCheck(1);	
	m_chkKanji.SetCheck(1);	
	m_chkHiragana.SetCheck(1);	
	m_chkEnglish.SetCheck(1);	
	//m_wndTaskbarNotifier.SetBMPStatus(m_chkKanji.GetCheck(), m_chkHiragana.GetCheck(), m_chkEnglish.GetCheck());
	m_ctrlSerial.SetCheck(1);
	m_ctrlRandom.SetCheck(0);	
	m_dKanjiColor = 0;
	m_ctrlKanjiColor.SetBackgroundColor(FALSE,0);	
	m_dHiraganaColor = 0;
	m_ctrlHiraganaColor.SetBackgroundColor(FALSE, 0);	
	m_dEnglishColor = 0;
	m_ctrlEnglishColor.SetBackgroundColor(FALSE, 0);
	m_wndTaskbarNotifier.SetTextColors(m_dKanjiColor, m_dHiraganaColor, m_dEnglishColor);	
	int kk = m_ctrlSkinSelector.FindString(-1, _T("default"));
	m_ctrlSkinSelector.SetCurSel(kk);
	m_wndTaskbarNotifier.SetSkin(_T("default"));

	CString ssss;
	ssss.Format(_T("%d"),m_nExcelCount);
	m_ctrlFromRange.SetWindowText(_T("1"));
	m_ctrlToRange.SetWindowText(ssss);
	m_dataMgr.SetRange(1,m_nExcelCount);


	OnBnClickedCheckKanji();
	OnBnClickedCheckHir();
	OnBnClickedCheckEng();
	UpdateData(0);
	return TRUE;
	
}


BOOL CKanjiFlasherDlg::SetPlaying(LPCTSTR sound, HMODULE hmod, DWORD fdwsound)
{
	BOOL bRet = TRUE;
	typedef BOOL (WINAPI* lpfnPlay)(LPCTSTR sound, HMODULE hmod, DWORD fdwsound);
	static HMODULE hUserDll;
	if (hUserDll == NULL)
	{
		hUserDll = ::LoadLibrary(_T("winmm.dll"));
	}
	// Check that "USER32.dll" library has been loaded successfully...
	if ( hUserDll )
	{
		lpfnPlay pFnPlay = NULL;
		pFnPlay = (lpfnPlay)GetProcAddress(hUserDll, "PlaySoundW");

		if (pFnPlay)
			bRet = pFnPlay(sound,NULL,fdwsound);
		else 
			bRet = FALSE;

	} //if( hUserDll )


	return bRet;
}



void CKanjiFlasherDlg::OnPlayPause()
{
	// TODO: Add your command handler code here
	CString str;
	m_trayMenu.GetMenuString(ID_PLAYPAUSE, str, NULL);

	if(0 == str.Compare(_T("Pause")))
	{
		m_wndTaskbarNotifier.HideWindow();
		m_trayMenu.ModifyMenu(ID_PLAYPAUSE, MF_BYCOMMAND,ID_PLAYPAUSE , _T("Resume"));
		
		//Change the icon in the system tray to the paused state
		DWORD dwMessage = NIM_DELETE;
		Shell_NotifyIcon(dwMessage, &m_iconData);  

		m_iconData.cbSize = sizeof(NOTIFYICONDATA);
		m_iconData.hWnd = this->GetSafeHwnd();
		m_iconData.uID = IDI_ICON2;   //ICON RESOURCE ID

		m_iconData.uFlags = NIF_MESSAGE|NIF_ICON;
		m_iconData.uCallbackMessage = MYWM_NOTIFYICON;
		m_iconData.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP; 
		m_iconData.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDI_ICON2));

		CString strTip(_T("KanjiFlasher Paused"));
		lstrcpyn(m_iconData.szTip, (LPCTSTR)strTip, sizeof(m_iconData.szTip));

		dwMessage = NIM_ADD;
		Shell_NotifyIcon(dwMessage, &m_iconData);  
	}
	else
	{
		m_trayMenu.ModifyMenu(ID_PLAYPAUSE, MF_BYCOMMAND,ID_PLAYPAUSE , _T("Pause"));

		//Change the icon in the system tray to the resumed state
		DWORD dwMessage = NIM_DELETE;
		Shell_NotifyIcon(dwMessage, &m_iconData); 

		m_iconData.cbSize = sizeof(NOTIFYICONDATA);
		m_iconData.hWnd = this->GetSafeHwnd();
		m_iconData.uID = IDR_MAINFRAME;   //ICON RESOURCE ID

		m_iconData.uFlags = NIF_MESSAGE|NIF_ICON;
		m_iconData.uCallbackMessage = MYWM_NOTIFYICON;
		m_iconData.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP; 
		m_iconData.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDR_MAINFRAME));

		CString strTip(_T("KanjiFlasher"));
		lstrcpyn(m_iconData.szTip, (LPCTSTR)strTip, sizeof(m_iconData.szTip));

		dwMessage = NIM_ADD;
		Shell_NotifyIcon(dwMessage, &m_iconData);  
	}
}

LRESULT CKanjiFlasherDlg::OnDispWindowRightClicked(WPARAM wParam,LPARAM lParam)
{
	CPoint pt;    
	GetCursorPos(&pt);
	m_trayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, this);
	return 0;
}

void CKanjiFlasherDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CString strDisplay;
	
	if((CSliderCtrl*) pScrollBar == &m_ctrlDurationSlider)
	{
		int nPosition =m_ctrlDurationSlider.GetPos();
		strDisplay.Format(_T("%d seconds"),(nPosition/1000));
		SetDlgItemText(IDC_DURATION, strDisplay);
	}
	else if((CSliderCtrl*) pScrollBar == &m_ctrlAlertTransperancy)
	{
		int nPosition =m_ctrlAlertTransperancy.GetPos();
		float fTransparency = static_cast<float> (280 - nPosition)/255;
		int nPercentage = static_cast<int>(100 - (fTransparency*100));
		strDisplay.Format(_T("%d %% transparent"),nPercentage);
		SetDlgItemText(IDC_TRANSPARENCY, strDisplay);
	}
	else if((CSliderCtrl*) pScrollBar == &m_ctrlKanjiHiraSlider)
	{
		int nPosition =m_ctrlKanjiHiraSlider.GetPos();
		strDisplay.Format(_T("%d seconds"),nPosition);
		SetDlgItemText(IDC_DELAY_KH, strDisplay);
	}
	else if((CSliderCtrl*) pScrollBar == &m_ctrlHiraEngSlider)
	{
		int nPosition =m_ctrlHiraEngSlider.GetPos();
		strDisplay.Format(_T("%d seconds"),(nPosition));
		SetDlgItemText(IDC_DELAY_HE, strDisplay);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CKanjiFlasherDlg::OnLvnBeginrdragList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	m_nDragIndex = ((NM_LISTVIEW *)pNMHDR)->iItem;

	m_bDragging = TRUE;
	SetCapture ();
}

void CKanjiFlasherDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		CRect r;
		m_List.GetWindowRect(&r);
		this->ScreenToClient(&r);		
		point.Offset(-1*r.left, -1*r.top);
		point.y = 2;
		m_List.SetItemPosition(m_nDragIndex, point);
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CKanjiFlasherDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		// release mouse capture
		VERIFY (::ReleaseCapture ());
		m_bDragging = FALSE;
		m_List.Arrange(LVA_SNAPTOGRID |LVA_ALIGNTOP);
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CKanjiFlasherDlg::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	m_nDragIndex = ((NM_LISTVIEW *)pNMHDR)->iItem;

	m_bDragging = TRUE;
	SetCapture ();
}

int CKanjiFlasherDlg::GetListOrder(void)
{
	CPoint p1,p2,p3;
	m_List.GetItemPosition(0,&p1);
	m_List.GetItemPosition(1,&p2);
	m_List.GetItemPosition(2,&p3);

	if(VerifyOrder(p1,p2,p3))
		return ORDER_KHE;
	if(VerifyOrder(p1,p3,p2))
		return ORDER_KEH;
	if(VerifyOrder(p2,p1,p3))
		return ORDER_HKE;
	if(VerifyOrder(p2,p3,p1))
		return ORDER_HEK;
	if(VerifyOrder(p3,p1,p2))
		return ORDER_EKH;	
	if(VerifyOrder(p3,p2,p1))
		return ORDER_EHK;

	return 0;
}

BOOL CKanjiFlasherDlg::SetListOrder(int dispOrder)
{
	CPoint p1,p2,p3;
	p1.x = 10; p1.y = 0;
	p2.x = 40; p2.y = 0;
	p3.x = 50; p3.y = 0;
	switch(dispOrder)
	{
		case ORDER_KHE:
			m_List.SetItemPosition(0,p1);
			m_List.SetItemPosition(1,p2);
			m_List.SetItemPosition(2,p3);
			break;
		case ORDER_KEH:
			m_List.SetItemPosition(0,p1);
			m_List.SetItemPosition(2,p2);
			m_List.SetItemPosition(1,p3);
			break;
		case ORDER_HKE:
			m_List.SetItemPosition(1,p1);
			m_List.SetItemPosition(0,p2);
			m_List.SetItemPosition(2,p3);
			break;
		case ORDER_HEK:
			m_List.SetItemPosition(1,p1);
			m_List.SetItemPosition(2,p2);
			m_List.SetItemPosition(0,p3);
			break;
		case ORDER_EKH:
			m_List.SetItemPosition(2,p1);
			m_List.SetItemPosition(0,p2);
			m_List.SetItemPosition(1,p3);
			break;
		case ORDER_EHK:
			m_List.SetItemPosition(2,p1);
			m_List.SetItemPosition(1,p2);
			m_List.SetItemPosition(0,p3);
			break;		

	}

	m_List.Arrange(LVA_SNAPTOGRID |LVA_ALIGNTOP);
	return 0;
}

BOOL CKanjiFlasherDlg::VerifyOrder(CPoint& pp1,CPoint& pp2, CPoint& pp3)
{
	if(pp1.x < pp2.x && pp2.x < pp3.x)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CKanjiFlasherDlg::ShowDisplayWindow(void)
{
	CEntry c;

	if(m_ctrlSerial.GetCheck() == TRUE)
	{
		c = m_dataMgr.GetNext();				
	}	
	else
	{
		c = m_dataMgr.GetRandomEntry();
	}

	if(m_chkAudioAlert.GetCheck() == TRUE)
	{
		SetPlaying(MAKEINTRESOURCE(IDR_WAVE3), AfxGetResourceHandle(), SND_SYNC|SND_RESOURCE);
	}
	if(m_chkAutoResize.GetCheck() == TRUE)
	{
		*(m_wndTaskbarNotifier.GetAutoResizeFlag()) = TRUE;
	}
	else
	{
		*(m_wndTaskbarNotifier.GetAutoResizeFlag()) = FALSE;
	}


	*(m_wndTaskbarNotifier.GetDisplayOrder()) = m_dispOrder;

	CString str;
	m_ctrlSkinSelector.GetLBText(m_ctrlSkinSelector.GetCurSel(),str );
	m_wndTaskbarNotifier.SetSkin(str);
	m_wndTaskbarNotifier.SetAllText(m_chkKanji.GetCheck() ? c.strKanji : _T(""), 
		m_chkHiragana.GetCheck() ? c.strHgna : _T(""),
		m_chkEnglish.GetCheck() ? c.strEngMeaning : _T(""));
	m_wndTaskbarNotifier.SetTimeDelays(m_ctrlKanjiHiraSlider.GetPos()*1000, m_ctrlHiraEngSlider.GetPos()*1000);
	m_wndTaskbarNotifier.Show();	

	return 0;
}
