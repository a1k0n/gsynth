#if !defined(AFX_ADDMODULE_H__1CBFA526_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
#define AFX_ADDMODULE_H__1CBFA526_190C_11D3_9504_0000C01DBEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddModule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddModule dialog

class CAddModule : public CDialog
{
// Construction
public:
	CAddModule(CWnd* pParent = NULL);   // standard constructor

	void AddMachineName(const char *name, void *modinfo);

// Dialog Data
	//{{AFX_DATA(CAddModule)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_combo1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddModule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddModule)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDMODULE_H__1CBFA526_190C_11D3_9504_0000C01DBEF9__INCLUDED_)
