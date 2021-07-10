#pragma once
#include "afxcoll.h"

class CEntry;

class CDataMgr
{
public:

	CDataMgr(void);
	~CDataMgr(void);	
	CEntry GetRandomEntry(void);
	CEntry GetNext();
	int InitializeXL(CString strFileName);
	BOOL CDataMgr::SetRange(int nStartRange, int nEndRange);

private:
	CStringArray m_strDataArray;
	CStringArray m_strDataArray3;
	CStringArray m_strDataArray2;

	int m_nStartRange;
	int m_nEndRange;

	int m_nCurrentIndex;
	int m_nCount;

	CEntry GetEntry(int index);		
};
