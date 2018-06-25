// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "PruebaPropiedades.h"
#include "WorkSpaceBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;
/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar

BEGIN_MESSAGE_MAP(CWorkSpaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkSpaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
{
	// TODO: add one-time construction code here

}

CWorkSpaceBar::~CWorkSpaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar message handlers

int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	//if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	//{
	//	TRACE0("Failed to create workspace view\n");
	//	return -1;      // fail to create
	//}

	//// Setup trees content:
	//HTREEITEM hRoot1 = m_wndTree.InsertItem (_T("Root 1"));
	//m_wndTree.InsertItem (_T("Item 1"), hRoot1);
	//m_wndTree.InsertItem (_T("Item 2"), hRoot1);

	RECT aux;
	aux.left = 0;
	aux.top = 0;
	aux.right = 250;
	aux.bottom = 500;
	m_wndPropList.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,aux,this,NULL);

	CBCGPColorProp* pColorProp = new CBCGPColorProp (_T("Color 1"), RGB (210, 192, 254), NULL, _T("Color 1"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor (COLOR_3DFACE));
	m_wndPropList.AddProperty(pColorProp);
	pColorProp = new CBCGPColorProp (_T("Color 2"), RGB (210, 192, 254), NULL, _T("Color 2"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor (COLOR_3DFACE));
	m_wndPropList.AddProperty(pColorProp);
	m_wndPropList.AddProperty(new CSliderProp (_T("r"), 0, _T("")));
	m_wndPropList.AddProperty(new CSliderProp (_T("s"), 0, _T("")));
	m_wndPropList.AddProperty(new CSliderProp (_T("t"), 0, _T("")));
	m_wndPropList.AdjustLayout();

	return 0;
}

void CWorkSpaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	m_wndPropList.SetWindowPos(NULL,0, 0,cx, cy ,SWP_NOACTIVATE | SWP_NOZORDER);
	// Tab control should cover a whole client area:
	//m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
	//	cx - 2 * nBorderSize, cy - 2 * nBorderSize,
	//	SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rectSB;
	m_wndPropList.GetWindowRect(rectSB);
//	m_wndTree.GetWindowRect (rectTree);
//	ScreenToClient (rectTree);

//	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectSB,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl

CPropSliderCtrl::CPropSliderCtrl(CSliderProp* pProp, COLORREF clrBack)
{
	m_clrBack = RGB(100,100,100);
	m_brBackground.CreateSolidBrush (m_clrBack);
	m_pProp = pProp;
}

CPropSliderCtrl::~CPropSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CPropSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CPropSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl message handlers

HBRUSH CPropSliderCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/) 
{
	pDC->SetBkColor (m_clrBack);
	return m_brBackground;
}
//************************************************************************************
void CPropSliderCtrl::HScroll (UINT /*nSBCode*/, UINT /*nPos*/)
{
	ASSERT_VALID (m_pProp);

	m_pProp->OnUpdateValue ();
	m_pProp->Redraw ();
}

////////////////////////////////////////////////////////////////////////////////
// CSliderProp class

CSliderProp::CSliderProp(const CString& strName, float nValue, 
							 LPCTSTR lpszDescr, DWORD dwData)
	:	CBCGPProp (strName, nValue, lpszDescr, dwData)
{
}
//************************************************************************************
CWnd* CSliderProp::CreateInPlaceEdit (CRect rectEdit, BOOL& bDefaultFormat)
{
	CPropSliderCtrl* pWndSlider = new CPropSliderCtrl (this, m_pWndList->GetBkColor ());

	rectEdit.left += rectEdit.Height () + 5;

	pWndSlider->Create (WS_VISIBLE | WS_CHILD, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
	pWndSlider->SetPos ((long) m_varValue);

	bDefaultFormat = TRUE;
	return pWndSlider;
}
//******************************************************************************************
BOOL CSliderProp::OnUpdateValue ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pWndInPlace);
	ASSERT_VALID (m_pWndList);
	ASSERT (::IsWindow (m_pWndInPlace->GetSafeHwnd ()));

	float lCurrValue = m_varValue;

	CSliderCtrl* pSlider = (CSliderCtrl*) m_pWndInPlace;

	m_varValue = (float) pSlider->GetPos ()/50.0f;

	if (lCurrValue != (float) m_varValue)
	{
		m_pWndList->OnPropertyChanged (this);
	}

	return TRUE;
}
//******************************************************************************************
BOOL CSliderProp::OnClickValue (UINT /*uiMsg*/, CPoint /*point*/)
{
	return TRUE;
}
