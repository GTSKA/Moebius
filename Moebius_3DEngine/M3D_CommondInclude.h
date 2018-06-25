#ifndef UNICODE
#error "Error: Moebius Engine requires a Unicode build."
#endif

// Standard Windows includes
#include <windows.h>
#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <shlobj.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>


//-----------------------------------------------------------------------------
//MACROS
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define MULTI_TO_WIDE( ach,wch )  MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, ach, -1, wch, _MAX_PATH );
#define WIDE_TO_MULTI(wch,ach,size) WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wch,-1,ach,size*sizeof(char),NULL,NULL);

#include <CArray_.h>
#include <QTimer_.h>

//-----------------------------------------------------------------------------
//INTERFACES
//-----------------------------------------------------------------------------



