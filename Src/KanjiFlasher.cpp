// KanjiFlasher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KanjiFlasher.h"
#include "KanjiFlasherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKanjiFlasherApp

BEGIN_MESSAGE_MAP(CKanjiFlasherApp, CWinApp)
	//{{AFX_MSG_MAP(CKanjiFlasherApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanjiFlasherApp construction

CKanjiFlasherApp::CKanjiFlasherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CKanjiFlasherApp object

CKanjiFlasherApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CKanjiFlasherApp initialization

BOOL CKanjiFlasherApp::InitInstance()
{
	if ( OpenEvent( EVENT_ALL_ACCESS, FALSE, _T("KanjiFlasher.exe" ) ))
	{
		AfxMessageBox(_T("Another instance is already running"));
		return( FALSE );                      // quit if same process is running
	}
	else
		CreateEvent( NULL, FALSE, FALSE, _T("KanjiFlasher.exe" ));    // register new process

	AfxEnableControlContainer();
	AfxOleInit();
	AfxInitRichEdit();
	

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CKanjiFlasherDlg dlg;
	m_pMainWnd = &dlg;	

	int nResponse = dlg.DoModal();
	
	




	//if (nResponse == IDOK)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with OK
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with Cancel
	//}

	 //Since the dialog has been closed, return FALSE so that we exit the
	 // application, rather than start the application's message pump.
	return FALSE;
}
