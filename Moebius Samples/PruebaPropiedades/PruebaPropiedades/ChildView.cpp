// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "PruebaPropiedades.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//dc.Rectangle(0,0,500,500);

	
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	//ID_CAPTION_TIMER
	//SetTimer(m_nTimer, 500, NULL);

	return 0;
}



void CChildView::OnFinalRelease()
{
	// TODO: Agregue aquí su código especializado o llame a la clase base

	CWnd::OnFinalRelease();
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado

	return false;
	//return CWnd::OnEraseBkgnd(pDC);
}


