// AddModule.cpp : implementation file
//

#include "stdafx.h"
#include "gsynth.h"
#include "AddModule.h"
extern "C"
{
#include <gui-common/gui-common.h>
}

#include <list>
using namespace std;

typedef struct
{
	CString name;
	void *data;
} ModuleNode;

list<ModuleNode> modulelist;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddModule dialog


CAddModule::CAddModule(CWnd* pParent /*=NULL*/)
	: CDialog(CAddModule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddModule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddModule)
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddModule, CDialog)
	//{{AFX_MSG_MAP(CAddModule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddModule message handlers

void CAddModule::OnOK() 
{
	// TODO: Add extra validation here
	int cursel = m_combo1.GetCurSel();
	if(cursel != CB_ERR)
	{
		::cb_machine_add(m_combo1.GetItemDataPtr(cursel)); // the param here is the moduleinfo
	}
	
	CDialog::OnOK();
}

void CAddModule::AddMachineName(const char *name, void *modinfo) 
{
	int newindex = m_combo1.AddString(name);
	m_combo1.SetItemDataPtr(newindex, modinfo);
	m_combo1.SetCurSel(newindex);
}

BOOL CAddModule::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	list<ModuleNode>::iterator _i;
	for(_i=modulelist.begin(); _i!= modulelist.end(); _i++)
	{
		AddMachineName((*_i).name, (*_i).data);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

extern "C" void add_machine_name(const char *name, void *modinfo)
{
	ModuleNode newnode;
	newnode.name = name;
	newnode.data = modinfo;
	modulelist.push_back(newnode);
}

