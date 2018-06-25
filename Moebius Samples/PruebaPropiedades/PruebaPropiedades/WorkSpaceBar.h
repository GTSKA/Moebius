// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkSpaceBar : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar();

// Attributes
protected:
	//CTreeCtrl	m_wndTree;

// Operations
public:
	CBCGPPropList	m_wndPropList;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkSpaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------
class CSliderProp : public CBCGPProp
{
public:
	CSliderProp(
		const CString& strName,
		float nValue,
		LPCTSTR lpszDescr = NULL,
		DWORD dwData = 0
	);

	virtual BOOL OnUpdateValue ();

protected:
	virtual CWnd* CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat);
	virtual BOOL OnSetCursor () const	{	return FALSE; /* Use default */	}
	virtual BOOL OnClickValue (UINT uiMsg, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl window

class CPropSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CPropSliderCtrl(CSliderProp* pProp, COLORREF clrBack);

// Attributes
protected:
	CBrush			m_brBackground;
	COLORREF		m_clrBack;
	CSliderProp*	m_pProp;


// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSliderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropSliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSliderCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void HScroll (UINT nSBCode, UINT nPos);

	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------
extern CPruebaPropiedadesApp theApp;