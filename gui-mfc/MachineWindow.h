#if !defined(AFX_MACHINEWINDOW_H__1CBFA52A_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
#define AFX_MACHINEWINDOW_H__1CBFA52A_190C_11D3_9504_0000C01DBEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MachineWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMachineWindow window

class CMachineWindow : public CWnd
{
// Construction
public:
	CMachineWindow();

// Attributes
public:
	CMenu *addmenu;
	CMenu *removemenu;
	CMenu *linkmenu;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMachineWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMachineWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMachineWindow)
	afx_msg void OnAddMachine();
	afx_msg void OnRemoveMachine();
	afx_msg void OnDisconnectMachine();
	afx_msg void OnRenameMachine();
	afx_msg void OnAboutMachine();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACHINEWINDOW_H__1CBFA52A_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
