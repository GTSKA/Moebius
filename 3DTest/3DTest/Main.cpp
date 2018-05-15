#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <cstdio>
#include "MemoryOperators.h"
#include "Vertex.h"
#include "MathUtil.h"
#include "Globals.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "ResourceManager.h"
#include "SceneManager.h"

using namespace std;


//include Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")



//global declarations
IDXGISwapChain *swapChain;			//pointer to the swap chain interface
ID3D11Device *dev;					//pointer to de Direct3D device interface
ID3D11DeviceContext *devcon;		//pointer to the Direct3D device context
ID3D11RenderTargetView *backbuffer;	//pointer to the back buffer
ID3D11Buffer *pVBuffer;
ID3D11Buffer *pCBuffer;
ID3D11InputLayout *pLayout;
ID3D11VertexShader *pVS;    // the vertex shader
ID3D11PixelShader *pPS;     // the pixel shader
ID3D11RasterizerState *pRS;    // global
ID3D11DepthStencilState * pDSState;
ID3D11DepthStencilView* pDSV;
ID3D11Texture2D* pDepthStencil = NULL;
LARGE_INTEGER g_L1;
LARGE_INTEGER g_L0;
LARGE_INTEGER Fq;
float angle;
float g_DeltaTime;
int keyPressed;
Shaders myShaders;
//Camera cam;
//Model woman;
//Texture texture;
static const float rotSpeed = 40.0f;
static const float radConversion = (float)M_PI / 180.0f;

HRESULT InitWindow(HWND *hWnd, HINSTANCE hInstance, int nCmdShow);
HRESULT Init(HWND *hWnd);
HRESULT InitPipeline();
HRESULT InitGraphics();
void Draw();
void Update(HWND* hwnd);
void Key(HWND* hwnd, unsigned char key, bool bIsPressed);
void CleanUp();
void Output(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);
	OutputDebugStringA(szBuff);
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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;//Window Handle

	if (FAILED(InitWindow(&hWnd, hInstance, nCmdShow)))
		return 0;
	if (FAILED(Init(&hWnd))) //Lesson 4
		return 0;
	//////////////Main loop:///////////////////////

	//Winndows event messages struct
	MSG msg;
	//Contains: HWND, message(UINT), wParam, lParam, time(DWORD)(exact time message was posted), pt(POINT)(Exact mouse position)
	//wait for the next message and store it in msg

	//cam.Init();
	//cam.calcWorldMatrix();
	//cam.calcViewMatrix();

	QueryPerformanceFrequency(&Fq);
	QueryPerformanceCounter(&g_L0);
	keyPressed = 0; //pressed keys flag
	while (TRUE)//It always returns TRUE, except for when the program is about to exit
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		UpdateTime();
		
		Update(&hWnd);
		Draw();
		
	}


	CleanUp();
	SceneManager::GetInstance()->Clean();
	SceneManager::DestroyInstance();
	ResourceManager::GetInstance()->Clean();
	ResourceManager::DestroyInstance();
			   //return wParam from WM_QUIT message to Windows
	MemoryDump();
	return msg.wParam;
}

//main message handler
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message,//we can use this to determine what message it is
	WPARAM wParam, LPARAM lParam)
{
	switch (message)//the switch statement looks for the code to run
	{
	case WM_KEYDOWN:
	{
		Key(&hWnd, unsigned char(wParam), true);
	}
		break;
	case WM_KEYUP:
	{
		Key(&hWnd, unsigned char(wParam), false);
	}
		break;
	case WM_DESTROY:
	{
		//close the application
		PostQuitMessage(0);
		return 0;
	}
	break;
	}

	//Handle messages not statement in the switch structure
	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT InitWindow(HWND *hWnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wc;//class info struct

	ZeroMemory(&wc, sizeof(WNDCLASSEXW));//clear the struct

										 //Filling the struct
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; //tell Windows to redraw the window if it is moved
	wc.lpfnWndProc = WindowProc; //what function to use when it gets a message from Windows (It doesn't matter what the function is called provided)
	wc.hInstance = hInstance; //Windows handed to us in WinMain
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //stores the default mouse image for the window class
											  //wc.hbrBackground = (HBRUSH)COLOR_WINDOW; //indicate the color of the background
	wc.lpszClassName = "WindowClass1"; // It doesn't matter what you name it, so long as you indicate it correctly when making the window itself

									   //register the window class
	RegisterClassEx(&wc);
	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	//create the window and set the result in hWnd
	*hWnd = CreateWindowEx(NULL, //dwExStyle, adds more options for the window style
		"WindowClass1", //name of the class our window will use
						//"Lesson 3 window",//Window name (it will be displayed in the window's title bar)
		"Our First Direct3D Program",//lesson 4 title
		WS_OVERLAPPEDWINDOW, //dwStyle, we can define all kinds of options for the window
		300,//X
		300,//Y
		SCREEN_WIDTH,//wr.right - wr.left,//Width
		SCREEN_HEIGHT,//wr.bottom - wr.top,//Height
		NULL,//Parent, no parent window -> NULL
			 /*Microsoft Word, which can open multiple documents in the same window, consists of a parent window with multiple 'child' windows.*/
		NULL,//Menu, not Menu -> NULL (This is a handle to a menu bar)
		hInstance, //Application Handle
		NULL);// used with multiple windows, NULL

	if (!(*hWnd))
		return E_FAIL;
	//display the window
	ShowWindow((*hWnd), nCmdShow);
	return S_OK;
}

void Update(HWND* hwnd)
{
//	//angle += (float)(g_DeltaTime * rotSpeed * radConversion);
//	if (keyPressed&(Globals::_KEY_A))
//	{
//		cam.moveLeft(g_DeltaTime);
//#ifdef _KEY_TEST
//		angleY += (float)(g_DeltaTime * rotSpeed * radConversion);//this test was for checking the correct key recognition
//#endif //_KEY_TEST
//	}
//	if (keyPressed&(Globals::_KEY_W))
//	{
//		cam.moveForward(g_DeltaTime);
//#ifdef _KEY_TEST
//		angleX += (float)(g_DeltaTime * rotSpeed * radConversion);
//#endif //_KEY_TEST
//	}
//	if (keyPressed&(Globals::_KEY_D))
//	{
//		cam.moveRight(g_DeltaTime);
//#ifdef _KEY_TEST
//		angleY -= (float)(g_DeltaTime * rotSpeed * radConversion);
//#endif //_KEY_TEST
//	}
//	if (keyPressed&(Globals::_KEY_S))
//	{
//		cam.moveBackward(g_DeltaTime);
//#ifdef _KEY_TEST
//		angleX -= (float)(g_DeltaTime * rotSpeed * radConversion);
//#endif //_KEY_TEST
//	}
//	if (keyPressed&(Globals::_KEY_LEFT))
//	{
//		cam.rotateLeft(g_DeltaTime);
//	}
//	if (keyPressed&(Globals::_KEY_UP))
//	{
//		cam.rotateUp(g_DeltaTime);
//	}
//	if (keyPressed&(Globals::_KEY_RIGHT))
//	{
//		cam.rotateRight(g_DeltaTime);
//	}
//	if (keyPressed&(Globals::_KEY_DOWN))
//	{
//		cam.rotateDown(g_DeltaTime);
//	}
	SceneManager::GetInstance()->Update(g_DeltaTime, keyPressed);
}

void Key(HWND* hwnd, unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
	{
		switch (key)
		{
		case Globals::KEY_A:
			keyPressed += (keyPressed&(Globals::_KEY_A)) ? 0 : Globals::_KEY_A;//If already pressed do nothing, else set the flag true
			break;
		case Globals::KEY_W:
			keyPressed += (keyPressed&(Globals::_KEY_W)) ? 0 : Globals::_KEY_W;
			break;
		case Globals::KEY_D:
			keyPressed += (keyPressed&(Globals::_KEY_D)) ? 0 : Globals::_KEY_D;
			break;
		case Globals::KEY_S:
			keyPressed += (keyPressed&(Globals::_KEY_S)) ? 0 : Globals::_KEY_S;
			break;
		case 37://left arrow key
			keyPressed += (keyPressed&(Globals::_KEY_LEFT)) ? 0 : Globals::_KEY_LEFT;
			break;
		case 38://up arrow key
			keyPressed += (keyPressed&(Globals::_KEY_UP)) ? 0 : Globals::_KEY_UP;
			break;
		case 39://right arrow key
			keyPressed += (keyPressed&(Globals::_KEY_RIGHT)) ? 0 : Globals::_KEY_RIGHT;
			break;
		case 40://down arrow key
			keyPressed += (keyPressed&(Globals::_KEY_DOWN)) ? 0 : Globals::_KEY_DOWN;
			break;
		/*case Globals::KEY_0:
			activeEffect = 0;
			break;
		case Globals::KEY_1:
			activeEffect = 1;
			break;
		case Globals::KEY_2:
			activeEffect = 2;
			break;
		case Globals::KEY_3:
			activeEffect = 3;
			break;
		case Globals::KEY_4:
			activeEffect = 4;
			break;*/

		}
	}
	else
	{
		switch (key)
		{
		case Globals::KEY_A:
			keyPressed -= (keyPressed&(Globals::_KEY_A)) ? Globals::_KEY_A : 0; //dismark flag only once
			break;
		case Globals::KEY_W:
			keyPressed -= (keyPressed&(Globals::_KEY_W)) ? Globals::_KEY_W : 0;
			break;
		case Globals::KEY_D:
			keyPressed -= (keyPressed&(Globals::_KEY_D)) ? Globals::_KEY_D : 0;
			break;
		case Globals::KEY_S:
			keyPressed -= (keyPressed&(Globals::_KEY_S)) ? Globals::_KEY_S : 0;
			break;
		case 37://left arrow key
			keyPressed -= (keyPressed&(Globals::_KEY_LEFT)) ? Globals::_KEY_LEFT : 0;
			break;
		case 38://up arrow key
			keyPressed -= (keyPressed&(Globals::_KEY_UP)) ? Globals::_KEY_UP : 0;
			break;
		case 39://right arrow key
			keyPressed -= (keyPressed&(Globals::_KEY_RIGHT)) ? Globals::_KEY_RIGHT : 0;
			break;
		case 40://down arrow key
			keyPressed -= (keyPressed&(Globals::_KEY_DOWN)) ? Globals::_KEY_DOWN : 0;
			break;
		}
	}
}



///////////////////////////////////////////////////////////////////DIRECT X////////////////////////////////////////////////
HRESULT Init(HWND *hWnd) 
{
	HRESULT hr = S_OK;

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
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swapChain, &dev, &g_featureLevel, &devcon);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;
	//get the adress of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	//set the render target as the back buffer
	//devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	

	

	/////////////////DEPTH STENCIL////////////////////
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = sd.BufferDesc.Width;
	descDepth.Height = sd.BufferDesc.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = dev->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		return hr;
	/*D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = dev->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(hr))
		return hr;
	devcon->OMSetDepthStencilState(pDSState, 1);*/
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = dev->CreateDepthStencilView(pDepthStencil, &descDSV, &pDSV);
	if (FAILED(hr))
		return hr;
	devcon->OMSetRenderTargets(1, &backbuffer, pDSV);
	//////////////////////////////////////////

	//////////////////set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 0.99f;

	devcon->RSSetViewports(1, &viewport);

	//if (FAILED(InitPipeline()))
	//	return S_FALSE;
	if (FAILED(InitGraphics()))
		return S_FALSE;
	///////////////

	


	return S_OK;
}
HRESULT InitPipeline()
{
	HRESULT hr;
	hr = S_OK;
	//load and compile the two shaders
	ID3D10Blob *VS, *PS;
	ID3D10Blob *VSerror, *PSerror;
	hr = D3DX11CompileFromFile("shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &VSerror, 0);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)VSerror->GetBufferPointer());
		return hr;
	}
	hr = D3DX11CompileFromFile("shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &PSerror, 0);

	//Encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	//set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,64,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 64;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	dev->CreateBuffer(&bd, NULL, &pCBuffer);
	devcon->VSSetConstantBuffers(0, 1, &pCBuffer);

	D3D11_RASTERIZER_DESC rd;
	// set the rasterizer properties here
	//rd.CullMode = D3D11_CULL_NONE;
	//rd.DepthClipEnable = FALSE;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_FRONT;
	rd.FrontCounterClockwise = FALSE;
	dev->CreateRasterizerState(&rd, &pRS);

	
	return hr;
}
HRESULT InitGraphics()
{
	HRESULT hr = S_OK;
	// create a triangle using the VERTEX struct
	/*VERTEX OurVertices[3]
	{
		{ Vector3(0.0f, 0.5f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(0.45f, -0.5, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3 (-0.45f, -0.5f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
	*/

	/*if (MODEL_SUCCESS != woman.InitModel("../Resources/Models/Woman1.nfg", dev, devcon))
		hr = S_FALSE;
	texture.setWrapMode(REPEAT);
	if (!texture.Init("../Resources/Textures/Woman1.tga",dev,devcon))
		hr = S_FALSE;
 	int res = myShaders.Init("../Resources/Shaders/TriangleShaderVS.hlsl", "../Resources/Shaders/TriangleShaderPS.glsl", dev, devcon);*/
	//int res = myShaders.Init("../Resources/Shaders/TerrainShaderVS.hlsl", "../Resources/Shaders/TerrainShaderPS.glsl", dev, devcon);
	bool resourceManagerResult;
	resourceManagerResult = ResourceManager::GetInstance()->Init("../Resources/RM.txt",dev,devcon);
	if (!resourceManagerResult)
	{
		OutputDebugString("Error while loading resources\n");
		return S_FALSE;
	}
	bool sceneManagerResult;
	sceneManagerResult = SceneManager::GetInstance()->Init("../Resources/SM.txt");
	if (!sceneManagerResult)
	{
		OutputDebugString("Error while loading scene\n");
		return S_FALSE;
	}
	return hr;
}
void Draw()
{
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	devcon->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//Matrix scaleMatrix;
	//Matrix rotationXMatrix;
	//Matrix rotationYMatrix;
	//Matrix rotationZMatrix;
	//Matrix translataionMatrix;
	//Matrix worldMatrix;

	//scaleMatrix.SetIdentity();
	//rotationXMatrix.SetIdentity();
	//rotationYMatrix.SetIdentity();
	//rotationZMatrix.SetIdentity();
	//
	//rotationYMatrix.SetRotationY(angle);

	//translataionMatrix.SetIdentity();
	//scaleMatrix.SetScale(0.8f);
	//worldMatrix = scaleMatrix * rotationZMatrix * rotationXMatrix * rotationYMatrix * translataionMatrix;
	////D3DXMATRIX matView, matProj;
	////D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(-1.0, 3.0, 5.0), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	////D3DXMatrixPerspectiveFovLH(&matProj, 1, (float)Globals::screenWidth / Globals::screenHeight, 0.1, 500);
	//
	//Matrix wMatrix = worldMatrix * cam.getViewMatrix() * cam.getProjectionMatrix();
	////D3DXMATRIX matWorld = D3DXMATRIX(&worldMatrix.m[0][0]);
	////D3DXMATRIX matwMatrix = matWorld * matView * matProj;
	////devcon->UpdateSubresource(pCBuffer, 0, 0, &wMatrix.m[0][0],0,0);
	//

	//D3D11_MAPPED_SUBRESOURCE ms;
	//devcon->Map(myShaders.getVertexConstBufferbyIndex(0), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	//memcpy(ms.pData, &wMatrix.m[0][0], 16*sizeof(float));                 // copy the data
	//devcon->Unmap(myShaders.getVertexConstBufferbyIndex(0), NULL);
	//
	////do 3D rendering on the back buffer here
	//// select which vertex buffer to display
	//
	////devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	//// select which primtive type we are using
	////devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//// draw the vertex buffer to the back buffer
	////devcon->Draw(3, 0);
	////devcon->RSSetState(pRS);
	//UINT stride = sizeof(VERTEX);
	//UINT offset;
	///*UINT* stride = new UINT[myShaders.getVSinputsize()];
	//UINT* offset = new UINT[myShaders.getVSinputsize()];
	//ID3D11Buffer** vertexbuffers = new ID3D11Buffer*[myShaders.getVSinputsize()];
	//for (int i = 0; i < myShaders.getVSinputsize(); ++i)
	//{
	//	stride[i] = sizeof(VERTEX);
	//	vertexbuffers[i] = woman.m_vertexBuffer;
	//}*/
	////offset[0] = 0;
	////offset[1] = 64;
	////devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	////for (int i = 0; i < myShaders.getVSinputsize(); ++i)
	//offset = 0;
	//devcon->IASetVertexBuffers(0, 1, &woman.m_vertexBuffer, &stride, &offset);
	//offset = 64;
	//devcon->IASetVertexBuffers(1, 1, &woman.m_vertexBuffer, &stride, &offset);

	////delete[] stride;
	////delete[] offset;
	////delete[] vertexbuffers;
	///*offset = 12;
	//devcon->IASetVertexBuffers(1, 1, &woman.m_vertexBuffer, &stride, &offset);
	//offset = 64;
	//devcon->IASetVertexBuffers(2, 1, &woman.m_vertexBuffer, &stride, &offset);*/

	////stride = 4 * sizeof(float);
	////offset = 12;
	////devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	////devcon->IASetVertexBuffers(12, 1, &woman.m_vertexBuffer, &stride, &offset);

	//devcon->IASetIndexBuffer(woman.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	////devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////devcon->Draw(3, 0);
	//devcon->PSSetShaderResources(0, 1, &texture.m_TextureView);
	//devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//devcon->DrawIndexed(woman.getIndexSize(), 0, 0);

	SceneManager::GetInstance()->Draw(devcon);

	//switch the back buffer and front buffer
	swapChain->Present(0, 0);
}


void CleanUp()
{
	swapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
												   //close and release all existing COM objects
	pVS->Release();
	pPS->Release();
	pCBuffer->Release();
	pRS->Release();
	pLayout->Release();
	pVBuffer->Release();
	pDSState->Release();
	pDSV->Release();
	pDepthStencil->Release();
	swapChain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();

}