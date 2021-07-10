#pragma once

#include "DisplayItem.h"

#define ORDER_KHE 0
#define ORDER_KEH 1
#define ORDER_HKE 2
#define ORDER_HEK 3
#define ORDER_EKH 4
#define ORDER_EHK 5

#define IDT_FIELD2 1
#define IDT_FIELD3 2
#define IDT_WAIT 3
#define IDT_APPEAR 4
#define IDT_DISAPPEAR 5

#define WM_DISPWINDOWRIGHTCLICKED	WM_USER+126
#define WM_HIDDEN					WM_USER+124

// CWndNotifier dialog

class CWndNotifier : public CDialog
{
public:
	CWndNotifier(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWndNotifier();

	void Show();		
	void SetTextColors(COLORREF m_dKanjiColor, COLORREF m_dHiraganaColor, COLORREF m_dEnglishColor);
	void SetAllText(LPCTSTR szCaption1, LPCTSTR szCaption2, LPCTSTR szCaption3);
	void SetTimeDelays(DWORD dwTime_1_to_2_ = 0, DWORD dwTime_2_to_3_ = 0);	
	void SetSkin(LPCTSTR skinName);
	void HideWindow();
	int* GetDisplayOrder()	{		return &m_nDispOrder;	}
	int* GetTransparency()	{		return &m_nTransparency;	}
	BOOL* GetAutoResizeFlag()	{		return &m_bAutoResize;	}

private:

	BOOL m_bLButtonDown;
	BOOL m_bResizing;
	CPoint m_pointLastLButtonDown;
	CRect m_rectWnd;

	CRect m_rectClose;	
	CRect m_rectPushPin;
	CRect m_rectResizer;

	HICON m_iconPushPin1;
	HICON m_iconPushPin2;
	HICON m_iconGrip;

	BOOL m_bMouseOverClose;
	BOOL m_bDisplayContinously;
	BOOL m_bMouseOverResizer;
	BOOL m_bMouseOverPushPin;

	int m_nTransparency;
	int m_nCurrentTransparency;

	CDisplayItem m_dispItemKanji;
	CDisplayItem m_dispItemHiragana;
	CDisplayItem m_dispItemEnglish;	

	int m_nDispOrder;
	DWORD m_dwTime_1_to_2_;
	DWORD m_dwTime_2_to_3_;
	DWORD m_dwTimeToLive;
	BOOL m_bAutoResize;
	CString m_strSkinName;

	void ResizeRects();
	void ValidateRectAndPoint(CPoint& point, CRect& rect, BOOL& condition);
	void SetTransparency(int nTransparency = 127);
	CDisplayItem* GetMappedStatus(int nIndex);
	void StartWaiting();
	void Hide();
	void inline StartTimer(int ID, int nElapse, int junk)
	{
		SetTimer(ID,nElapse,NULL);
	}
	void inline StopTimer(int ID)
	{
		KillTimer(ID);
	}
	void Stick(BOOL bGradual);

	DECLARE_DYNAMIC(CWndNotifier)
// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnMouseHover(WPARAM w, LPARAM l);
	afx_msg LRESULT OnMouseLeave(WPARAM w, LPARAM l);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
