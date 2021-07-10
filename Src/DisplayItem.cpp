#include "StdAfx.h"
#include ".\displayitem.h"

CDisplayItem::CDisplayItem(void):m_bDisplay(FALSE),m_bEnabled(TRUE)
{
}

CDisplayItem::~CDisplayItem(void)
{
	m_font.DeleteObject();
}

void CDisplayItem::Initialize(LPCTSTR szFont, int nSize, COLORREF cr, CString szCaption)
{
	m_strCaption = szCaption;
	m_font.DeleteObject();
	m_font.CreatePointFont(nSize,szFont);
	m_crf = cr;
}

void CDisplayItem::SetText(CString szCaption, CWnd* wndParent)
{
	m_strCaption = szCaption;
	m_bEnabled = _tcscmp(_T(""), m_strCaption)?1:0;

	CPaintDC dc(wndParent);	
	dc.SelectObject(&m_font);
	m_sizeText = dc.GetOutputTextExtent(m_strCaption);
}

void CDisplayItem::DrawDisplayItem(CDC* pDC)
{
	if(m_bDisplay == TRUE)
	{	
		pDC->SetTextColor(m_crf);
		pDC->SelectObject(&m_font);
		pDC->DrawText(m_strCaption,m_rectText,DT_WORDBREAK | DT_VCENTER | DT_END_ELLIPSIS | DT_CENTER | DT_SINGLELINE);
	}
}
