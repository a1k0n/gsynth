#if !defined(AFX_TRACKERWINDOW_H__1CBFA52B_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
#define AFX_TRACKERWINDOW_H__1CBFA52B_190C_11D3_9504_0000C01DBEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackerWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackerWindow window

class CTrackerWindow : public CWnd
{
// Construction
public:
	CTrackerWindow();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackerWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrackerWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTrackerWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelectModule();
	afx_msg void OnSelectPattern();
	afx_msg void OnCreatePattern();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKERWINDOW_H__1CBFA52B_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
