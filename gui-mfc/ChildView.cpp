// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "gsynth.h"
#include "ChildView.h"

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
// CChildView
CWnd *childwnd = NULL;
static int inited = 0;


CChildView::CChildView()
{
	childwnd = this;

	mytab = new CTabCtrl;

	machinewindow = new CMachineWindow();
	trackerwindow = new CTrackerWindow();
}

CChildView::~CChildView()
{
	mytab->DeleteAllItems();
	delete mytab;
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_BUTTON32771, OnButtonPlay)
	ON_COMMAND(ID_BUTTON32772, OnButtonStop)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, 1010, OnTabSelchange)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
}


void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	if(inited)
	{
		machinewindow->MoveWindow(5,40,cx-10,cy-45);
		trackerwindow->MoveWindow(5,40,cx-10,cy-45);
		mytab->MoveWindow(0,0,cx,cy, FALSE);
	}

	// TODO: Add your message handler code here

}

void CChildView::OnDestroy() 
{
	CWnd ::OnDestroy();

	machinewindow->DestroyWindow();
	trackerwindow->DestroyWindow();
	
	// TODO: Add your message handler code here


}

void CChildView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd ::OnShowWindow(bShow, nStatus);
	
	if(!inited)
	{
		mytab->Create(TCS_TABS | WS_VISIBLE, CRect(0,0,300, 50), this, 1010);
		mytab->InsertItem(0, "Machine Editor");
		mytab->InsertItem(1, "Tracker");
		mytab->InsertItem(2, "Sequencer");
//		mytab->InsertItem(3, "Tab4");
//		mytab->InsertItem(4, "Tab5");
//		mytab->InsertItem(5, "Tab6");
//		mytab->InsertItem(6, "Tab7");
//		mytab->InsertItem(7, "Tab8");
//		mytab->InsertItem(8, "Tab9");
//		mytab->InsertItem(9, "Tab10");


		RECT rec={0,0,100,100};
		BOOL blah = machinewindow->Create(NULL, NULL, WS_CHILD, rec, this, 0, NULL);
		machinewindow->ShowWindow(SW_SHOW);

		RECT rec2={0,0,100,100};
		blah = trackerwindow->Create(NULL, NULL, WS_CHILD, rec2, this, 0, NULL);
		trackerwindow->ShowWindow(SW_HIDE);

		init_common_gui();
		inited = 1;
	}
	// TODO: Add your message handler code here
}

void CChildView::OnButtonPlay()
{
	// TODO: Add your command handler code here
	MessageBox("THIS DOES NOTHING!@!", "playing");
}

void CChildView::OnButtonStop()
{
	// TODO: Add your command handler code here
	MessageBox("DON'T PRESS THIS !@*!^%@(&*($(%#*@!", "Stop");
}

void CChildView::OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	int ntab = mytab->GetCurSel();

	currentwindowstate = ntab;

	switch(ntab)
	{
	case 0:
		machinewindow->ShowWindow(SW_SHOW);
		trackerwindow->ShowWindow(SW_HIDE);
		break;
	case 1:
		machinewindow->ShowWindow(SW_HIDE);
		trackerwindow->ShowWindow(SW_SHOW);
		break;
	case 2:
		machinewindow->ShowWindow(SW_HIDE);
		trackerwindow->ShowWindow(SW_HIDE);
		break;
	}

	*pResult = 0;
}


UINT convert(UINT inkey, short shiftstatus)
{
    if(isalpha(inkey) && !shiftstatus)
	return tolower(inkey);
    if(shiftstatus)
	switch(inkey)
	{
	case '0': return ')';
	case '1': return '!';
	case '2': return '@';
	case '3': return '#';
	case '4': return '$';
	case '5': return '%';
	case '6': return '^';
	case '7': return '&';
	case '8': return '*';
	case '9': return '(';
	case '`': return '~';
	case '-': return '_';
	case '=': return '+';
	case '[': return '{';
	case ']': return '}';
	case '\\': return '|';
	case ';': return ':';
	case '\'': return '"';
	case ',': return '<';
	case '.': return '>';
	case '/': return '?';
	}

    return inkey;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int shiftstatus = GetKeyState(VK_SHIFT)&0x08000;
	switch(currentwindowstate)
	{
	case 0:
		break;
	case 1:
		if(nChar == VK_UP)
			nChar = GUI_KEY_UP;
		else if(nChar == VK_DOWN)
			nChar = GUI_KEY_DOWN;
		else if(nChar == VK_LEFT)
			nChar = GUI_KEY_LEFT;
		else if(nChar == VK_RIGHT)
			nChar = GUI_KEY_RIGHT;
		else
		{
			UINT charmap = MapVirtualKey(nChar,2);
			nChar = convert(charmap, shiftstatus);
		}
		::cb_tracker_key(nChar, shiftstatus);
		break;
	}
	
	CWnd ::OnKeyDown(nChar, nRepCnt, nFlags);
}
