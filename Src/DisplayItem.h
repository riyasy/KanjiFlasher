#pragma once

class CDisplayItem
{
public:
	CDisplayItem(void);	
	~CDisplayItem(void);
	void Initialize(LPCTSTR szFont, int nSize, COLORREF cr, CString szCaption);
	void DrawDisplayItem(CDC* pDC);
	void SetText(CString szCaption,CWnd* wndParent);

	COLORREF m_crf;
	CFont m_font;
	CString m_strCaption;
	CRect m_rectText;
	BOOL m_bDisplay;
	BOOL m_bEnabled;
	CSize m_sizeText;
};
