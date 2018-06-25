// PruebaPropiedades.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PruebaPropiedades.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRenderCtrl _engine;

// CPruebaPropiedadesApp

BEGIN_MESSAGE_MAP(CPruebaPropiedadesApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


// CPruebaPropiedadesApp construction

CPruebaPropiedadesApp::CPruebaPropiedadesApp() :
	CBCGPWorkspace (TRUE /* m_bResourceSmartUpdate */)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPruebaPropiedadesApp object

CPruebaPropiedadesApp theApp;


// CPruebaPropiedadesApp initialization

BOOL CPruebaPropiedadesApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	globalData.SetDPIAware ();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("BCGP AppWizard-Generated Applications"));

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();

	// TODO: Remove this if you don't want extended tooltips:
	InitTooltipManager();

	CBCGPToolTipParams params;
	params.m_bVislManagerTheme = TRUE;

	theApp.GetTooltipManager ()->SetTooltipParams (
		BCGP_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS (CBCGPToolTipCtrl),
		&params);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// TODO:  Agregue aquí su código de creación especializado
	if(SUCCEEDED(_engine.CheckDevice(pFrame->m_wndView, true)))
	//if(SUCCEEDED(_engine.CheckDevice(pFrame->m_hWnd, true)))
	{
		if(SUCCEEDED(_engine.CreateDevice()))
		{
			//MessageBox(L"SUCCES!!!", 0 ,0);
		}
	}

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();

	

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// CPruebaPropiedadesApp message handlers

int CPruebaPropiedadesApp::ExitInstance() 
{
	BCGCBProCleanUp();

	_engine.OnDisposeDevice();

	return CWinApp::ExitInstance();
}
void CPruebaPropiedadesApp::OnFileNew() 
{
	CString strUntitled;
	CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, m_pMainWnd);
	if (pFrameWnd != NULL)
	{
		//TODO: close and destroy any file-specific data you've got
		// in memory. Once done, execute the following if statement
		// to reset the title of the frame window
		if (strUntitled.LoadString(AFX_IDS_UNTITLED))
			pFrameWnd->UpdateFrameTitleForDocument(strUntitled);
		else
			pFrameWnd->UpdateFrameTitleForDocument(NULL);
	}
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	CBCGPURLLinkButton m_btnURL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CPruebaPropiedadesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CPruebaPropiedadesApp message handlers


void CPruebaPropiedadesApp::PreLoadState ()
{
}


int CPruebaPropiedadesApp::Run()
{
	// TODO: Agregue aquí su código especializado o llame a la clase base
	MSG msg;
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
		  if(GetMessage(&msg, NULL, 0, 0) == FALSE)
		  {
			  ExitInstance();
			  return msg.wParam;    // WM_QUIT
		  }
		  TranslateMessage(&msg);
		  DispatchMessage(&msg);
		}
		else
		{
			_engine.FrameLoop(false);
		}
	}
	return __super::Run();
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	_engine.OnResize(nType, 0 ,0);
	// TODO: Agregue aquí su código de controlador de mensajes
}

void CChildView::OnEnterSizeMove()
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	_engine.OnBeginSizeMove();
	CWnd::OnEnterSizeMove();
}

void CChildView::OnExitSizeMove()
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	_engine.OnEndSizeMove();
	CWnd::OnExitSizeMove();
}

void CChildView::OnClose()
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	//KillTimer(m_nTimer); 
	CWnd::OnClose();
}
void CChildView::RenderPass()
{
	_engine.FrameLoop(false);
}



BOOL CPruebaPropiedadesApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Agregue aquí su código especializado o llame a la clase base

	

	return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Agregue aquí su código especializado o llame a la clase base
	

	return CBCGPFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CChildView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Agregue aquí su código especializado o llame a la clase base
	switch(message)
	{
		case WM_MOUSEMOVE:
			//this->SetWindowTextW(L"MOUSE MOVE");

			_engine.xMouse = GET_X_LPARAM(lParam); 
			_engine.yMouse = GET_Y_LPARAM(lParam); 

		break;
	}
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}
