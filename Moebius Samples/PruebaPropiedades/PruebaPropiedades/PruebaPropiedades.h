// PruebaPropiedades.h : main header file for the PruebaPropiedades application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols



// CPruebaPropiedadesApp:
// See PruebaPropiedades.cpp for the implementation of this class
//

class CPruebaPropiedadesApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CPruebaPropiedadesApp();

	// Override from CBCGPWorkspace
	virtual void PreLoadState ();
	
	void OnFileNew() ;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int Run();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};