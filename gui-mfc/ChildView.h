// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__505472C5_C374_11D2_9503_0000C01DBEF9__INCLUDED_)
#define AFX_CHILDVIEW_H__505472C5_C374_11D2_9503_0000C01DBEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

#include "MachineWindow.h"
#include "TrackerWindow.h"

class CChildView : public CWnd
{
	int currentwindowstate; // 0 = machine window, 1 = tracker window

	CMachineWindow *machinewindow;
	CTrackerWindow *trackerwindow;
// Construction
public:
	CChildView();

// Attributes
public:
	CTabCtrl *mytab;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonStop();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__505472C5_C374_11D2_9503_0000C01DBEF9__INCLUDED_)
