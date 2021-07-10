#include "StdAfx.h"
#include ".\datamgr.h"
#include "wCApplication.h" // necessary for reading and writing into excel sheet.
#include "CWorkBooks.h"    // necessary for reading and writing into excel sheet
#include "CWorkBook.h"     // necessary for reading and writing into excel sheet
#include "CWorksheets.h"   // necessary for reading and writing into excel sheet
#include "CWorksheet.h"    // necessary for reading and writing into excel sheet
#include "CRange.h"        // necessary for reading and writing into excel sheet
#include  "Entry.h"

CDataMgr::CDataMgr(void)
{
	m_nCount = 0;
	m_nCurrentIndex = 0;
}

CDataMgr::~CDataMgr(void)
{
}

int CDataMgr::InitializeXL(CString strFileName)
{
	CApplication appReadEntry; // appReadEntry is the Excel _Application object

	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	// Start Excel and get Application object.

	if(!appReadEntry.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("Cannot start Excel and get Application object."));
		ExitProcess(0);
		return FALSE;
	}
	else
	{
		//Make the application visible and give the user control of
		//Microsoft Excel.
		appReadEntry.put_Visible(FALSE);    // Application not visible to user.
		appReadEntry.put_UserControl(TRUE); // Application is put to user control.
	}
	try
	{
		CWorkbooks theWorkbook;        
		theWorkbook = appReadEntry.get_Workbooks();
		CWorkbook book = theWorkbook.Open(
			strFileName,covOptional ,covFalse, covOptional,covOptional, 
			covOptional,covOptional, covOptional, covOptional, covOptional,
			covOptional,covOptional,covOptional);

		CWorksheets sheets = book.get_Worksheets();
		// get the first and only worksheet from the collection
		// of worksheets
		CWorksheet sheet = 
			(CWorksheet)sheets.get_Item(COleVariant((short)1)); 	


		CRange range = 
			sheet.get_Range(COleVariant(_T("A1")),COleVariant(_T("A1"))); 

		CRange range2 = range.get_EntireColumn();		

		CRange range3 = range2.get_End(4);

		int rowCount = range3.get_Row();

		if(rowCount == 1 || rowCount == 65536)
		{
			theWorkbook.Close(); // Closes the excel sheet.
			appReadEntry.Quit(); // Exits from the application	
			return 0;
		}
			
		else
			m_nCount = rowCount - 1;

		CString strStart;
		strStart.Format(_T("A%d"), 2);
		CString strEnd;
		strEnd.Format(_T("F%d"), rowCount);

		range = 
			sheet.get_Range(COleVariant(strStart),COleVariant(strEnd)); 

		COleSafeArray safeArray;// To read the cells.
		DWORD numElements[]={m_nCount,6};   
		safeArray.Create(VT_BSTR, 2, numElements);
		safeArray=range.get_Value2();
		long iRows;
		long iCols;
		safeArray.GetUBound(1, &iRows); 
		safeArray.GetUBound(2, &iCols);
		CString strVal,strBstr;	
		CStringArray xArray;
		long index[2];
		COleVariant oleVal;

		// Iterates through all the rows and columns.
		for (int m_nrowCounter = 1; m_nrowCounter <= iRows; m_nrowCounter++) 
		{
			for (int m_ncolCounter = 1; m_ncolCounter <= iCols; m_ncolCounter++)
			{
				index[0] = m_nrowCounter;
				index[1] = m_ncolCounter;			
				safeArray.GetElement(index,oleVal);
				switch(oleVal.vt)
				{
				case VT_R8:
					{
						strVal.Format(_T("%.0f"),oleVal.dblVal);
						//xArray.SetAtGrow(m_ncolCounter-1,strVal);					
						xArray.Add(strVal);
						break;
					}
				case VT_BSTR:
					{
						strBstr = CString(oleVal.bstrVal);
						//xArray.SetAtGrow(m_ncolCounter-1,strBstr);
						xArray.Add(strBstr);
						break;
					}
				case VT_EMPTY:
					{
						strBstr = CString(_T(""));
						//xArray.SetAtGrow(m_ncolCounter-1,strBstr);
						xArray.Add(strBstr);
						break;
					}
				}
			}       
		}

		theWorkbook.Close(); // Closes the excel sheet.
		appReadEntry.Quit(); // Exits from the application	

		m_strDataArray.RemoveAll();		
		m_strDataArray.Copy(xArray);	

	}
	catch(...)
	{
		AfxMessageBox(_T("Cannot read kanjis.xls"));	
		appReadEntry.Quit(); // Exits from the application	
		ExitProcess(0);		
	}
	
	m_nCurrentIndex = 1;

	return m_nCount;
}

// Read data from Excel file
BOOL CDataMgr::SetRange(int nStartRange, int nEndRange)
{
	m_nStartRange = nStartRange;
	m_nEndRange = nEndRange;

	m_nCurrentIndex = m_nStartRange;
	
	m_strDataArray2.RemoveAll();
	
	for(int i = (m_nStartRange - 1)* 6 ; i < m_nEndRange*6 ; i++)
	{
		m_strDataArray2.Add(m_strDataArray[i]);
	}

	return TRUE;

}

CEntry CDataMgr::GetRandomEntry(void)
{
	CEntry ss;

	m_nCurrentIndex = 0;

	if(m_strDataArray2.IsEmpty())
	{
		m_strDataArray2.RemoveAll();

		for(int i = (m_nStartRange - 1)* 6 ; i < m_nEndRange*6 ; i++)
		{
			m_strDataArray2.Add(m_strDataArray[i]);
		}	
	}

	if(!m_strDataArray2.IsEmpty())
	{
		srand(time(0));
		int i = (rand()%(m_strDataArray2.GetCount()/6));		
		i = i*6;
		
		ss.strKanji = m_strDataArray2[i];
		ss.strHgna = m_strDataArray2[i+1];
		for(int jj = 2 ; jj < 6 ; jj++)
		{
			if(m_strDataArray2[i+jj].Compare(_T("")))
			{
				ss.strEngMeaning = ss.strEngMeaning + _T(", ") + m_strDataArray2[i+jj];
			}
		}

		if(ss.strEngMeaning.Find(_T(", "),0) == 0)
		{
			ss.strEngMeaning.Delete(0,2);
		}

		m_strDataArray2.RemoveAt(i,6);

	}

	return ss;

}



CEntry CDataMgr::GetEntry(int index)
{
	CEntry ss;


	if(!m_strDataArray.IsEmpty())
	{
		int i = (index-1)*6;	

		ss.strKanji = m_strDataArray[i];
		ss.strHgna = m_strDataArray[i+1];
		for(int jj = 2 ; jj < 6 ; jj++)
		{
			if(m_strDataArray[i+jj].Compare(_T("")))
			{
				ss.strEngMeaning = ss.strEngMeaning + _T(", ") + m_strDataArray[i+jj];
			}
		}	

		if(ss.strEngMeaning.Find(_T(", "),0) == 0)
		{
			ss.strEngMeaning.Delete(0,2);
		}

	}
	return ss;
}

CEntry CDataMgr::GetNext()
{
	if(m_nCurrentIndex <= m_nEndRange)
	{		
		return GetEntry(m_nCurrentIndex++);
	}
	else
	{
		m_nCurrentIndex = m_nStartRange;
		return GetEntry(m_nCurrentIndex);
	}
}


