// TrackerWindow.cpp : implementation file
//

#include "stdafx.h"
#include "gsynth.h"
#include "TrackerWindow.h"
extern "C"
{
#include <gui-common/gui-common.h>
}
#include "CreatePattern.h"
#include <list>
using namespace std;

typedef struct
{
	CString name;
	void *data;
} GenericNode;

list<GenericNode> mlist;
list<GenericNode> plist;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTrackerWindow
int window_width=100, window_height=100;
int font_width=12, font_height=20;
int mlist_items = 0;
int plist_items = 0;
CDC *Tpdc = NULL;
CBitmap *Toldbitmap;
CBitmap *Tpbitmap;
CPen **Tpenlist = NULL;
CBrush **Tbrushlist = NULL;

CComboBox *mlistbox;
CComboBox *plistbox;
CButton *makebutton;

CTrackerWindow::CTrackerWindow()
{
	mlistbox = new CComboBox;
	plistbox = new CComboBox;
	makebutton = new CButton;
}

CTrackerWindow::~CTrackerWindow()
{
	delete mlistbox;
	delete plistbox;
	delete makebutton;
}


BEGIN_MESSAGE_MAP(CTrackerWindow, CWnd)
	//{{AFX_MSG_MAP(CTrackerWindow)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(1101, OnSelectModule)
	ON_CBN_SELCHANGE(1102, OnSelectPattern)
	ON_BN_CLICKED(1103, OnCreatePattern)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTrackerWindow message handlers

int CTrackerWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CDC *maindc = GetDC();
	Tpdc = new CDC;
	Tpdc->CreateCompatibleDC(maindc);
	Tpdc->SetBkMode(TRANSPARENT);

	Tpbitmap = new CBitmap;
	Tpbitmap->CreateCompatibleBitmap(maindc, window_width, window_height);
	Toldbitmap = Tpdc->SelectObject(Tpbitmap);
	ReleaseDC(maindc);

	Tpdc->SelectObject(GetStockObject(ANSI_FIXED_FONT));

	TEXTMETRIC tm;
	Tpdc->GetTextMetrics(&tm);
	font_height = tm.tmHeight;
	font_width = tm.tmAveCharWidth;


	Tpdc->SelectStockObject(LTGRAY_BRUSH);
	Tpdc->SelectStockObject(WHITE_PEN);
	
	Tpenlist = new CPen*[4];
	Tbrushlist = new CBrush*[4];
	Tpenlist[0] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	Tpenlist[1] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	Tpenlist[2] = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	Tpenlist[3] = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	Tbrushlist[0] = new CBrush(RGB(192, 192, 192));
	Tbrushlist[1] = new CBrush(RGB(224, 224, 224));
	Tbrushlist[2] = new CBrush(RGB(0, 0, 255));
	Tbrushlist[3] = new CBrush(RGB(170, 170, 170));

	mlistbox->Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD, CRect(10, 10, 10+250, 160), this, 1101);
	plistbox->Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD, CRect(10+10+250, 10, 10+10+250+250, 160), this, 1102);
	makebutton->Create("Create", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD, CRect(10+10+10+250+250, 10, 10+10+10+250+250+50, 35), this, 1103);

	int index = plistbox->AddString("(no patterns)");
	plistbox->SetItemDataPtr(index, NULL);
	plistbox->SetCurSel(index);

	return 0;
}

void CTrackerWindow::OnDestroy() 
{
	CWnd::OnDestroy();
	
	delete Tpdc;
	delete Tpbitmap;
	delete Tpenlist[0];
	delete Tpenlist[1];
	delete Tpenlist[2];
	delete Tpenlist[3];
	delete Tbrushlist[0];
	delete Tbrushlist[1];
	delete Tbrushlist[2];
	delete Tbrushlist[3];
	delete[] Tpenlist;
	delete[] Tbrushlist;
}

void CTrackerWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.BitBlt(0,0,window_width, window_height, Tpdc, 0,0, SRCCOPY);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CTrackerWindow::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	window_width = cx;
	window_height = cy;
	if(!Tpdc) return;

	CBitmap *ppbitmap = new CBitmap;
	ppbitmap->CreateCompatibleBitmap(Tpdc, window_width, window_height);
	CBitmap *prevbitmap = Tpdc->SelectObject(ppbitmap);
	if(prevbitmap)
		delete prevbitmap;
	Tpbitmap = ppbitmap;
	Tpdc->FillSolidRect(0,0,window_width, window_height, RGB(192,192,192));
	Invalidate(FALSE);

	::cb_tracker_repaint();
}

void CTrackerWindow::OnSelectModule()
{
	int cursel = mlistbox->GetCurSel();
	if(cursel != CB_ERR)
	{
		::cb_tracker_module_select(mlistbox->GetItemDataPtr(cursel));
	}
	// set focus back to main window
	GetParent()->SetFocus();
}

void CTrackerWindow::OnSelectPattern()
{
	int cursel = plistbox->GetCurSel();
	if(cursel != CB_ERR)
	{
		::cb_tracker_pattern_select(plistbox->GetItemDataPtr(cursel));
	}
	// set focus back to main window
	GetParent()->SetFocus();
}

void CTrackerWindow::OnCreatePattern()
{
	CCreatePattern dlg;
	dlg.DoModal();
	// set focus back to main window
	GetParent()->SetFocus();
}

int
tracker_mlist_add (const char *name, void *data)
{
	int newindex = mlistbox->AddString(name);
	mlistbox->SetItemDataPtr(newindex, data);

	mlistbox->SetCurSel(newindex);
	::cb_tracker_module_select(mlistbox->GetItemDataPtr(newindex));


	return mlistbox->GetCount();
}

int
tracker_plist_add (const char *name, void *data)
{
	int newindex = plistbox->AddString(name);
	plistbox->SetItemDataPtr(newindex, data);

	plistbox->SetCurSel(newindex);
	::cb_tracker_pattern_select(plistbox->GetItemDataPtr(newindex));

	return plistbox->GetCount();
}

void
tracker_mlist_clear (void)
{
  /* assuming an add will be done soon after, this is all that's necessary */
  mlistbox->ResetContent();
}

void
tracker_plist_clear (void)
{
  plistbox->ResetContent();
}


void CTrackerWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
}
