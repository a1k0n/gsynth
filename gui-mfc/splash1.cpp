// Splash1.cpp : implementation file
//

#include "stdafx.h"
#include "gsynth.h"
#include "Splash1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplash dialog


CSplash::CSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplash)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplash, CDialog)
	//{{AFX_MSG_MAP(CSplash)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplash message handlers

void CSplash::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(timerID);

	CDialog::OnTimer(nIDEvent);

	DestroyWindow();
	delete this;
}

BOOL CSplash::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	timerID = SetTimer(1, 2000, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
