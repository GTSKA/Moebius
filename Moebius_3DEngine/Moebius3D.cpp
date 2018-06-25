#include "StdAfx.h"
#include "Moebius3D.h"
#include "resource.h"

//CRITICAL_SECTION    _moebius_g_cs;
//bool                _moebius_g_bThreadSafe = true;

CRITICAL_SECTION    __moebius_g_cs;
bool                __moebius_g_bThreadSafe;

inline CMoebius3D::CMoebius_3DLock_::CMoebius_3DLock_()  { if( __moebius_g_bThreadSafe ) EnterCriticalSection( &__moebius_g_cs ); }
inline CMoebius3D::CMoebius_3DLock_::~CMoebius_3DLock_() { if( __moebius_g_bThreadSafe ) LeaveCriticalSection( &__moebius_g_cs ); }

CMoebius3D::CMoebius3D_State__::CMoebius3D_State__()
{
	__moebius_g_bThreadSafe = true;
    InitializeCriticalSectionAndSpinCount(&__moebius_g_cs, 1000);

	//Initialize the STATE struct,....
	ZeroMemory(&m_State, sizeof(STATE));

	m_State.m_DeviceWindow = NULL;
	m_State.m_ShowFullScreenOptions = false;
	m_State.m_D3DSDKVERSION = 0;

	m_State.m_DevSuport_D3DCREATE_MIXED_VERTEXPROCESSING = false;
	m_State.m_VertexProccessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	m_State.m_DevSuport_D3DPTEXTURECAPS_CUBEMAP = false;
	m_State.m_DevSuport_D3DPTEXTURECAPS_VOLUMEMAP = false;
	m_State.m_DevSuport_MaxVS_Version = 0.0f;
	m_State.m_DevSuport_MaxPS_Version = 0.0f;	
	m_State.m_SurfaceFormat = D3DFMT_X8R8G8B8;
	m_State.m_DepthFormat = D3DFMT_D16;
	m_State.m_SupportDepthStencilBuffer = false;
	m_State.m_MultiSampleIndex = 0;	
	m_State.m_MultiSampleQuality = 0;
	m_State.m_ScreenResolutionIndex = 0;

	m_State.m_StrAdpterDescription = new WCHAR[512];
	m_State.m_StrSurfaceFormat = new WCHAR[512];
	m_State.m_StrDepthFormat = new WCHAR[512];
	m_State.m_StrLongDeviceInfo = new WCHAR[512];
	
	m_State.m_Maximized = false;
	m_State.m_Minimized = false;
	m_State.m_WindowedMode = false;

	m_State.m_CreatedDevice = false;
	m_State.m_LostDevice = false;
	m_State.m_PauseTimer = false;
	m_State.m_PauseRender = false;

	m_State.m_Time = 0;
	m_State.m_AbsTime =  0;					
	m_State.m_ElapsedTime = 0;				
	m_State.m_FPS = 0;			
}

CMoebius3D::CMoebius3D_State__::~CMoebius3D_State__()
{
	SAFE_DELETE_ARRAY(m_State.m_StrAdpterDescription);
	SAFE_DELETE_ARRAY(m_State.m_StrSurfaceFormat);
	SAFE_DELETE_ARRAY(m_State.m_StrDepthFormat);
	SAFE_DELETE_ARRAY(m_State.m_StrLongDeviceInfo);

    DeleteCriticalSection(&__moebius_g_cs);
}

void CMoebius3D::CMoebius3D_State__::SetKey(int nKey, bool value)
{
	assert(nKey >= 0 && nKey < 256); 
	//CMoebius_GraphicsLock l; 
	m_State.m_Keys[nKey] = value;
}

void CMoebius3D::CMoebius3D_State__::AddResolutionInfo(D3DDISPLAYMODE dspM)
{
	bool bAdd = true;
	if(m_deviceCaps.m_ResolutionList.GetSize() > 0)
	{
		for(UINT i=0; i<m_deviceCaps.m_ResolutionList.GetSize(); i++)
		{
			if(m_deviceCaps.m_ResolutionList[i].m_dMode.Width == dspM.Width &&
				m_deviceCaps.m_ResolutionList[i].m_dMode.Height == dspM.Height)
			{
				bAdd = false;
				break;
			}
		}
	}
	if(bAdd)
	{
		m_deviceCaps.m_ResolutionList.Add();
		m_deviceCaps.m_ResolutionList[m_deviceCaps.m_ResolutionList.GetSize()-1].m_dMode = dspM;
		StringCchPrintf(m_deviceCaps.m_ResolutionList[m_deviceCaps.m_ResolutionList.GetSize()-1].m_sResolution,
			255 * sizeof(WCHAR), L"%d, %d", dspM.Width, dspM.Height);
		StringCchPrintf(m_deviceCaps.m_ResolutionList[m_deviceCaps.m_ResolutionList.GetSize()-1].m_sRefreshRate,
			255 * sizeof(WCHAR), L"%d", dspM.RefreshRate);
		GetSurfaceFormatName(m_deviceCaps.m_ResolutionList[m_deviceCaps.m_ResolutionList.GetSize()-1].m_sFormatName,
			255 * sizeof(WCHAR), dspM.Format);
	}
}

void CMoebius3D::CMoebius3D_State__::AddMultisampleInfo(D3DMULTISAMPLE_TYPE multiSample, UINT maxQLevels)
{
	m_deviceCaps.m_MultySampleList.Add();
	m_deviceCaps.m_MultySampleList[m_deviceCaps.m_MultySampleList.GetSize()-1].m_multisapleType = multiSample;
	m_deviceCaps.m_MultySampleList[m_deviceCaps.m_MultySampleList.GetSize()-1].m_maxQLevels = maxQLevels; 
	StringCchPrintf(m_deviceCaps.m_MultySampleList[m_deviceCaps.m_MultySampleList.GetSize()-1].m_sMaxQLevels,
		255 * sizeof(WCHAR), L"%d", maxQLevels);
	GetMultiSampleName(m_deviceCaps.m_MultySampleList[m_deviceCaps.m_MultySampleList.GetSize()-1].m_sFormatMultisapleName,
		255 * sizeof(WCHAR), multiSample);
}

//--------------------------------------------------------------------------------------
//General function definitions
//--------------------------------------------------------------------------------------
HRESULT IsSurfaceFormatValidOnFullScreen(D3DFORMAT sFormat)
{
	switch(sFormat)
	{
	case D3DFMT_A2R10G10B10:
		return TRUE;
	case D3DFMT_X8R8G8B8:
		return TRUE;
	case D3DFMT_X1R5G5B5:
		return TRUE;
	case D3DFMT_R5G6B5:
		return TRUE;
	default:
		return FALSE;
	}
}

void GetSurfaceFormatName(STRSAFE_LPWSTR strName, UINT strNameSize ,D3DFORMAT sFormat)
{
	if(strName != NULL)
	{
		switch(sFormat)
		{
		case D3DFMT_A2R10G10B10:
			StringCchPrintfW(strName, strNameSize * sizeof(WCHAR), L"A2R10G10B10");
			break;
		case D3DFMT_A8R8G8B8:
			StringCchPrintfW(strName, strNameSize * sizeof(WCHAR), L"A8R8G8B8");
			break;
		case D3DFMT_X8R8G8B8:
			StringCchPrintfW(strName, strNameSize * sizeof(WCHAR), L"X8R8G8B8");
			break;
		case D3DFMT_A1R5G5B5:
			StringCchPrintfW(strName, strNameSize * sizeof(WCHAR), L"A1R5G5B5");
			break;
		case D3DFMT_X1R5G5B5:
			StringCchPrintfW(strName, strNameSize * sizeof(WCHAR), L"X1R5G5B5");
			break;
		case D3DFMT_R5G6B5:
			StringCchPrintfW(strName, strNameSize * sizeof(WCHAR), L"R5G6B5");
			break;
		default:
			StringCchPrintfW(strName, strNameSize * sizeof(WCHAR), L"");
			break;
		}
	}
}

void GetDephtFormatName(STRSAFE_LPWSTR strName, UINT strNameSize ,D3DFORMAT sFormat)
{
	if(strName != NULL)
	{
		switch(sFormat)
		{
		case D3DFMT_D16:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"D16");
			break;
		case D3DFMT_D24S8:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"D24S8");
			break;
		case D3DFMT_D24X8:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"D24X8");
			break;
		}
	}
	else
	{
		StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"NULL string");
	}
}


void GetMultiSampleName(STRSAFE_LPWSTR strName, UINT strNameSize ,D3DMULTISAMPLE_TYPE sMultiSample)
{
	if(strName != NULL)
	{
		switch(sMultiSample)
		{
		case D3DMULTISAMPLE_NONE:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"No Multisampling");
			break;
		case D3DMULTISAMPLE_2_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"2 Samples");
			break;
		case D3DMULTISAMPLE_3_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"3 Samples");
			break;
		case D3DMULTISAMPLE_4_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"4 Samples");
			break;
		case D3DMULTISAMPLE_5_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"5 Samples");
			break;
		case D3DMULTISAMPLE_6_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"6 Samples");
			break;
		case D3DMULTISAMPLE_7_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"7 Samples");
			break;
		case D3DMULTISAMPLE_8_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"8 Samples");
			break;
		case D3DMULTISAMPLE_9_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"9 Samples");
			break;
		case D3DMULTISAMPLE_10_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"10 Samples");
			break;
		case D3DMULTISAMPLE_11_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"11 Samples");
			break;
		case D3DMULTISAMPLE_12_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"12 Samples");
			break;
		case D3DMULTISAMPLE_13_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"13 Samples");
			break;
		case D3DMULTISAMPLE_14_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"14 Samples");
			break;
		case D3DMULTISAMPLE_15_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"15 Samples");
			break;
		case D3DMULTISAMPLE_16_SAMPLES:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"16 Samples");
			break;
		case D3DMULTISAMPLE_NONMASKABLE:
			StringCchPrintf(strName, strNameSize * sizeof(WCHAR), L"No Mask Enable");
			break;
		}
	}
}

CMoebius3D::CMoebius3D(void)
{
	_State = new ::CMoebius3D::CMoebius3D_State__();
	
	dwFrames = 0;
	prevTime = 0;
	_FPS = 0;
	_Time = 0;
	_AbsTime = 0;
	_ElapsedTime = 0;

	m_FrameStats = new WCHAR[255];
}

CMoebius3D::~CMoebius3D(void)
{
	SAFE_DELETE(m_FrameStats);

	SAFE_DELETE(_State);
	SAFE_DELETE(m_pD3D);
}

//HRESULT CMoebius3D::CheckDevice(HINSTANCE hInstance, HWND hWnd, bool bWindowed)
HRESULT CMoebius3D::CheckDevice(HWND hWnd, bool bWindowed)
{
	 _State->SetCheckedDevice(false);

	//Check if the window is valid
	//---------------------------------------------------------------------------------------------------
	if(hWnd == NULL)
	{
		MessageBoxA(NULL, "The window must not be NULL", "Device Error", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	//Save the window handle
	 _State ->SetDeviceWindow(hWnd);

	////Save the current instance
	////---------------------------------------------------------------------------------------------------
	//if(m_hInstance)
	//{
	//	MessageBoxA(NULL, "Error in the Instance handle", "Device Error", MB_OK|MB_ICONERROR);
	//	return E_FAIL;
	//}
	////Save the current instance
	//m_hInstance = hInstance;

	//The device is windowed mode?...
	//---------------------------------------------------------------------------------------------------
	 _State ->SetWindowedMode(bWindowed);

	 

	//Create the D3DX Foregraund
	//---------------------------------------------------------------------------------------------------
	m_pD3D = NULL;
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		MessageBoxA(NULL, "Error in Direct3DCreate9", "Device Error", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	if(FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, &devCaps)))
	{
		MessageBoxA(NULL, "No able to get the device caps", "Device Error", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	//Try with D3DFMT_X8R8G8B8 for Adpater Format and 
	if(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
                                     D3DDEVTYPE_HAL,
									 D3DFMT_X8R8G8B8,
									 D3DUSAGE_DEPTHSTENCIL,
                                     D3DRTYPE_SURFACE,
									 D3DFMT_X8R8G8B8))
	{
		if(!IsGetDepthFormatValid(D3DFMT_D24S8, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8))
		{
			//Test for depth buffer format D3DFMT_D16
			if(IsGetDepthFormatValid(D3DFMT_D24S8, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8))
			{	
				 _State ->SetSupportDepthStencilBuffer(false);
				 _State ->SetDepthFormat(D3DFMT_D16);
				 _State ->SetSurfaceFormat(D3DFMT_X8R8G8B8);
			}
		}
		else
		{
			 _State ->SetSupportDepthStencilBuffer(true);
			 _State ->SetDepthFormat(D3DFMT_D24S8);
			 _State ->SetSurfaceFormat(D3DFMT_X8R8G8B8);
		}
	}
	else
	{
		return E_FAIL;
	}

	//Check for the surface y depht buffers
	//if( _State ->GetWindowedMode())
	//{
	//	//Try with D3DFMT_X8R8G8B8
	//	if(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
 //                                        D3DDEVTYPE_HAL,
	//									 D3DFMT_X8R8G8B8,
	//									 D3DUSAGE_DEPTHSTENCIL,
 //                                        D3DRTYPE_SURFACE,
	//									 D3DFMT_X8R8G8B8))
	//	{
	//		if(!IsGetDepthFormatValid(D3DFMT_D24S8, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8))
	//		{
	//			//Test for depth buffer format D3DFMT_D16
	//			if(IsGetDepthFormatValid(D3DFMT_D24S8, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8))
	//			{	
	//				 _State ->SetSupportDepthStencilBuffer(false);
	//				 _State ->SetDepthFormat(D3DFMT_D16);
	//				 _State ->SetSurfaceFormat(D3DFMT_X8R8G8B8);
	//			}
	//		}
	//		else
	//		{
	//			 _State ->SetSupportDepthStencilBuffer(true);
	//			 _State ->SetDepthFormat(D3DFMT_D24S8);
	//			 _State ->SetSurfaceFormat(D3DFMT_X8R8G8B8);
	//		}
	//	}
	//	else
	//	{
	//		return E_FAIL;
	//	}
	//}
	//else
	//{
	//	
	//	//Try with D3DFMT_A8R8G8B8
	//	if(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
 //                                        D3DDEVTYPE_HAL,
	//									 D3DFMT_A8R8G8B8,
	//									 D3DUSAGE_DEPTHSTENCIL,
 //                                        D3DRTYPE_SURFACE,
	//									 D3DFMT_A8R8G8B8))
	//	{
	//		
	//		if(!IsGetDepthFormatValid(D3DFMT_D24S8, D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8))
	//		{
	//			
	//			//Test for depth buffer format D3DFMT_D16
	//			if(IsGetDepthFormatValid(D3DFMT_D16, D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8))
	//			{	
	//				 _State ->SetSupportDepthStencilBuffer(false);
	//				 _State ->SetDepthFormat(D3DFMT_D16);
	//				 _State ->SetSurfaceFormat(D3DFMT_A8R8G8B8);
	//			}
	//			else
	//			{
	//				return E_FAIL;
	//			}
	//		}
	//		else
	//		{
	//			//MessageBox(NULL, L"STAR UP", 0,0);	
	//			 _State ->SetSupportDepthStencilBuffer(true);
	//			 _State ->SetDepthFormat(D3DFMT_D24S8);
	//			 _State ->SetSurfaceFormat(D3DFMT_A8R8G8B8);
	//		}
	//	}
	//	else
	//	{
	//		//Now try with D3DFMT_X8R8G8B8
	//		if(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
	//										 D3DDEVTYPE_HAL,
	//										 D3DFMT_X8R8G8B8,
	//										 D3DUSAGE_DEPTHSTENCIL,
	//										 D3DRTYPE_SURFACE,
	//										 D3DFMT_X8R8G8B8))
	//		{
	//			if(!IsGetDepthFormatValid(D3DFMT_D24S8, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8))
	//			{
	//				//Test for depth buffer format D3DFMT_D16
	//				if(IsGetDepthFormatValid(D3DFMT_D24S8, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8))
	//				{	
	//					 _State ->SetSupportDepthStencilBuffer(false);
	//					 _State ->SetDepthFormat(D3DFMT_D16);
	//					 _State ->SetSurfaceFormat(D3DFMT_X8R8G8B8);
	//				}
	//				else
	//				{
	//					return E_FAIL;
	//				}
	//			}
	//			else
	//			{
	//				 _State ->SetSupportDepthStencilBuffer(true);
	//				 _State ->SetDepthFormat(D3DFMT_D24S8);
	//				 _State ->SetSurfaceFormat(D3DFMT_X8R8G8B8);
	//			}
	//		}
	//		else
	//		{
	//			return E_FAIL;
	//		}
	//	}
	//}

	

	GetSurfaceFormatName( _State ->GetStrSurfaceFormat(), 512 * sizeof(WCHAR),  _State ->GetSurfaceFormat());
	GetDephtFormatName( _State ->GetStrDepthFormat(), 512 * sizeof(WCHAR),  _State ->GetDepthFormat());

	//Now we have a correct back and deep buffer for the current adapter, we enumerate all de screen resolutions
	UINT nAdapterCount = m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,  D3DFMT_X8R8G8B8);
	D3DDISPLAYMODE dspM;
	if(nAdapterCount > 0)
	{
		for(UINT iC = 0; iC< nAdapterCount; iC++)
		{
			if(SUCCEEDED(m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,  D3DFMT_X8R8G8B8, iC, &dspM)))
			{
				//We add a new resolution
				 _State ->AddResolutionInfo(dspM);
			}
		}
	}

	//We find the list of SamplerStates
	DWORD maxQLevels;
	for(UINT i=0; i<MultisampleListSize; i++)
	{
		if(SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,  _State ->GetSurfaceFormat(), bWindowed, 
			MultisampleList[i], &maxQLevels)))
		{
			 _State ->AddMultisampleInfo(MultisampleList[i], maxQLevels);
		}
	}

	//Get the vertex proccessing
	//---------------------------------------------------------------------------------------------------
	if((devCaps.DevCaps & D3DDEVCAPS_PUREDEVICE))
	{
		_State ->SetVertexProccessing(D3DCREATE_MIXED_VERTEXPROCESSING);
		_State ->SetDevSuport_D3DCREATE_MIXED_VERTEXPROCESSING(true);
	}
	
	//The device, supports pure cube maps ?
	//---------------------------------------------------------------------------------------------------
    if(devCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP)
	{
		 _State ->SetDevSuport_D3DPTEXTURECAPS_CUBEMAP(true);
	}
	else
	{
		MessageBoxA(NULL, "The device must support cube maps", "Device Error", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	//The device, supports volumtric maps ?
	//---------------------------------------------------------------------------------------------------
	if(devCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP)
	{
		 _State ->SetDevSuport_D3DPTEXTURECAPS_VOLUMEMAP(true);
	}

	//Get the top vertex-shader version
	//---------------------------------------------------------------------------------------------------
	_State ->SetDevSuport_MaxVS_Version(0.0);
	for(UINT i=3; i>=1; i--)
	{
		for(int j=9; j>=0; j--)
		{
			if(devCaps.VertexShaderVersion == D3DVS_VERSION( i, j ))
			{
				 _State ->SetDevSuport_MaxVS_Version(i + ((float)(0.1) * j));
				break;
			}
		}
	}
	if(_State ->GetDevSuport_MaxVS_Version()<1)
	{
		MessageBoxA(NULL, "The device must support at least Vertex Shader 1.0", "Device Error", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	//Get the top pixel-shader version
	//---------------------------------------------------------------------------------------------------
	_State ->SetDevSuport_MaxPS_Version(0.0);
	for(UINT i=3; i>=1; i--)
	{
		for(int j=9; j>=0; j--)
		{
			if(devCaps.PixelShaderVersion < D3DPS_VERSION( i, j ))
			{
				 _State ->SetDevSuport_MaxPS_Version(i + ((float)(0.1) * j));
				break;
			}
		}
	}
	//Check the pixel shader version
	//---------------------------------------------------------------------------------------------------
	if(_State ->GetDevSuport_MaxPS_Version()<1)
	{
		MessageBoxA(NULL, "The device must support at least Pixel Shader 1.0", "Device Error", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	//Get the Adapter Information
	//---------------------------------------------------------------------------------------------------
	ZeroMemory(&m_AdapterInfo, sizeof(m_AdapterInfo));
	m_pD3D->GetAdapterIdentifier(devCaps.AdapterOrdinal, 0, &m_AdapterInfo);
	MULTI_TO_WIDE(m_AdapterInfo.Description,  _State ->GetStrAdpterDescription());

	//Write the long device information
	//---------------------------------------------------------------------------------------------------
	StringCchPrintf( _State ->GetStrLongDeviceInfo(), 512 * sizeof(WCHAR), L"%s, %s, %s, VS %lf, PS %lf", 
		 _State ->GetStrAdpterDescription(), 
		 _State ->GetStrSurfaceFormat(), 
		 _State ->GetStrDepthFormat(), 
		 _State ->GetDevSuport_MaxVS_Version(), 
		 _State ->GetDevSuport_MaxPS_Version());
	
	//Everything is OK
	//---------------------------------------------------------------------------------------------------
	 _State ->SetCheckedDevice(true);
	return S_OK;
}

HRESULT CMoebius3D::CreateDevice()
{
	_State ->SetCreatedDevice(false);
	if(_State ->GetCheckedDevice())
	{
		//Check de user device caps
		if(SUCCEEDED(CheckDeviceCaps()))
		{
			//Get de device resolution...
			ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );

			//Set the windows mode
			//---------------------------------------------------------------------------------------------------
			m_d3dpp.Windowed					= _State ->GetWindowedMode();

			if(!_State->GetWindowedMode())
			{
				//Is full screen we must provide a valid screen resolution,...
				m_d3dpp.BackBufferWidth = _State->m_deviceCaps.m_ResolutionList[_State ->GetScreenResolutionIndex()].m_dMode.Width;
				m_d3dpp.BackBufferHeight = _State->m_deviceCaps.m_ResolutionList[_State ->GetScreenResolutionIndex()].m_dMode.Height;
			}

			_State->SetScreenWidth(m_d3dpp.BackBufferWidth);
			_State->SetScreenHeight(m_d3dpp.BackBufferHeight);

			//Set swap effect value (Default: D3DSWAPEFFECT_DISCARD)
			//---------------------------------------------------------------------------------------------------
			m_d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;

			//Presentation interval (Default: D3DPRESENT_INTERVAL_IMMEDIATE)
			//---------------------------------------------------------------------------------------------------
			m_d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

			//Set the backbuffer o surface format
			//---------------------------------------------------------------------------------------------------
			m_d3dpp.BackBufferFormat			= _State ->GetSurfaceFormat();

			//Set the depth format and enable aunto depthstencil, if it´s pocible
			//---------------------------------------------------------------------------------------------------
			m_d3dpp.EnableAutoDepthStencil		= _State ->GetSupportDepthStencilBuffer();
			m_d3dpp.AutoDepthStencilFormat		= _State ->GetDepthFormat();

			//Set the multisample values
			//---------------------------------------------------------------------------------------------------
			m_d3dpp.MultiSampleType				= _State ->m_deviceCaps.m_MultySampleList[_State ->GetMultiSampleIndex()].m_multisapleType;
			m_d3dpp.MultiSampleQuality			= _State ->GetMultiSampleQuality();

			//Now we create the device object!!!
			//---------------------------------------------------------------------------------------------------
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
						  _State ->GetDeviceWindow(),
						  _State ->GetVertexProccessing(),
                          &m_d3dpp, &m_pd3dDevice )))
			{
				MessageBox(NULL, L"Error creating the D3D device object", L"Device Error", MB_OK|MB_ICONERROR);
				return E_FAIL;
			}

			if(FAILED(OnInitDeviceObjects()))
			{
				MessageBox(NULL, L"Error in OnIninitDeviceObjects() function", L"Device Error", MB_OK|MB_ICONERROR);
				return E_FAIL;
			}
			if(FAILED(OnRestoreDeviceObjects()))
			{
				MessageBox(NULL, L"Error in OnRestoreDeviceObjects() function", L"Device Error", MB_OK|MB_ICONERROR);
				return E_FAIL;
			}

			Pause(false, false);

			//Everything is OK
			//---------------------------------------------------------------------------------------------------
			_State ->SetCreatedDevice(true);

			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
		
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT CMoebius3D::IsGetDepthFormatValid(D3DFORMAT DepthFormat, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat)
{
    
    // Verify that the depth format exists
    if(FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
                                         D3DDEVTYPE_HAL,
                                         AdapterFormat,
                                         D3DUSAGE_DEPTHSTENCIL,
                                         D3DRTYPE_SURFACE,
                                         DepthFormat)))
	{
		return FALSE;
	}

	//Verify that the depth format is compatible
	if(FAILED(m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
                                      D3DDEVTYPE_HAL,
                                      AdapterFormat,
                                      BackBufferFormat,
                                      DepthFormat)))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}



CMoebius_ResolutionDesc_& CMoebius3D::GetValidResolution(UINT index)
{
	return  _State ->m_deviceCaps.m_ResolutionList[index];
}
CMoebius_MultisapleInfo_& CMoebius3D::GetMultisample(UINT index)
{
	return  _State ->m_deviceCaps.m_MultySampleList[index];
}


void CMoebius3D::GetDesktopResolution(int *widht, int *height, UINT *validIndex)
{
	(*widht) = GetSystemMetrics(SM_CXSCREEN);
	(*height) = GetSystemMetrics(SM_CYSCREEN);

	for(UINT i=0; i< _State ->m_deviceCaps.m_ResolutionList.GetSize(); i++)
	{
		if( _State ->m_deviceCaps.m_ResolutionList[i].m_dMode.Width == (*widht) &&
			 _State ->m_deviceCaps.m_ResolutionList[i].m_dMode.Height == (*height))
		{
			(*validIndex) = i;
			break;
		}
	}
}
UINT CMoebius3D::GetNumberValidResolutions()
{
	return  _State ->m_deviceCaps.m_ResolutionList.GetSize();
}


UINT CMoebius3D::GetNumberMultisamplerTypes()
{
	return  _State ->m_deviceCaps.m_MultySampleList.GetSize();
}

UINT CMoebius3D::GetCurrentResolutionIndex()
{
	return _State->GetScreenResolutionIndex();
}
UINT CMoebius3D::GetCurrentMultisamplerTypeIndex()
{
	return _State->GetMultiSampleIndex();
}
UINT CMoebius3D::GetCurrentMultisamplerQuality()
{
	return _State->GetMultiSampleQuality();
}
void CMoebius3D::SetCurrentResolutionIndex(UINT index)
{
	_State->SetScreenResolutionIndex(index);
}
void CMoebius3D::SetCurrentMultisamplerTypeIndex(UINT index)
{
	_State->SetMultiSampleIndex(index);
}
void CMoebius3D::SetCurrentMultisamplerQuality(UINT value)
{
	_State->SetMultiSampleQuality(value);
}
WCHAR* CMoebius3D::GetStrLongDeviceDescription()
{
	return  _State ->GetStrLongDeviceInfo();
}

HRESULT CMoebius3D::OnInitDeviceObjects()
{
	//Create the default font
	//-------------------------------------------------------------------------------
	if(FAILED(D3DXCreateFont( m_pd3dDevice, 14, 0, FW_BOLD, 0, FALSE, 
		                 DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		                 DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), 
		                 &m_pd3dxFont )))
	{
		MessageBoxA(NULL, "Error creating the default font", "ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	//Create the default sprite
	//-------------------------------------------------------------------------------
	if(FAILED(m_sprite.CreateSprite(m_pd3dDevice)))
	{
		MessageBoxA(NULL, "Error creating the default sprite", "ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	//Add white, defautl texture to the texture list
	//-------------------------------------------------------------------------------
	m_textureList.AddTexture(D3DXCOLOR(1,1,1,1));

	m_effectList.AddEffect(m_pd3dDevice, _MOEBIUS_EFFECT_BASIC);
	
	m_preprocessManager.OnInitDeviceObjects(m_pd3dDevice);
	
	

	return InitDeviceObjects();
}

HRESULT CMoebius3D::OnRestoreDeviceObjects()
{
	//Reset the font
	if(FAILED(m_pd3dxFont->OnResetDevice()))
	{
		MessageBoxA( NULL, "Default font not restored properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_sprite.OnResetDevice(m_pd3dDevice)))
	{
		MessageBoxA( NULL, "Default sprite not restored properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_textureList.OnResetDevice(m_pd3dDevice)))
	{
		MessageBoxA( NULL, "Default texture list not restored properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_geometryList.OnResetDevice(m_pd3dDevice)))
	{
		MessageBoxA( NULL, "Default geometry list not restored properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_effectList.OnResetDevice(m_pd3dDevice)))
	{
		MessageBoxA( NULL, "Default effct list not restored properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_offScreenManager.OnResetDevice(m_pd3dDevice)))
	{
		MessageBoxA( NULL, "Default offscreenmanager not restored properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	m_preprocessManager.OnResetDevice(m_pd3dDevice);

	return RestoreDeviceObjects();
}

HRESULT CMoebius3D::OnLostDeviceObjects()
{
	if(FAILED(m_pd3dxFont->OnLostDevice()))
	{
		MessageBoxA( NULL, "Default font not released properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_sprite.OnLostDevice()))
	{
		MessageBoxA( NULL, "Default sprite not released properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_textureList.OnLostDevice()))
	{
		MessageBoxA( NULL, "Default texture list not released properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_geometryList.OnLostDevice()))
	{
		MessageBoxA( NULL, "Default geometry list not released properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}


	if(FAILED(m_effectList.OnLostDevice()))
	{
		MessageBoxA( NULL, "Default effect list not released properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	if(FAILED(m_offScreenManager.OnLostDevice()))
	{
		MessageBoxA( NULL, "Default offscreenrendermanger not released properly", 
			"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return E_FAIL;
	}

	m_preprocessManager.OnLostDevice();

	return LostDeviceObjects();
}


void CMoebius3D::OnDisposeDevice()
{
	//Release the Font
	SAFE_RELEASE(m_pd3dxFont);

	//Release the default sprite
	m_sprite.OnDisposeDevice();

	//Release the texture list
	m_textureList.OnDisposeDevice();

	//Release the sprite list
	m_geometryList.OnDisposeDevice();

	m_effectList.OnDisposeDevice();


	m_preprocessManager.OnDisposeDevice();
	m_offScreenManager.OnDisposeDevice();
 
	SAFE_RELEASE(m_pd3dDevice)
    SAFE_RELEASE(m_pD3D)

	DisposeDeviceObjects();
}

void CMoebius3D::ResetTimer()
{
	m_Timer.Reset();
}

void CMoebius3D::StopTimer()
{
	m_Timer.Stop();
}

void CMoebius3D::StartTimer()
{
	m_Timer.Start();
}

void CMoebius3D::FrameLoop(bool bPauseTimer, bool bPauseRender)
{
	HRESULT hr;
	
	//if( GetDXUTState().GetDeviceLost() || DXUTIsRenderingPaused() || !DXUTIsActive() )
	if(_State->GetLostDevice() || _State->GetPauseRender())
    {
        //Window is minimized or paused so just sleep to jump to another process
        Sleep(60);
    }
	if(m_pd3dDevice == NULL)
	{
		if(_State->GetLostDevice())
		{
			//The device is null and lost!!!!!, we try to recreate the device,...
			//Recreate the device,...
		}
	}
	else
	{
		//If the device is lost..¿?
		if(_State->GetLostDevice() && !_State->GetPauseRender())
		{
			//Wait some time
			Sleep(60); 

			//Test the cooperative level, if is OK render, otherwise hadle a lost device
			if(FAILED(hr = m_pd3dDevice->TestCooperativeLevel()))
			{
				//If the device is lost and is not pocible to restore it
				if(hr == D3DERR_DEVICELOST )
					return;
				
				//If the device is lost and is  pocible to restore it
				if(hr==D3DERR_DEVICENOTRESET)
				{
					//Destroy all the devices related objects
					OnLostDeviceObjects();

					if(!_State ->GetWindowedMode())
					{
						//Is full screen we must provide a valid screen resolution,...
						m_d3dpp.BackBufferWidth = _State ->m_deviceCaps.m_ResolutionList[_State ->GetScreenResolutionIndex()].m_dMode.Width;
						m_d3dpp.BackBufferHeight = _State ->m_deviceCaps.m_ResolutionList[_State ->GetScreenResolutionIndex()].m_dMode.Height;
					}

					//Now reset the device
					if(SUCCEEDED(hr = m_pd3dDevice->Reset(&m_d3dpp)))
					{
						//Restore all the devices related objects
						if(FAILED(OnRestoreDeviceObjects()))
						{
							 MessageBox( NULL, L"ERROR OnRestoreDevice",
								 L"ERROR", MB_OK|MB_ICONERROR);
						}
						_State->SetLostDevice(false);
						return;
					}
					else
					{
						if(hr == D3DERR_DEVICELOST)
						{
							//The device cannot be reseted at this time,...
							return;
						}
						if(hr == D3DERR_DEVICEREMOVED)
						{
							MessageBox( NULL, L"Call to Reset() failed with D3DERR_DEVICEREMOVED! ",
									L"ERROR", MB_OK | MB_ICONEXCLAMATION );
							return;
						}
						if(hr == D3DERR_DRIVERINTERNALERROR)
						{
							MessageBox( NULL, L"Call to Reset() failed with D3DERR_DRIVERINTERNALERROR! ",
									L"ERROR", MB_OK | MB_ICONEXCLAMATION );
							return;
						}
						if(hr == D3DERR_OUTOFVIDEOMEMORY)
						{
							MessageBox( NULL, L"Call to Reset() failed with D3DERR_OUTOFVIDEOMEMORY! ",
									L"ERROR", MB_OK | MB_ICONEXCLAMATION );
							return;
						}
						if(hr == D3DERR_INVALIDCALL)
						{
							MessageBox( NULL, L"Call to Reset() failed with D3DERR_INVALIDCALL! ",
									L"ERROR", MB_OK|MB_ICONERROR);
							return;
						}
						if(FAILED(hr))
						{
							MessageBox( NULL, L"Error in reset device",
									L"ERROR", MB_OK|MB_ICONERROR);
							return;
						}
					}
				}
				return;
			}
			
		}
		else
		{
			if(!_State->GetPauseTimer() && !bPauseTimer)
			{
				m_Timer.GetTimeValues(&_Time, &_AbsTime, &_ElapsedTime);
				dwFrames++;
				if(_Time - prevTime > 1)
				{
					_FPS = (float)(dwFrames / (_Time - prevTime));
					dwFrames = 0;
					prevTime = _Time;

					//Write the frames stats
					//StringCchPrintf(m_FrameStats, 255 * sizeof(WCHAR), L"(%d, %d) fps: %f, Elapsed Time: %f, MaxVShaderVersion: %f, MaxPShaderVersion: %f ", 
					StringCchPrintf(m_FrameStats, 255 * sizeof(WCHAR), L"%s (%d, %d) FPS: %f, Elapsed Time: %f", 
						_State->GetStrAdpterDescription(), 
						m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, _FPS, _ElapsedTime);
						//_State->GetDevSuport_MaxVS_Version(), _State->GetDevSuport_MaxVS_Version());

					_State->SetTime(_Time);
					_State->SetAbsTime(_AbsTime);
					_State->SetElapsedTime(_ElapsedTime);
				}
			}
			else
			{
				//Write the frames stats
				StringCchPrintf(m_FrameStats, 255 * sizeof(WCHAR), L" (%d, %d) FPS: 0, ELAPSED TIME: 0", 
						m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight);
			}
		
			if(!_State->GetPauseRender() && !bPauseRender)
			{
				//-----------------------------------------------------------------------------------------
				//Update the scene
				FrameUpdate();

				//-----------------------------------------------------------------------------------------
				//Render the scene
				Render();

				hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
				if(hr == D3DERR_DEVICELOST)
				{
					MessageBox( NULL, L"Call to Present() failed with D3DERR_INVALIDCALL! ",
									L"ERROR", MB_OK | MB_ICONEXCLAMATION );
					_State->SetLostDevice(true);
				}
			}
		}
	}
}

void CMoebius3D::Pause(bool bPauseTimer, bool bPauseRender)
{
	_State->SetPauseRender(bPauseRender);
	_State->SetPauseTimer(bPauseTimer);
	if(_State->GetPauseTimer())
	{
		StopTimer();
	}
	else
	{
		StartTimer();
	}
}
void CMoebius3D::OnResize(UINT nType, int cx, int cy)
{
	if(nType == SIZE_MINIMIZED)
    {
		//Pause the engine while is in minimize estate
		Pause(true, true);
		_State->SetMinimized(true);
		_State->SetMaximized(false);
	}
	else
	{
		RECT rcCurrentClient;
		GetClientRect(_State->GetDeviceWindow(), &rcCurrentClient);
        if( rcCurrentClient.top == 0 && rcCurrentClient.bottom == 0 )
        {
		 // Rapidly clicking the task bar to minimize and restore a window
		 // can cause a WM_SIZE message with SIZE_RESTORED when 
		 // the window has actually become minimized due to rapid change
		 // so just ignore this message
        }
		else if(nType  == SIZE_MAXIMIZED)
        {
			//If was minimized
			if(_State->GetMinimized())
			{
				//Unpause the engine because the window is not longer minimized
				Pause(false, false);
			}
            _State->SetMinimized(true);
			_State->SetMaximized(false);
            
			//Update the device
			UpdateDevice();
         }
		 else if(nType  == SIZE_RESTORED)
         {
			//DXUTCheckForDXGIFullScreenSwitch();
			if(_State->GetMaximized())
			{
				_State->SetMaximized(false);
				UpdateDevice();
            }
            else if(_State->GetMinimized())
            {
				Pause(false, false);
				_State->SetMinimized(false);
				UpdateDevice();
            }
            else if(_State->GetInSizeMove())
            {
				// If we're neither maximized nor minimized, the window size 
				// is changing by the user dragging the window edges.  In this 
                // case, we don't reset the device yet -- we wait until the 
                // user stops dragging, and a WM_EXITSIZEMOVE message comes.
             }
             else
             {
				 // This WM_SIZE come from resizing the window via an API like SetWindowPos() so 
				 // resize and reset the device now.
				 //MessageBox(NULL, L"ONSIZE", 0 ,0);
				 UpdateDevice();
			}
		}
	}
}
void CMoebius3D::OnBeginSizeMove()
{
	Pause(true, true);
	_State->SetInSizeMove(true);
}
void CMoebius3D::OnEndSizeMove()
{
	//The user has finished to resize the window, unpause the engine application
    Pause(false, false);
	UpdateDevice();
    _State->SetInSizeMove(false);
}
void CMoebius3D::OnDraw()
{
	FrameLoop(true);
}
HRESULT CMoebius3D::UpdateDevice()
{
	if(m_pd3dDevice != NULL)
	{
		//For windowed mode...
		if(_State->GetWindowedMode())
		{
			//Release the device objects
			OnLostDeviceObjects();

			//Update the presentation parameters
			if(_State->GetDeviceWindow()!=NULL)
			{
				RECT rcCurrentClient;
				if(GetClientRect(_State->GetDeviceWindow(), &rcCurrentClient))
				{
					m_d3dpp.BackBufferWidth  = rcCurrentClient.right;
					m_d3dpp.BackBufferHeight = rcCurrentClient.top;

					_State->SetScreenWidth(m_d3dpp.BackBufferWidth);
					_State->SetScreenHeight(m_d3dpp.BackBufferHeight);
				}
				else
				{
					MessageBox( NULL, L"Invalid window rect",
					 L"ERROR: CMoebius_GraphicsEngine_::UpdateDevice()", MB_OK|MB_ICONERROR);
					return E_INVALIDARG;
				}
			}
			else
			{
				MessageBox( NULL, L"Invalid window handle",
					 L"ERROR: CMoebius_GraphicsEngine_::UpdateDevice()", MB_OK|MB_ICONERROR);
				return E_INVALIDARG;
			}

			//Now reset the device
			HRESULT hr;
			if(SUCCEEDED(hr = m_pd3dDevice->Reset(&m_d3dpp)))
			{
				//Restore all the devices related objects
				if(FAILED(OnRestoreDeviceObjects()))
				{
					 MessageBox( NULL, L"ERROR OnRestoreDevice",
						 L"ERROR", MB_OK|MB_ICONERROR);
				}
				_State->SetLostDevice(false);
				return S_OK;
			}
			else
			{
				if(hr == D3DERR_DEVICELOST)
				{
					//The device cannot be reseted at this time,...
					_State->SetLostDevice(true);
					return S_OK;
				}
				if(hr == D3DERR_DEVICEREMOVED)
				{
					MessageBox( NULL, L"Call to Reset() failed with D3DERR_DEVICEREMOVED! ",
							L"ERROR", MB_OK | MB_ICONEXCLAMATION );
					
				}
				if(hr == D3DERR_DRIVERINTERNALERROR)
				{
					MessageBox( NULL, L"Call to Reset() failed with D3DERR_DRIVERINTERNALERROR! ",
							L"ERROR", MB_OK | MB_ICONEXCLAMATION );
					return hr;
				}
				if(hr == D3DERR_OUTOFVIDEOMEMORY)
				{
					MessageBox( NULL, L"Call to Reset() failed with D3DERR_OUTOFVIDEOMEMORY! ",
							L"ERROR", MB_OK | MB_ICONEXCLAMATION );
					return hr;
				}
				if(hr == D3DERR_INVALIDCALL)
				{
					MessageBox( NULL, L"Call to Reset() failed with D3DERR_INVALIDCALL! ",
							L"ERROR", MB_OK|MB_ICONERROR);
					return hr;
				}
				if(FAILED(hr))
				{
					MessageBox( NULL, L"Error in reset device",
							L"ERROR", MB_OK|MB_ICONERROR);
					return E_FAIL;
				}
				else
				{
					return S_OK;
				}
			}
		}
		else
		{
			//Release the device objects
			//OnLostDevice();

			return S_OK;
		}
	}
	else
	{
		return E_FAIL;
	}
}