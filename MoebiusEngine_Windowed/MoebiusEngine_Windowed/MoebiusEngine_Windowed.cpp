// MoebiusEngine_Windowed.cpp: define el punto de entrada de la aplicaci�n.
//

#include "stdafx.h"
#include "MoebiusEngine_Windowed.h"
#include "Implementation.h"

#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;								// Instancia actual
TCHAR szTitle[MAX_LOADSTRING];					// Texto de la barra de t�tulo
TCHAR szWindowClass[MAX_LOADSTRING];			// nombre de clase de la ventana principal

static HWND hWnd;

// Declaraciones de funciones adelantadas incluidas en este m�dulo de c�digo:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

static CDirectXManager_ mDXManager;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: colocar c�digo aqu�.
	MSG msg;
	HACCEL hAccelTable;

	// Inicializar cadenas globales
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MOEBIUSENGINE_WINDOWED, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Realizar la inicializaci�n de la aplicaci�n:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOEBIUSENGINE_WINDOWED));

	// Main message loop:
	msg.message = WM_NULL;
	while( WM_QUIT != msg.message  )
    {
        // Use PeekMessage() so we can use idle time to render the scene. 
        bool bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

        if( bGotMsg )
        {
            // Translate and dispatch the message
            if( hWnd == NULL || 0 == TranslateAccelerator( msg.hwnd, hAccelTable, &msg ))
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        {
			mDXManager.FramePass();
        }
    }

	//Release the device
	mDXManager.OnDisposeDevice();

	UnregisterClass(szWindowClass, hInstance);

	return (int) msg.wParam;
}

//
//  FUNCI�N: MyRegisterClass()
//
//  PROP�SITO: registrar la clase de ventana.
//
//  COMENTARIOS:
//
//    Esta funci�n y su uso son s�lo necesarios si desea que el c�digo
//    sea compatible con sistemas Win32 anteriores a la funci�n
//    'RegisterClassEx' que se agreg� a Windows 95. Es importante llamar a esta funci�n
//    para que la aplicaci�n obtenga iconos peque�os bien formados
//    asociados a ella.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOEBIUSENGINE_WINDOWED));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MOEBIUSENGINE_WINDOWED);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCI�N: InitInstance(HINSTANCE, int)
//
//   PROP�SITO: guardar el identificador de instancia y crear la ventana principal
//
//   COMENTARIOS:
//
//        En esta funci�n, se guarda el identificador de instancia en una variable com�n y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  

   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   if(FAILED(mDXManager.OneTimeInit(hWnd, hInst)))
   {
	   MessageBox(hWnd, L"Error in OneTimeInit", L"", 0);
	   return FALSE;
   }

   if(FAILED(mDXManager.CreateDevice(hWnd, hInst, TRUE)))
   {
	   MessageBox(hWnd, L"Error in device creation", L"", 0);
	   return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCI�N: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROP�SITO: procesar mensajes de la ventana principal.
//
//  WM_COMMAND	: procesar el men� de aplicaci�n
//  WM_PAINT	: pintar la ventana principal
//  WM_DESTROY	: enviar un mensaje de finalizaci�n y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Analizar las selecciones de men�:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	//case WM_PAINT:
	//	hdc = BeginPaint(hWnd, &ps);
	//	// TODO: agregar c�digo de dibujo aqu�...
	//	EndPaint(hWnd, &ps);
	//	break;

	case WM_SIZE:
		mDXManager.OnResize(wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
