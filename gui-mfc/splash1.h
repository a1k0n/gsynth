#if !defined(AFX_SPLASH1_H__52A33404_2B03_11D3_9504_0000C01DBEF9__INCLUDED_)
#define AFX_SPLASH1_H__52A33404_2B03_11D3_9504_0000C01DBEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Splash1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplash dialog

class CSplash : public CDialog
{
	UINT timerID;

// Construction
public:
	CSplash(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplash)
	enum { IDD = IDD_SPLASHDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplash)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASH1_H__52A33404_2B03_11D3_9504_0000C01DBEF9__INCLUDED_)
