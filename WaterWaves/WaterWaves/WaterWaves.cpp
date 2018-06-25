// WaterWaves.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WaterWaves.h"
#include "Camera.h"
#include <D3D11.h>
#include <D3Dcompiler.h>
#include <cuda_d3d11_interop.h>
#include <cuda.h>
#include <cuda_runtime.h>



//include Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#define MAX_LOADSTRING 100

// Global Variables:
struct CUSTOMVERTEX { FLOAT x, y, z; DWORD color; };
struct wave { float amplitude; float wavelength; float speed; float directionx; float directiony; float steepness; };
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ID3D11Device* dev;
ID3D11DeviceContext* devcon;
IDXGISwapChain *swapChain;

ID3D11VertexShader *pVS;    // the vertex shader
ID3D11PixelShader *pPS;     // the pixel shader

ID3D11RasterizerState *pRS;    
ID3D11InputLayout *pLayout;

ID3D11Buffer *pCBuffer;

int numVertex;
int numIndex;

ID3D11Buffer* vertexBuffer;
ID3D11Buffer* indexBuffer;

struct cudaGraphicsResource* vertexBuffer_CUDA;

LARGE_INTEGER g_L1;
LARGE_INTEGER g_L0;
LARGE_INTEGER Fq;
float g_DeltaTime;
static const UINT g_width = 64;
static const UINT g_height = 64;

Camera cam;

// Function declaration:
HRESULT InitGraphics(HWND *hWnd);
HRESULT InitPlane(UINT width, UINT height);
HRESULT InitPipeline();
void release();
void UpdateTime();
void Update(HWND* hwnd);
void Draw();
int LoadShader(unsigned int Type, char* filename, ID3DBlob** shaderBlob);

extern "C"
{
	void createVertices(float4* positions, float time, unsigned int width, unsigned int height);
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WATERWAVES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WATERWAVES));

    MSG msg;
	QueryPerformanceFrequency(&Fq);
	QueryPerformanceCounter(&g_L0);
	InitGraphics();
	InitPlane(g_width, g_height);

	cam.Init();
	cam.calcWorldMatrix();
	cam.calcViewMatrix();
	keyPressed = 0; //pressed keys flag
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		UpdateTime();

		Update(&msg.hwnd);
		Draw();
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WATERWAVES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WATERWAVES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
			
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

HRESULT InitGraphics(HWND *hWnd)
{
	HRESULT hr = S_OK;
	IDXGIFactory* factory;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	IDXGIAdapter* adapter = NULL;
	for (UINT i = 0; !adapter; ++i)
	{
		if (FAILED(factory->EnumAdapters(i, &adapter)))
			break;
		int dev;
		if (cudaD3D11GetDevice(&dev, adapter) == cudaSuccess)
			break;
		adapter->Release();
	}
	factory->Release();

	RECT rc;
	GetClientRect(*hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = *hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(adapter, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swapChain, &dev, &g_featureLevel, &devcon);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;
	adapter->Release();

	cudaD3D11SetDirect3DDevice(dev);

	return hr;
}

HRESULT InitPlane(UINT width, UINT height)
{
	CUSTOMVERTEX* modelVertex;
	UINT* modelIndices;
	numVertex = height*width;//(height - 1)*(width - 1) * 6;
	modelVertex = new CUSTOMVERTEX[numVertex];
	numIndex = (height - 1)*(width - 1) * 6;
	modelIndices = new UINT[numIndex];
	/*float twidth = 1.0f / (float)width;
	float theight = 1.0f / (float)height;
	for (int i = 0; i < height - 1; ++i)
	{
		for (int j = 0; j < width - 1; ++j)
		{
			modelVertex[i * 6 * (width - 1) + j * 6].x = j*twidth;
			modelVertex[i * 6 * (width - 1) + j * 6].y = i*theight;

			modelVertex[i * 6 * (width - 1) + j * 6 + 1].x = j*twidth;
			modelVertex[i * 6 * (width - 1) + j * 6 + 1].y = (i+1)*theight;

			modelVertex[i * 6 * (width - 1) + j * 6 + 2].x = (j+1)*twidth;
			modelVertex[i * 6 * (width - 1) + j * 6 + 2].y = i*theight;

			modelVertex[i * 6 * (width - 1) + j * 6 + 3].x = j*twidth;
			modelVertex[i * 6 * (width - 1) + j * 6 + 3].y = (i + 1)*theight;

			modelVertex[i * 6 * (width - 1) + j * 6 + 4].x = (j + 1)*twidth;
			modelVertex[i * 6 * (width - 1) + j * 6 + 4].y = (i + 1)*theight;

			modelVertex[i * 6 * (width - 1) + j * 6 + 5].x = (j + 1)*twidth;
			modelVertex[i * 6 * (width - 1) + j * 6 + 5].y = i*theight;


			modelIndices[i * 6 * (width - 1) + j * 6] = i * 6 * (width - 1) + j * 6;
			modelIndices[i * 6 * (width - 1) + j * 6 + 1] = i * 6 * (width - 1) + j * 6 + 1;
			modelIndices[i * 6 * (width - 1) + j * 6 + 2] = i * 6 * (width - 1) + j * 6 + 2;

			modelIndices[i * 6 * (width - 1) + j * 6 + 3] = i * 6 * (width - 1) + j * 6 + 3;
			modelIndices[i * 6 * (width - 1) + j * 6 + 4] = i * 6 * (width - 1) + j * 6 + 4;
			modelIndices[i * 6 * (width - 1) + j * 6 + 5] = i * 6 * (width - 1) + j * 6 + 5;
		}
	}*/

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(CUSTOMVERTEX) * numVertex;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = 0;    // allow CPU to write in buffer
	bd.MiscFlags = 0;
	HRESULT hr;

	hr = dev->CreateBuffer(&bd, NULL, &vertexBuffer); 
	if (FAILED(hr))
		return hr;

	cudaGraphicsD3D11RegisterResource(&vertexBuffer_CUDA, vertexBuffer, cudaGraphicsRegisterFlagsNone);
	cudaGraphicsResourceSetMapFlags(vertexBuffer_CUDA, cudaGraphicsMapFlagsWriteDiscard);

	/*D3D11_BUFFER_DESC bd2;
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(unsigned int) * numIndex;
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd2.MiscFlags = 0;

	hr = dev->CreateBuffer(&bd2, NULL, &indexBuffer);
	if (FAILED(hr))
		return hr;*/
}

void release()
{
	cudaGraphicsUnregisterResource(vertexBuffer_CUDA);
	vertexBuffer->Release();
	devcon->Release();
	dev->Release();
}

void UpdateTime()
{
	//Take the frec read
	QueryPerformanceCounter(&g_L1);

	//Obtain delta frec
	long long Diff = g_L1.QuadPart - g_L0.QuadPart;

	//obtaint delta time in sec
	g_DeltaTime = (float)Diff / Fq.QuadPart;

	//Update current read
	g_L0 = g_L1;
}

void Update(HWND* hwnd)
{
}

void Draw()
{
	//Map vertex buffer for writing from CUDA
	float4* positions;
	cudaGraphicsMapResources(1, &vertexBuffer_CUDA, 0);
	size_t num_bytes;
	cudaGraphicsResourceGetMappedPointer((void**)&positions, &num_bytes, vertexBuffer_CUDA);

	//Execute Kernel
	dim3 dimBlock(16, 16, 1);
	dim3 dimGrid(g_width / dimBlock.x, g_height / dimBlock.y, 1);
	createVertices<<<dimGrid, dimBlock>>> (positions, time, width, height);

	//Unmap vertex buffer
	cudaGraphicsUnmapResources(1, &vertexBuffer_CUDA, 0);

	//Draw and present
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	devcon->IASetInputLayout(pLayout);
	devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
	Matrix wMatrix = cam.getViewMatrix() * cam.getProjectionMatrix();
	devcon->UpdateSubresource(pCBuffer, 0, 0, &wMatrix.m[0][0], 0, 0);
	UINT stride = sizeof(CUSTOMVERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(3, 0);
}

HRESULT InitPipeline()
{
	ID3D10Blob *VS, *PS;
	ID3D10Blob *VSerror, *PSerror;
	LoadShader(1, "shaders.shader", &VS);
	LoadShader(2, "shaders.shader", &PS);
	////Encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 64;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	dev->CreateBuffer(&bd, NULL, &pCBuffer);

	D3D11_RASTERIZER_DESC rd;
	// set the rasterizer properties here
	//rd.CullMode = D3D11_CULL_NONE;
	//rd.DepthClipEnable = FALSE;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_FRONT;
	rd.FrontCounterClockwise = FALSE;
	dev->CreateRasterizerState(&rd, &pRS);
}

int LoadShader(unsigned int Type, char* filename, ID3DBlob** shaderBlob)
{
	*shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// Load the shader source
	FILE * pf;
	if (fopen_s(&pf, filename, "rb") != 0)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif
	HRESULT hr;
	if (Type == 1)
		hr = D3DCompile(shaderSrc, size, NULL, NULL, NULL, "VShader", "vs_4_0", flags, 0, shaderBlob, &errorBlob);
	if (Type == 2)
		hr = D3DCompile(shaderSrc, size, NULL, NULL, NULL, "PShader", "ps_4_0", flags, 0, shaderBlob, &errorBlob);
	if (Type > 2)
		return -1;//TO DO:
	if (SUCCEEDED(hr))
	{
		if (errorBlob)
		{
			// The message is a 'warning' from the HLSL compiler.
			char const* message =
				static_cast<char const*>(errorBlob->GetBufferPointer());
			// Write the warning to the output window when the program is
			// executing through the Microsoft Visual Studio IDE.
			OutputDebugStringA(message);
			errorBlob->Release();
		}
		return 0;
	}
	else
	{
		if (errorBlob)
		{
			// The message is an 'error' from the HLSL compiler.
			char const* message =
				static_cast<char const*>(errorBlob->GetBufferPointer());
			// Write the warning to the output window when the program is
			// executing through the Microsoft Visual Studio IDE.
			OutputDebugStringA(message);
			errorBlob->Release();
		}
		return -1;
	}
}