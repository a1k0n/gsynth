// MachineWindow.cpp : implementation file
//

#include "stdafx.h"
#include "gsynth.h"
#include "MachineWindow.h"
#include "AddModule.h"
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
// CMachineWindow
CDC *pdc = NULL;
extern int mwidth, mheight; /* width and height of the drawing window */
CBitmap *oldbitmap;
CBitmap *pbitmap;
int mwidth = 100, mheight = 100; /* width and height of the drawing window */
CPen **penlist = NULL;
CBrush **brushlist = NULL;

CMachineWindow::CMachineWindow()
{
	addmenu = new CMenu;
	removemenu = new CMenu;
	linkmenu = new CMenu;
	addmenu->LoadMenu(IDR_MENU1);
	removemenu->LoadMenu(IDR_MENU2);
	linkmenu->LoadMenu(IDR_MENU3);
}

CMachineWindow::~CMachineWindow()
{
	addmenu->DestroyMenu();
	delete addmenu;
	removemenu->DestroyMenu();
	delete removemenu;
	linkmenu->DestroyMenu();
	delete linkmenu;
}


BEGIN_MESSAGE_MAP(CMachineWindow, CWnd)
	//{{AFX_MSG_MAP(CMachineWindow)
	ON_COMMAND(ID__ADDMACHINE, OnAddMachine)
	ON_COMMAND(ID__REMOVEMACHINE, OnRemoveMachine)
	ON_COMMAND(ID__DISCONNECTMACHINE, OnDisconnectMachine)
	ON_COMMAND(ID__RENAMEMODULE, OnRenameMachine)
	ON_COMMAND(ID__ABOUTTHISMODULE, OnAboutMachine)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMachineWindow message handlers
void CMachineWindow::OnAddMachine()
{
	CAddModule *addmoduledlg=NULL;
	addmoduledlg = new CAddModule(this);
	addmoduledlg->DoModal();
	delete addmoduledlg;
}

void CMachineWindow::OnRemoveMachine()
{
	::cb_machine_remove();
}

void CMachineWindow::OnDisconnectMachine()
{
	::cb_machine_disconnect();
}

void CMachineWindow::OnRenameMachine()
{
	CRenameModule renamemoduledlg(this);
	renamemoduledlg.DoModal();
}

void CMachineWindow::OnAboutMachine()
{
    const char *stuff = ::cb_machine_about_module();
    if(!stuff) stuff = "No information available about this module.";

	MessageBox(stuff, "About");
}



void CMachineWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	::cb_machine_mouse_move(point.x, point.y);
	
	CWnd::OnMouseMove(nFlags, point);
}

void CMachineWindow::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int which = ::cb_machine_popup_dialog();
	ClientToScreen(&point);
	switch(which) {
		case 0:
			(addmenu->GetSubMenu(0))->TrackPopupMenu( TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
			break;
		case 1:
			(removemenu->GetSubMenu(0))->TrackPopupMenu( TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
			break;
		case 2:
			(linkmenu->GetSubMenu(0))->TrackPopupMenu( TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
			break;
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CMachineWindow::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CMachineWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	int state = GUI_STATE_BUTTON_DOWN;
	if(GetKeyState(VK_SHIFT)&0x08000)
	{
		state |= GUI_STATE_SHIFT;
	}
	::cb_machine_mouse_button(1, state, point.x, point.y);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMachineWindow::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	int state = 0;
	if(GetKeyState(VK_SHIFT)&0x08000)
	{
		state |= GUI_STATE_SHIFT;
	}
	::cb_machine_mouse_button(1, state, point.x, point.y);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CMachineWindow::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	mwidth = cx;
	mheight = cy;
	if(!pdc) return;

	CBitmap *ppbitmap = new CBitmap;
	ppbitmap->CreateCompatibleBitmap(pdc, mwidth, mheight);
	CBitmap *prevbitmap = pdc->SelectObject(ppbitmap);
	if(prevbitmap)
		delete prevbitmap;
	pbitmap = ppbitmap;
	pdc->FillSolidRect(0,0,mwidth,mheight, RGB(192,192,192));
	Invalidate(FALSE);

	::cb_machine_repaint();
}

int CMachineWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CDC *maindc = GetDC();
	pdc = new CDC;
	pdc->CreateCompatibleDC(maindc);
	pdc->SetBkMode(TRANSPARENT);

	pbitmap = new CBitmap;
	pbitmap->CreateCompatibleBitmap(maindc, mwidth, mheight);
	oldbitmap = pdc->SelectObject(pbitmap);
	ReleaseDC(maindc);

	pdc->SelectStockObject(LTGRAY_BRUSH);
	pdc->SelectStockObject(WHITE_PEN);

	penlist = new CPen*[NUM_MACHINE_STATES];
	brushlist = new CBrush*[NUM_MACHINE_STATES];
	penlist[MACHINE_MASTER] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	penlist[MACHINE_EFFECT] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	penlist[MACHINE_GENERATOR] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	penlist[MACHINE_PRELIGHT] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	penlist[MACHINE_SELECTED] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	brushlist[MACHINE_MASTER] = new CBrush(RGB(192, 192, 192));
	brushlist[MACHINE_EFFECT] = new CBrush(RGB(0, 128, 128));
	brushlist[MACHINE_GENERATOR] = new CBrush(RGB(128, 0, 128));
	brushlist[MACHINE_PRELIGHT] = new CBrush(RGB(255, 255, 255));
	brushlist[MACHINE_SELECTED] = new CBrush(RGB(255, 200, 200));

	return 0;
}

void CMachineWindow::OnDestroy() 
{
	CWnd::OnDestroy();
	
	delete pdc;
	delete pbitmap;

	delete penlist[MACHINE_MASTER];
	delete penlist[MACHINE_EFFECT];
	delete penlist[MACHINE_GENERATOR];
	delete penlist[MACHINE_PRELIGHT];
	delete penlist[MACHINE_SELECTED];
	delete brushlist[MACHINE_MASTER];
	delete brushlist[MACHINE_EFFECT];
	delete brushlist[MACHINE_GENERATOR];
	delete brushlist[MACHINE_PRELIGHT];
	delete brushlist[MACHINE_SELECTED];
	delete[] penlist;
	delete[] brushlist;
}

void CMachineWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.BitBlt(0,0,mwidth, mheight, pdc, 0,0, SRCCOPY);

	// Do not call CWnd::OnPaint() for painting messages
}
