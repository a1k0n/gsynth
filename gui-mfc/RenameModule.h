#if !defined(AFX_RENAMEMODULE_H__1CBFA527_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
#define AFX_RENAMEMODULE_H__1CBFA527_190C_11D3_9504_0000C01DBEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenameModule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenameModule dialog

class CRenameModule : public CDialog
{
// Construction
public:
	CRenameModule(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRenameModule)
	enum { IDD = IDD_DIALOG2 };
	CString	m_newname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenameModule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenameModule)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENAMEMODULE_H__1CBFA527_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
