// CreatePattern.cpp : implementation file
//

#include "stdafx.h"
#include "gsynth.h"
#include "CreatePattern.h"
extern "C"
{
#include <gui-common/gui-common.h>
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreatePattern dialog


CCreatePattern::CCreatePattern(CWnd* pParent /*=NULL*/)
	: CDialog(CCreatePattern::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreatePattern)
	m_patternname = _T("");
	m_patternlength = 16;
	//}}AFX_DATA_INIT
}


void CCreatePattern::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreatePattern)
	DDX_Text(pDX, IDC_EDIT1, m_patternname);
	DDX_Text(pDX, IDC_EDIT2, m_patternlength);
	DDV_MinMaxUInt(pDX, m_patternlength, 1, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreatePattern, CDialog)
	//{{AFX_MSG_MAP(CCreatePattern)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreatePattern message handlers

void CCreatePattern::OnOK() 
{
	UpdateData();

	::cb_tracker_create_pattern(m_patternname, m_patternlength);
	
	CDialog::OnOK();
}
