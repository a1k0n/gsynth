#if !defined(AFX_CREATEPATTERN_H__AE717AE3_195A_11D3_9504_0000C01DBEF9__INCLUDED_)
#define AFX_CREATEPATTERN_H__AE717AE3_195A_11D3_9504_0000C01DBEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreatePattern.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreatePattern dialog

class CCreatePattern : public CDialog
{
// Construction
public:
	CCreatePattern(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreatePattern)
	enum { IDD = IDD_DIALOG3 };
	CString	m_patternname;
	UINT	m_patternlength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreatePattern)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreatePattern)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEPATTERN_H__AE717AE3_195A_11D3_9504_0000C01DBEF9__INCLUDED_)
