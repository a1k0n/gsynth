// RenameModule.cpp : implementation file
//

#include "stdafx.h"
#include "gsynth.h"
#include "RenameModule.h"
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
// CRenameModule dialog


CRenameModule::CRenameModule(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameModule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenameModule)
	m_newname = _T("");
	//}}AFX_DATA_INIT
    m_newname = cb_machine_get_modulename();
}


void CRenameModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenameModule)
	DDX_Text(pDX, IDC_EDIT1, m_newname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRenameModule, CDialog)
	//{{AFX_MSG_MAP(CRenameModule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenameModule message handlers

void CRenameModule::OnOK() 
{
	UpdateData();
	// TODO: Add extra validation here
    cb_machine_rename(m_newname);
	CDialog::OnOK();
}
