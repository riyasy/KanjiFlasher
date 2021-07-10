// WndNotifier.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WndNotifier.h"
#include <crtdbg.h>
#include ".\wndnotifier.h"


// CWndNotifier dialog

IMPLEMENT_DYNAMIC(CWndNotifier, CDialog)
CWndNotifier::CWndNotifier(CWnd* pParent /*=NULL*/)
	: CDialog(CWndNotifier::IDD, pParent),
	m_bLButtonDown(0),
	m_bMouseOverClose(0),
	m_bDisplayContinously(FALSE),
	m_bMouseOverResizer(0),
	m_bMouseOverPushPin(0),
	m_bResizing(0),
	m_nTransparency(127),
	m_nDispOrder(0),
	m_dwTime_1_to_2_(0),
	m_dwTime_2_to_3_(0),
	m_dwTimeToLive(2000),
	m_bAutoResize(TRUE),
	m_nCurrentTransparency(0),
	m_strSkinName(_T("default"))
{
	m_dispItemKanji.Initialize(_T("Arial"),200,RGB(0,0,0),_T("Kanji"));
	m_dispItemHiragana.Initialize(_T("Arial"),170,RGB(0,0,0),_T("Hiragana"));
	m_dispItemEnglish.Initialize(_T("Arial"),100,RGB(0,0,0),_T("English"));
}

CWndNotifier::~CWndNotifier()
{
}

void CWndNotifier::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWndNotifier, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CWndNotifier message handlers

int CWndNotifier::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//ResizeRects();

	GetClientRect(&m_rectWnd);
	ClientToScreen(&m_rectWnd);
	
	CRect rcDesktop;
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcDesktop,0);
	MoveWindow(m_rectWnd.left+(rcDesktop.right - m_rectWnd.right),0,m_rectWnd.Width(),m_rectWnd.Height(),FALSE);

	m_iconPushPin1 = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDI_PUSHPIN1),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	m_iconPushPin2 = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDI_PUSHPIN2),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	m_iconGrip = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDI_GRIP),IMAGE_ICON,8,23,LR_DEFAULTCOLOR);

	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, 
		GWL_EXSTYLE) |  WS_EX_LAYERED );

	//::SetClassLong(m_hWnd, GCL_STYLE, ::GetClassLong(m_hWnd, GCL_STYLE) | CS_DROPSHADOW);

	SetTransparency(m_nTransparency);
	return 0;
}

void CWndNotifier::OnDestroy()
{
	DeleteObject(m_iconPushPin1);
	DeleteObject(m_iconPushPin2);
	DeleteObject(m_iconGrip);
	CDialog::OnDestroy();
}


void CWndNotifier::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMaxTrackSize.y=lpMMI->ptMaxSize.y=60;
	lpMMI->ptMinTrackSize.y = 60;
	lpMMI->ptMaxTrackSize.x=lpMMI->ptMaxSize.x=1024;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CWndNotifier::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLButtonDown = TRUE;
	//_RPTF0(_CRT_WARN, "m_bLButtonDown = TRUE\n");

	GetCursorPos(&m_pointLastLButtonDown);
	GetClientRect(&m_rectWnd);
	ClientToScreen(&m_rectWnd);

	if(m_rectResizer.PtInRect(point))
	{		
		SetCapture();	
		if(m_bAutoResize == FALSE)
			m_bResizing = TRUE;
	}	
	else if(m_rectClose.PtInRect(point))
	{
		InvalidateRect(m_rectClose, TRUE);
	}
	else if(m_rectPushPin.PtInRect(point))
	{

	}
	else
	{
		SetCapture();
	}


	CDialog::OnLButtonDown(nFlags, point);
}

void CWndNotifier::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLButtonDown  = FALSE;
	//_RPTF0(_CRT_WARN, "m_bLButtonDown  = FALSE\n");
	m_bResizing = FALSE;
	ReleaseCapture();

	if(m_rectPushPin.PtInRect(point))
	{
		m_bDisplayContinously = !m_bDisplayContinously;
		InvalidateRect(m_rectPushPin, TRUE);
	}
	else if(m_bMouseOverClose)
	{
		m_bMouseOverClose = FALSE;
		HideWindow();
	}
	else
	{
		Stick(TRUE);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CWndNotifier::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//::PostMessage(::GetParent(m_hWnd),WM_DISPWINDOWRIGHTCLICKED,0,0);
	::PostMessage(::GetParent(m_hWnd),WM_DISPWINDOWRIGHTCLICKED, NULL, NULL);
	CDialog::OnRButtonUp(nFlags, point);
}

void CWndNotifier::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT t_MouseEvent;
	t_MouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
	t_MouseEvent.dwFlags     = TME_LEAVE | TME_HOVER;
	t_MouseEvent.hwndTrack   = m_hWnd;
	t_MouseEvent.dwHoverTime = 1;

	::_TrackMouseEvent(&t_MouseEvent);

	if(m_bLButtonDown == TRUE && m_bResizing == TRUE)
	{
		CPoint pp = point;
		ClientToScreen(&pp);
		int nNewWidth = m_rectWnd.Width()+pp.x - m_pointLastLButtonDown.x;

		CRect rectDesktop;
		::SystemParametersInfo(SPI_GETWORKAREA,0,&rectDesktop,0);

		if(nNewWidth > 100 && nNewWidth < rectDesktop.right)
		{
			MoveWindow(m_rectWnd.left, m_rectWnd.top, nNewWidth,m_rectWnd.Height(),TRUE);
			ResizeRects();
		}

	}
	else if (m_rectClose.PtInRect(point) == FALSE &&
		m_rectResizer.PtInRect(point) == FALSE &&
		m_rectPushPin.PtInRect(point) == FALSE &&
		m_bLButtonDown == TRUE)
	{
		CPoint pp = point;
		ClientToScreen(&pp);	
		MoveWindow(m_rectWnd.left+(pp.x - m_pointLastLButtonDown.x), m_rectWnd.top+(pp.y - m_pointLastLButtonDown.y), m_rectWnd.Width(),m_rectWnd.Height(),TRUE);
	}
	else
	{
		ValidateRectAndPoint(point,m_rectClose, m_bMouseOverClose);
		ValidateRectAndPoint(point,m_rectPushPin, m_bMouseOverPushPin);
		ValidateRectAndPoint(point,m_rectResizer, m_bMouseOverResizer);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CWndNotifier::ValidateRectAndPoint(CPoint& point, CRect& rect, BOOL& condition)
{
	if (rect.PtInRect(point))
	{
		if(condition == FALSE)
		{
			condition = TRUE;			
			InvalidateRect(rect, TRUE);
		}
	}
	else
	{
		if(condition == TRUE)
		{
			condition = FALSE;			
			InvalidateRect(rect, TRUE);
		}
	}
}

LRESULT CWndNotifier::OnMouseHover(WPARAM w, LPARAM l)
{
	m_nCurrentTransparency = 255;
	SetTransparency(m_nCurrentTransparency);
	StopTimer(IDT_WAIT);
	StopTimer(IDT_APPEAR);
	StopTimer(IDT_DISAPPEAR);
	return 0;
}

LRESULT CWndNotifier::OnMouseLeave(WPARAM w, LPARAM l)
{
	m_nCurrentTransparency = m_nTransparency;
	SetTransparency(m_nCurrentTransparency);
	StartTimer(IDT_WAIT,m_dwTimeToLive,NULL);
	return 0;
}

void CWndNotifier::ResizeRects()
{

	InvalidateRect(m_rectClose);
	InvalidateRect(m_rectPushPin);
	InvalidateRect(m_rectResizer);

	InvalidateRect(m_dispItemKanji.m_rectText);
	InvalidateRect(m_dispItemKanji.m_rectText);
	InvalidateRect(m_dispItemKanji.m_rectText);

	CRect r;
	GetClientRect(r);
	int nSkinHeight = r.Height();
	int nSkinWidth = r.Width();

	int margin = 20+37;
	int a1,a2,a3;

	if(!m_bAutoResize)
	{
		//a1 = ((nSkinWidth-margin)*1*m_dispItemKanji.m_bEnabled)/(1*m_dispItemKanji.m_bEnabled + 1*m_dispItemHiragana.m_bEnabled + 2*m_dispItemEnglish.m_bEnabled);
		//a2 = ((nSkinWidth-margin)*1*m_dispItemHiragana.m_bEnabled)/(1*m_dispItemKanji.m_bEnabled + 1*m_dispItemHiragana.m_bEnabled + 2*m_dispItemEnglish.m_bEnabled);
		//a3 = ((nSkinWidth-margin)*2*m_dispItemEnglish.m_bEnabled)/(1*m_dispItemKanji.m_bEnabled + 1*m_dispItemHiragana.m_bEnabled + 2*m_dispItemEnglish.m_bEnabled);

		int totalSize = (m_dispItemKanji.m_sizeText.cx*m_dispItemKanji.m_bEnabled) +
						(m_dispItemHiragana.m_sizeText.cx*m_dispItemHiragana.m_bEnabled) + 
						(m_dispItemEnglish.m_sizeText.cx*m_dispItemEnglish.m_bEnabled);


		a1 = (((nSkinWidth-margin)*m_dispItemKanji.m_sizeText.cx*m_dispItemKanji.m_bEnabled)/totalSize);
		a2 = (((nSkinWidth-margin)*m_dispItemHiragana.m_sizeText.cx*m_dispItemHiragana.m_bEnabled)/totalSize);
		a3 = (((nSkinWidth-margin)*m_dispItemEnglish.m_sizeText.cx*m_dispItemEnglish.m_bEnabled)/totalSize);

	}
	else
	{
		a1 = m_dispItemKanji.m_sizeText.cx + 20*m_dispItemKanji.m_bEnabled;
		a2 = m_dispItemHiragana.m_sizeText.cx + 20*m_dispItemHiragana.m_bEnabled;
		a3 = m_dispItemEnglish.m_sizeText.cx + 20*m_dispItemEnglish.m_bEnabled;
		nSkinWidth = a1+a2+a3+margin;
		ClientToScreen(&r);
		static int first;
		if(first == 0)
		{
			CRect rcDesktop;
			::SystemParametersInfo(SPI_GETWORKAREA,0,&rcDesktop,0);
			MoveWindow(r.left+(rcDesktop.right - r.right)+(r.Width()-nSkinWidth), r.top, nSkinWidth, r.Height(), TRUE); 
			first++;
		}
		else
			MoveWindow(r.left, r.top, nSkinWidth, r.Height(), TRUE);
		Stick(TRUE);
	}

	m_dispItemKanji.m_rectText.SetRect(0, 5, a1 , nSkinHeight-5);
	m_dispItemHiragana.m_rectText.SetRect(0, 5, a2, nSkinHeight-5);
	m_dispItemEnglish.m_rectText.SetRect(0, 5, a3 , nSkinHeight-5);

	int offset = 5;
	if(GetMappedStatus(1)->m_bEnabled)
	{
		GetMappedStatus(1)->m_rectText.OffsetRect(offset,0);
		offset = GetMappedStatus(1)->m_rectText.right + 5;
	}
	if(GetMappedStatus(2)->m_bEnabled)
	{
		GetMappedStatus(2)->m_rectText.OffsetRect(offset,0);
		offset = GetMappedStatus(2)->m_rectText.right + 5;
	}
	if(GetMappedStatus(3)->m_bEnabled)
	{
		GetMappedStatus(3)->m_rectText.OffsetRect(offset,0);
	}

	m_rectClose.SetRect(nSkinWidth - 17, 6, nSkinWidth-3 , 21);
	m_rectPushPin.SetRect(nSkinWidth - 37, 6, nSkinWidth-19 , 21);
	m_rectResizer.SetRect(nSkinWidth - 11, 22, nSkinWidth-3 , 45);

	RedrawWindow();

}




BOOL CWndNotifier::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(r);
	int nSkinHeight = r.Height();
	int nSkinWidth = r.Width();
	CPaintDC dc(this);

	CBitmap bmpOrig, bmpScaled;
	BITMAP bbOrig;
	CDC memDCOrig, memDCScaled;
	
	memDCOrig.CreateCompatibleDC(&dc);
	memDCScaled.CreateCompatibleDC(&dc);

	if(m_strSkinName.Compare(_T("default")) == 0)
		bmpOrig.LoadBitmap(IDB_SKIN_BLUE);
	else if(m_strSkinName.Compare(_T("woody grey")) == 0)
		bmpOrig.LoadBitmap(IDB_SKIN_BROWN);
	else
		bmpOrig.LoadBitmap(IDB_SKIN_BLUE);

	CBitmap* pBmpOld1 = memDCOrig.SelectObject(&bmpOrig);
	GetObject(bmpOrig.GetSafeHandle(), sizeof(bbOrig), &bbOrig);

	bmpScaled.CreateCompatibleBitmap(&memDCOrig, nSkinWidth, nSkinHeight);	
	CBitmap* pBmpOld2 = memDCScaled.SelectObject(&bmpScaled);

	memDCScaled.StretchBlt(0,0,nSkinWidth,nSkinHeight,&memDCOrig,0,0,bbOrig.bmWidth,bbOrig.bmHeight,SRCCOPY);

	//*************************************************
	memDCScaled.SetBkMode(TRANSPARENT);

	m_dispItemKanji.DrawDisplayItem(&memDCScaled);
	m_dispItemHiragana.DrawDisplayItem(&memDCScaled);
	m_dispItemEnglish.DrawDisplayItem(&memDCScaled);

	if(m_bAutoResize == FALSE)
		memDCScaled.DrawState(m_rectResizer.TopLeft(), m_rectResizer.Size(), m_iconGrip, DSS_NORMAL,
		(HBRUSH)NULL);

	if(m_bDisplayContinously)
		memDCScaled.DrawState(m_rectPushPin.TopLeft(), m_rectPushPin.Size(), m_iconPushPin1, DSS_NORMAL,
		(HBRUSH)NULL);
	else
		memDCScaled.DrawState(m_rectPushPin.TopLeft(), m_rectPushPin.Size(), m_iconPushPin2, DSS_NORMAL,
		(HBRUSH)NULL);

	if(m_bMouseOverPushPin)
		memDCScaled.DrawFocusRect(m_rectPushPin);

	if(m_bMouseOverResizer && m_bAutoResize == FALSE)
		memDCScaled.DrawFocusRect(m_rectResizer);

	if(m_bMouseOverClose && m_bLButtonDown)
		memDCScaled.DrawEdge(m_rectClose, EDGE_SUNKEN, BF_RECT | BF_SOFT);


	CBrush objBrush; objBrush.CreateStockObject(HOLLOW_BRUSH);	
	CBrush *pOldBrush = memDCScaled.SelectObject(&objBrush);

	int wid = 3;
	CPen objPen(PS_SOLID, 1, RGB(0,0,0));
	CPen *pOldPen = memDCScaled.SelectObject(&objPen);

	memDCScaled.MoveTo(m_rectClose.left+wid+1,m_rectClose.top+wid+1);
	memDCScaled.LineTo(m_rectClose.right-wid , m_rectClose.bottom-wid);
	memDCScaled.MoveTo(m_rectClose.right- wid-1,m_rectClose.top+wid+1);
	memDCScaled.LineTo(m_rectClose.left+wid , m_rectClose.bottom-wid);

	//memDCScaled.Rectangle(m_dispItemKanji.m_rectText);
	//memDCScaled.Rectangle(m_dispItemHiragana.m_rectText);
	//memDCScaled.Rectangle(m_dispItemEnglish.m_rectText);

	if(m_bMouseOverClose && m_bLButtonDown == FALSE)
	{
		memDCScaled.Rectangle(m_rectClose);
	}

	memDCScaled.SelectObject(pOldBrush);
	memDCScaled.SelectObject(pOldPen);
	objBrush.DeleteObject();
	objPen.DeleteObject();

	//************************************************

	dc.BitBlt(0,0,nSkinWidth,nSkinHeight,&memDCScaled,0,0,SRCCOPY);

	memDCOrig.SelectObject(pBmpOld1);
	memDCScaled.SelectObject(pBmpOld2);

	memDCOrig.DeleteDC();
	memDCScaled.DeleteDC();
	bmpOrig.DeleteObject();
	bmpScaled.DeleteObject();
	

	return 1;
}


void CWndNotifier::OnPaint()
{

}

void CWndNotifier::SetTransparency(int nTransparency)
{
	::SetLayeredWindowAttributes(m_hWnd, RGB(255,0,255), nTransparency, LWA_ALPHA|LWA_COLORKEY);
	_RPTF1(_CRT_WARN, "transparency = %d\n",nTransparency);
}

void CWndNotifier::Show()
{
	ResizeRects();
	
	if(!m_bDisplayContinously)
	{
		m_nCurrentTransparency = 0;
		SetTransparency(m_nCurrentTransparency);
		ShowWindow(SW_SHOWNOACTIVATE);
		RedrawWindow();	
		StartTimer(IDT_APPEAR, 5, NULL);
	}
	else
	{
		ShowWindow(SW_SHOWNOACTIVATE);
	}
	StartWaiting();

}

void CWndNotifier::Hide()
{
	if(!m_bDisplayContinously)
	{
		StartTimer(IDT_DISAPPEAR, 5, NULL);
	}
	else
	{
		//HideWindow();
		::PostMessage(::GetParent(m_hWnd),WM_HIDDEN, NULL, NULL);
	}

}

void CWndNotifier::HideWindow()
{
	StopTimer(IDT_APPEAR);
	StopTimer(IDT_DISAPPEAR);
	StopTimer(IDT_WAIT);
	StopTimer(IDT_FIELD2);
	StopTimer(IDT_FIELD3);

	ShowWindow(SW_HIDE);
	//PostMessageToParent
	::PostMessage(::GetParent(m_hWnd),WM_HIDDEN, NULL, NULL);
}

void CWndNotifier::SetAllText(LPCTSTR szCaption1, LPCTSTR szCaption2, LPCTSTR szCaption3)
{
	m_dispItemKanji.SetText(szCaption1, this);
	m_dispItemHiragana.SetText(szCaption2, this);
	m_dispItemEnglish.SetText(szCaption3, this);

	if(m_dispItemKanji.m_bEnabled == FALSE && 
		m_dispItemHiragana.m_bEnabled == FALSE &&
		m_dispItemEnglish.m_bEnabled == FALSE)
	{
		m_dispItemEnglish.SetText(_T("Please select atleast one field."),this);
	}

	//ResizeRects();
}

void CWndNotifier::SetTimeDelays(DWORD dwTime_1_to_2_, DWORD dwTime_2_to_3_)
{
	m_dispItemKanji.m_bDisplay = FALSE;
	m_dispItemHiragana.m_bDisplay = FALSE;
	m_dispItemEnglish.m_bDisplay = FALSE;

	m_dwTime_1_to_2_ = dwTime_1_to_2_;
	m_dwTime_2_to_3_ = dwTime_2_to_3_;

	int nState = (GetMappedStatus(1)->m_bEnabled<<2)|(GetMappedStatus(2)->m_bEnabled<<1)|(GetMappedStatus(3)->m_bEnabled);

	m_dwTimeToLive = 3000;

	switch(nState)
	{
	case 1:
		GetMappedStatus(3)->m_bDisplay = TRUE;
		break;
	case 2:
		GetMappedStatus(2)->m_bDisplay = TRUE;
		break;
	case 3:
		GetMappedStatus(2)->m_bDisplay = TRUE;
		if(m_dwTime_2_to_3_ == 0)
		{
			GetMappedStatus(3)->m_bDisplay = TRUE;
		}
		m_dwTimeToLive+= m_dwTime_2_to_3_ ;
		break;
	case 4:
		GetMappedStatus(1)->m_bDisplay = TRUE;
		break;
	case 5:
		GetMappedStatus(1)->m_bDisplay = TRUE;
		if(m_dwTime_2_to_3_ == 0 && m_dwTime_1_to_2_ == 0)
		{
			GetMappedStatus(3)->m_bDisplay = TRUE;
		}
		m_dwTimeToLive+=m_dwTime_2_to_3_;
		m_dwTimeToLive+=m_dwTime_1_to_2_;
		break;
	case 6:
		GetMappedStatus(1)->m_bDisplay = TRUE;
		if(m_dwTime_1_to_2_ == 0)
		{
			GetMappedStatus(2)->m_bDisplay = TRUE;
		}
		m_dwTimeToLive+=m_dwTime_1_to_2_;
		break;
	case 7:
		GetMappedStatus(1)->m_bDisplay = TRUE;
		if(m_dwTime_1_to_2_ == 0)
		{
			GetMappedStatus(2)->m_bDisplay = TRUE;
		}
		if(m_dwTime_2_to_3_ == 0)
		{
			GetMappedStatus(3)->m_bDisplay = TRUE;
		}
		m_dwTimeToLive+=m_dwTime_2_to_3_;
		m_dwTimeToLive+=m_dwTime_1_to_2_;
		break;

	}
	if(m_dwTimeToLive == 3000){ m_dwTimeToLive = 4000;	}

}
	
CDisplayItem* CWndNotifier::GetMappedStatus(int nIndex)
{
	switch(m_nDispOrder)
	{
	case ORDER_KHE:
		if(nIndex == 1) return &m_dispItemKanji;
		if(nIndex == 2) return &m_dispItemHiragana;
		if(nIndex == 3) return &m_dispItemEnglish;
		break;
	case ORDER_KEH:
		if(nIndex == 1) return &m_dispItemKanji;
		if(nIndex == 3) return &m_dispItemHiragana;
		if(nIndex == 2) return &m_dispItemEnglish;
		break;
	case ORDER_HKE:
		if(nIndex == 2) return &m_dispItemKanji;
		if(nIndex == 1) return &m_dispItemHiragana;
		if(nIndex == 3) return &m_dispItemEnglish;
		break;
	case ORDER_HEK:
		if(nIndex == 3) return &m_dispItemKanji;
		if(nIndex == 1) return &m_dispItemHiragana;
		if(nIndex == 2) return &m_dispItemEnglish;
		break;
	case ORDER_EKH:
		if(nIndex == 2) return &m_dispItemKanji;
		if(nIndex == 3) return &m_dispItemHiragana;
		if(nIndex == 1) return &m_dispItemEnglish;
		break;
	case ORDER_EHK:
		if(nIndex == 3) return &m_dispItemKanji;
		if(nIndex == 2) return &m_dispItemHiragana;
		if(nIndex == 1) return &m_dispItemEnglish;
		break;		

	}
	return NULL;
}

void CWndNotifier::StartWaiting()
{
	int nState = (m_dispItemKanji.m_bEnabled<<2) | (m_dispItemHiragana.m_bEnabled<<1) | (m_dispItemEnglish.m_bEnabled);

	switch(nState)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		if( m_dwTime_2_to_3_ != 0)
			StartTimer(IDT_FIELD3, m_dwTime_2_to_3_, NULL);
		break;
	case 4:
		break;
	case 5:
		if((m_dwTime_2_to_3_ + m_dwTime_1_to_2_) != 0)
			StartTimer(IDT_FIELD3, m_dwTime_2_to_3_+ m_dwTime_1_to_2_, NULL);
		break;
	case 6:
		if(m_dwTime_1_to_2_ != 0)
			StartTimer(IDT_FIELD2, m_dwTime_1_to_2_, NULL);
		break;
	case 7:
		if(m_dwTime_1_to_2_ != 0)
			StartTimer(IDT_FIELD2, m_dwTime_1_to_2_ , NULL);
		if(m_dwTime_2_to_3_ + m_dwTime_1_to_2_ != 0)
			StartTimer(IDT_FIELD3, m_dwTime_2_to_3_ + m_dwTime_1_to_2_ , NULL);
		break;

	}

	StartTimer(IDT_WAIT, m_dwTimeToLive, NULL);
}


void CWndNotifier::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{

		case IDT_FIELD2:
			GetMappedStatus(2)->m_bDisplay = TRUE;
			StopTimer(IDT_FIELD2);
			RedrawWindow();
			break;

		case IDT_FIELD3:
			GetMappedStatus(3)->m_bDisplay = TRUE;
			StopTimer(IDT_FIELD3);
			RedrawWindow();
			break;
		case IDT_WAIT:
			Hide();
			StopTimer(IDT_WAIT);
			break;
		case IDT_APPEAR:
			SetTransparency(m_nCurrentTransparency);
			m_nCurrentTransparency++;
			if(m_nCurrentTransparency >= m_nTransparency)
			{
				StopTimer(IDT_APPEAR);
			}
			break;
		case IDT_DISAPPEAR:
			SetTransparency(m_nCurrentTransparency);
			m_nCurrentTransparency--;
			if(m_nCurrentTransparency <= 0)
			{
				StopTimer(IDT_DISAPPEAR);
				HideWindow();
			}
			break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CWndNotifier::SetSkin(LPCTSTR skinName)
{
	m_strSkinName = skinName;
}

void CWndNotifier::SetTextColors(COLORREF m_dKanjiColor, COLORREF m_dHiraganaColor, COLORREF m_dEnglishColor)
{
	m_dispItemKanji.m_crf = m_dKanjiColor;
	m_dispItemHiragana.m_crf = m_dHiraganaColor;
	m_dispItemEnglish.m_crf = m_dEnglishColor;
}

void CWndNotifier::Stick(BOOL bGradual)
{
	CRect rectDesktop;
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rectDesktop,0);
	CRect rectWnd;
	GetClientRect(&rectWnd);
	ClientToScreen(&rectWnd);
	int newX = rectWnd.left;
	int newY = rectWnd.top;

	if(rectWnd.left < rectDesktop.left || (rectWnd.left - rectDesktop.left) <= 15)
		newX = rectDesktop.left;
	if(rectWnd.right > rectDesktop.right || (rectDesktop.right - rectWnd.right) <= 15)
		newX = newX - (rectWnd.right - rectDesktop.right);	

	if(rectWnd.top < rectDesktop.top || (rectWnd.top - rectDesktop.top) <= 15)
		newY = rectDesktop.top;
	if(rectWnd.bottom > rectDesktop.bottom || (rectDesktop.bottom - rectWnd.bottom) <= 15)
		newY = newY - (rectWnd.bottom - rectDesktop.bottom);	

	MoveWindow(newX,newY,rectWnd.Width(),rectWnd.Height(),FALSE);
}



