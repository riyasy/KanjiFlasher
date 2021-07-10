#include "StdAfx.h"
#include ".\entry.h"



CEntry::CEntry()
{
}

CEntry::~CEntry(void)
{
}

CEntry::CEntry(CString content)
{    
	int position = content.Find(_T(","),0);
    if(position >-1)
    {
        strKanji = content.Left(position);
	    content.Delete(0, position+1);

        position = content.Find(_T(","),0);
        if(position >-1)
        {
	        strHgna = content.Left(position);
	        content.Delete(0, position+1);
        }   
    }
	content.Replace(',','\r');
	strEngMeaning = content;
}