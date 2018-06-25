#pragma once
#ifndef _MOEBIUS3D_H_
#define	_MOEBIUS3D_H_

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#include "M3D_CommondInclude.h"

// Direct3D9 includes
#include <d3d9.h>
#include <d3dx9.h>

#include <CArray_.h>
#include <QTimer_.h>

#include "SpriteManager.h"
#include "TextureManager_.h"

#include "Geometry_.h"
#include "Effect_.h"

#include "TextureList_.h"
#include "GeometryList_.h"
#include "EffectList_.h"
#include "OffScreenRenderManager_.h"
#include "PreprocessRender_.h"

#define _MIN_WINDOW_WIDTH	640;
#define _MIN_WINDOW_HEIGHT	480;

//--------------------------------------------------------------------------------------
//General function declarations
//--------------------------------------------------------------------------------------
HRESULT IsSurfaceFormatValidOnFullScreen(D3DFORMAT sFormat);
void GetSurfaceFormatName(STRSAFE_LPWSTR strName, UINT strNameSize ,D3DFORMAT sFormat);
void GetDephtFormatName(STRSAFE_LPWSTR strName, UINT strNameSize ,D3DFORMAT sFormat);
void GetMultiSampleName(STRSAFE_LPWSTR strName, UINT strNameSize ,D3DMULTISAMPLE_TYPE sMultiSample);

const D3DFORMAT AdapterDephtFormatArray[] =
{
	D3DFMT_D16,						//16-bit z-buffer bit depth
	D3DFMT_D24S8,					//32-bit z-buffer bit depth using 24 bits for the depth channel and 8 bits for the stencil channel.
	D3DFMT_D24X8,					//32-bit z-buffer bit depth using 24 bits for the depth channel.
	D3DFMT_D24X4S4,					//32-bit z-buffer bit depth using 24 bits for the depth channel and 4 bits for the stencil channel.
	D3DFMT_D32F_LOCKABLE,			//A lockable format where the depth value is represented as a standard IEEE floating-point number.
	D3DFMT_D24FS8,					//A non-lockable format that contains 24 bits of depth (in a 24-bit floating point format - 20e4) and 8 bits of stencil.
};

const UINT AdapterSurfaceFormatSize = 2;
const D3DFORMAT AdapterSurfaceFormatArray[] =
{
	D3DFMT_A8R8G8B8,
	D3DFMT_X8R8G8B8,
   /* D3DFMT_X1R5G5B5,
    D3DFMT_R5G6B5,
    D3DFMT_A2R10G10B10,*/
};

const UINT MultisampleListSize = 16;
const D3DMULTISAMPLE_TYPE MultisampleList[] =
{
	D3DMULTISAMPLE_NONE,
    D3DMULTISAMPLE_2_SAMPLES,
	D3DMULTISAMPLE_3_SAMPLES,
	D3DMULTISAMPLE_4_SAMPLES,
	D3DMULTISAMPLE_5_SAMPLES,
	D3DMULTISAMPLE_6_SAMPLES,
	D3DMULTISAMPLE_7_SAMPLES,
	D3DMULTISAMPLE_8_SAMPLES,
	D3DMULTISAMPLE_9_SAMPLES,
	D3DMULTISAMPLE_10_SAMPLES,
	D3DMULTISAMPLE_11_SAMPLES,
	D3DMULTISAMPLE_12_SAMPLES,
	D3DMULTISAMPLE_13_SAMPLES,
	D3DMULTISAMPLE_14_SAMPLES,
	D3DMULTISAMPLE_15_SAMPLES,
	D3DMULTISAMPLE_16_SAMPLES,
	D3DMULTISAMPLE_NONMASKABLE,
};


class CMoebius_MultisapleInfo_
{
public:
	D3DMULTISAMPLE_TYPE		m_multisapleType;
	DWORD					m_maxQLevels;
	WCHAR					m_sFormatMultisapleName[255];
	WCHAR					m_sMaxQLevels[255];
};
class CMoebius_ResolutionDesc_
{
public:
	D3DDISPLAYMODE		m_dMode;
	WCHAR				m_sFormatName[255];
	WCHAR				m_sResolution[255];
	WCHAR				m_sRefreshRate[255];
};

class CMoebius_DeviceCapsItem_
{
public:
	D3DFORMAT							m_sFormat;
	CArray_<CMoebius_ResolutionDesc_>	m_ResolutionList;
	CArray_<CMoebius_MultisapleInfo_>	m_MultySampleList;
};

//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables with thread safety
//--------------------------------------------------------------------------------------
#define SET_ACCESSOR_( x, y )       inline void Set##y( x t )   { CMoebius_3DLock_ l; m_State.m_##y = t; };
#define GET_ACCESSOR_( x, y )       inline x Get##y()           { CMoebius_3DLock_ l; return m_State.m_##y; };
#define GET_SET_ACCESSOR_( x, y )   SET_ACCESSOR_( x, y ) GET_ACCESSOR_( x, y )

#define SETP_ACCESSOR_( x, y )      inline void Set##y( x* t )  { CMoebius_3DLock_ l; m_State.m_##y = *t; };
#define GETP_ACCESSOR_( x, y )      inline x* Get##y()          { CMoebius_3DLock_ l; return &m_State.m_##y; };
#define GETP_SETP_ACCESSOR_( x, y ) SETP_ACCESSOR_( x, y ) GETP_ACCESSOR_( x, y )

class CMoebius3D
{

private:

	//--------------------------------------------------------------------------------------
	// Automatically enters & leaves the CS upon object creation/deletion
	//--------------------------------------------------------------------------------------
	class CMoebius_3DLock_
	{
	public:
		inline CMoebius_3DLock_();
		inline ~CMoebius_3DLock_();
	};

	class CMoebius3D_State__
	{
	protected:
		struct STATE
		{
		public:
			
			//--------------------------------------------------------------------------------------------
			//Window for the device
			//--------------------------------------------------------------------------------------------
			HWND m_DeviceWindow;			

			//--------------------------------------------------------------------------------------------
			//Start UP
			//--------------------------------------------------------------------------------------------
			bool m_ShowFullScreenOptions;					//True if the the device has been created
			DWORD m_D3DSDKVERSION;							//Save the Microsoft DirectX version 

			//--------------------------------------------------------------------------------------------
			//Presentation parameters
			//--------------------------------------------------------------------------------------------
			bool m_DevSuport_D3DCREATE_MIXED_VERTEXPROCESSING;		//Device Suport PUREDEVICE rendering	//D3DCREATE_MIXED_VERTEXPROCESSING
			DWORD m_VertexProccessing;								//Vertex processing for the current device 
			bool m_DevSuport_D3DPTEXTURECAPS_CUBEMAP;				//Device Suport Cube Textures
			bool m_DevSuport_D3DPTEXTURECAPS_VOLUMEMAP;				//Device Suport Volumetric Textures
			float m_DevSuport_MaxVS_Version;						//Max Vertex Shader Version
			float m_DevSuport_MaxPS_Version;						//Max Pixel Shader Version
			D3DFORMAT m_SurfaceFormat;								//Surface Format for the engine
			D3DFORMAT m_DepthFormat;								//Depth Format for the engine
			bool m_SupportDepthStencilBuffer;						//m_SupportDepthStencilBuffer
			UINT m_MultiSampleIndex;								//MultiSample Type
			DWORD m_MultiSampleQuality;								//MultiSample Quality Level
			UINT m_ScreenResolutionIndex;							//MultiSample Type
			CMoebius_DeviceCapsItem_ m_devCapsInfo;					//m_SupportDepthStencilBuffer

			WCHAR*		m_StrAdpterDescription;						//Write the device description
			WCHAR*		m_StrSurfaceFormat;							//Write the surface format
			WCHAR*		m_StrDepthFormat;							//Write the depth format
			WCHAR*		m_StrLongDeviceInfo;						//Write the device description, max vertex shader & pixel shader version

			//--------------------------------------------------------------------------------------------
			//Timer values
			//--------------------------------------------------------------------------------------------
			double m_Time;									//Current application time
			double m_AbsTime;								//Current absolute time
			double m_ElapsedTime;							//Elapsed time beetwen the render pass
			double m_FPS;									//Frames per second

			//--------------------------------------------------------------------------------------------
			//Execution states
			//--------------------------------------------------------------------------------------------
			bool m_CheckedDevice;					//True if the the device has been created
			bool m_CreatedDevice;					//True if the the device has been created
			bool m_LostDevice;						//True if the the device is in "lost" state
			bool m_PauseTimer;						//True for pause the timer execution
			bool m_PauseRender;						//True for pause the render loop
			
			//--------------------------------------------------------------------------------------------
			//Window states
			//--------------------------------------------------------------------------------------------
			bool m_WindowedMode;					//True if the the device runs in windowed mode
			bool m_Maximized;						//True if the HWND is maximized
			bool m_Minimized;						//True if the HWND is minimized
			bool m_InSizeMove;						//True if the is sizing by a grap operation
			bool m_MinimizedWhileFullscreen;		//True, the HWND is minimized due to a focus switch away when fullscreen mode

			//--------------------------------------------------------------------------------------------
			//Key and mouse states values
			//--------------------------------------------------------------------------------------------
			bool m_AltKeyDown;						//True if the alt key is down
			bool m_Keys[256];						//Array list of key states
			bool m_LastKeys[256];					//Array list of last key states
			bool m_MouseButtons[5];					//Array list of mouse states

			//--------------------------------------------------------------------------------------------
			//Key and mouse states values
			//--------------------------------------------------------------------------------------------
			UINT m_ScreenWidth;						//Screen Widht
			UINT m_ScreenHeight;					//Screen Height

		};
		STATE m_State;

	public:

		CMoebius_DeviceCapsItem_ m_deviceCaps;

		CMoebius3D_State__();
		~CMoebius3D_State__();

		//Set the key states values
		//--------------------------------------------------------------------------------------------
		void SetKey(int nKey, bool value);

		//Add a new resolution information
		//--------------------------------------------------------------------------------------------
		void AddResolutionInfo(D3DDISPLAYMODE dspM);

		//Add a new multisapler information
		//--------------------------------------------------------------------------------------------
		void AddMultisampleInfo(D3DMULTISAMPLE_TYPE multiSample, UINT maxQLevels);

		//Put all the acessors
		GET_SET_ACCESSOR_(HWND, DeviceWindow);

		GET_SET_ACCESSOR_(bool, ShowFullScreenOptions);
		GET_SET_ACCESSOR_(DWORD, D3DSDKVERSION);

		GET_SET_ACCESSOR_(bool, DevSuport_D3DCREATE_MIXED_VERTEXPROCESSING);
		GET_SET_ACCESSOR_(DWORD, VertexProccessing);
		GET_SET_ACCESSOR_(bool, DevSuport_D3DPTEXTURECAPS_CUBEMAP);
		GET_SET_ACCESSOR_(bool, DevSuport_D3DPTEXTURECAPS_VOLUMEMAP);
		GET_SET_ACCESSOR_(float, DevSuport_MaxVS_Version);
		GET_SET_ACCESSOR_(float, DevSuport_MaxPS_Version);
		GET_SET_ACCESSOR_(D3DFORMAT, SurfaceFormat);
		GET_SET_ACCESSOR_(D3DFORMAT, DepthFormat);
		GET_SET_ACCESSOR_(bool, SupportDepthStencilBuffer);
		GET_SET_ACCESSOR_(UINT, MultiSampleIndex);
		GET_SET_ACCESSOR_(UINT, MultiSampleQuality);
		GET_SET_ACCESSOR_(UINT, ScreenResolutionIndex);
		
		GET_ACCESSOR_(WCHAR*, StrAdpterDescription);
		GET_ACCESSOR_(WCHAR*, StrSurfaceFormat);
		GET_ACCESSOR_(WCHAR*, StrDepthFormat);
		GET_ACCESSOR_(WCHAR*, StrLongDeviceInfo);

		GET_SET_ACCESSOR_(double, Time);
		GET_SET_ACCESSOR_(double, AbsTime);
		GET_SET_ACCESSOR_(double, ElapsedTime);
		GET_SET_ACCESSOR_(double, FPS);

		
		GET_SET_ACCESSOR_(bool, CheckedDevice);
		GET_SET_ACCESSOR_(bool, CreatedDevice);
		GET_SET_ACCESSOR_(bool, LostDevice);
		GET_SET_ACCESSOR_(bool, PauseTimer);
		GET_SET_ACCESSOR_(bool, PauseRender);

		GET_SET_ACCESSOR_(bool, WindowedMode);
		GET_SET_ACCESSOR_(bool, Maximized);
		GET_SET_ACCESSOR_(bool, Minimized);
		GET_SET_ACCESSOR_(bool, InSizeMove);

		GET_SET_ACCESSOR_(UINT, ScreenWidth);
		GET_SET_ACCESSOR_(UINT, ScreenHeight);
	};

protected:
	CMoebius3D_State__* _State;
	

public:

	//CGeometry_<_MoebiusVertex_puvc> m_geom;	
	CSpriteManager m_sprite;

	CGeometryList_<_MoebiusVertex_puvc> m_geometryList;
	CTextureList_ m_textureList;
	CEffectList_ m_effectList;

	CPreprocessRender_			m_preprocessManager;
	COffScreenRenderManager_	m_offScreenManager;


	//App instance
	//-----------------------------------------------------------------------------------------------
	HINSTANCE				m_hInstance;

	//Direct 3D Object
	//-----------------------------------------------------------------------------------------------
	LPDIRECT3D9             m_pD3D;

	//Direct 3D Device
	//-----------------------------------------------------------------------------------------------
	LPDIRECT3DDEVICE9       m_pd3dDevice;
	
	//Console font
	//-----------------------------------------------------------------------------------------------
	LPD3DXFONT	m_pd3dxFont;

	//Default line
	//-----------------------------------------------------------------------------------------------
	LPD3DXLINE	m_pLine;

	//Default Sprite
	//-----------------------------------------------------------------------------------------------
	
	
	//String for the frame stats
	//--------------------------------------------------------------------------------------------
	LPWSTR		m_FrameStats;				

	//Presentation parameters
	//-----------------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS   m_d3dpp;

	//Multysample type
	//-----------------------------------------------------------------------------------------------
	D3DMULTISAMPLE_TYPE		m_multiSampleType;

	//
	//-----------------------------------------------------------------------------------------------
	D3DCAPS9 devCaps;
	
	//Save the device adaptername
	//-----------------------------------------------------------------------------------------------
	D3DADAPTER_IDENTIFIER9 m_AdapterInfo;

	//Textures
	//-----------------------------------------------------------------------------------------------
	//CTextureDX_List		m_textures;

	//
	//-----------------------------------------------------------------------------------------------
	SIZE  m_bWindowSize;

	//
	//-----------------------------------------------------------------------------------------------
	LPD3DXBUFFER pBufferErrors;// = NULL;

	//Timer variables
	//--------------------------------------------------------------------------------------------
	double _FPS;
	double _Time;
	double _AbsTime;
	double _ElapsedTime;

	CMoebius3D(void);
	~CMoebius3D(void);

	//Check the default device capabilities
	//---------------------------------------------------------
	//HRESULT CheckDevice(HINSTANCE hInstance, HWND hWnd, bool bWindowed = false);
	HRESULT CheckDevice(HWND hWnd, bool bWindowed = false);

	//Create the device
	//---------------------------------------------------------
	HRESULT CreateDevice();

	//Check if the depth format is valid for the selected adpater and backbuffer format
	//---------------------------------------------------------
	//- 
	//---------------------------------------------------------
	HRESULT IsGetDepthFormatValid(D3DFORMAT DepthFormat, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat);

	//Get the size if the desktop resolution and the index for the valid resolution description list
	//---------------------------------------------------------
	//- 
	//---------------------------------------------------------
	void GetDesktopResolution(int *widht, int *height, UINT* validIndex);

	//Return the long device descrition, Adapter name,....
	//---------------------------------------------------------
	WCHAR* GetStrLongDeviceDescription();

	//Return the number of valid resolutions supported by the current adpapter
	//---------------------------------------------------------
	UINT GetNumberValidResolutions();

	//Return the number of valid multisapler types supported by the current adpapter
	//---------------------------------------------------------
	UINT GetNumberMultisamplerTypes();

	//Return the number of valid multisapler types supported by the current adpapter
	//---------------------------------------------------------
	UINT GetCurrentResolutionIndex();

	//Return the number of valid multisapler types supported by the current adpapter
	//---------------------------------------------------------
	UINT GetCurrentMultisamplerTypeIndex();

	//Return the number of valid multisapler types supported by the current adpapter
	//---------------------------------------------------------
	UINT GetCurrentMultisamplerQuality();

	//Return the number of valid multisapler types supported by the current adpapter
	//---------------------------------------------------------
	void SetCurrentResolutionIndex(UINT index);

	//Return the number of valid multisapler types supported by the current adpapter
	//---------------------------------------------------------
	void SetCurrentMultisamplerTypeIndex(UINT index);

	//Return the number of valid multisapler types supported by the current adpapter
	//---------------------------------------------------------
	void SetCurrentMultisamplerQuality(UINT value);

	//Return one resolution description for the resolution description list
	//---------------------------------------------------------
	CMoebius_ResolutionDesc_& GetValidResolution(UINT index);

	//Return one multisapler description for the multisapler information list
	//---------------------------------------------------------
	CMoebius_MultisapleInfo_& GetMultisample(UINT index);

	//Make the frame pass
	//--------------------------------------------------------------------------------------
	void FrameLoop(bool bPauseTimer, bool bPauseRender = false);

	//Make the frame pass
	//--------------------------------------------------------------------------------------
	void Pause(bool bPauseTimer, bool bPauseRender = false);

	//Start the quik time object
	//---------------------------------------------------------
	void StartTimer();

	//Stop the quik time object
	//---------------------------------------------------------
	void StopTimer();

	//Reset the quik time object
	//---------------------------------------------------------
	void ResetTimer();

	//Called when the device must be updated
	//---------------------------------------------------------
	HRESULT UpdateDevice();

	//Called when the window has been resized
	//---------------------------------------------------------
	void OnResize(UINT nType, int cx, int cy);

	//Called when the window just begun to resize by drag operation
	//---------------------------------------------------------
	void OnBeginSizeMove();

	//Called when the window has finished to resize by drag operation
	//---------------------------------------------------------
	void OnEndSizeMove();

	//Called when the window must be draw ofside the render loop
	//---------------------------------------------------------
	void OnDraw();

	//Virtual function for check the device capabilities
	//---------------------------------------------------------
	virtual HRESULT CheckDeviceCaps() { return S_OK; }

	//Virtual function for check the device capabilities
	//---------------------------------------------------------
	virtual HRESULT SetupDevice() { return S_OK; }
	
	//Virtual function for initialize all the device objects witch no need to restored with the device
	//---------------------------------------------------------
	virtual HRESULT InitDeviceObjects() { return S_OK; }

	//Virtual function for restore all the device linked objects
	//---------------------------------------------------------
	virtual HRESULT RestoreDeviceObjects() { return S_OK; }

	//Virtual function for restore all the device linked objects
	//---------------------------------------------------------
	virtual HRESULT LostDeviceObjects() { return S_OK; }

	//Virtual function for dsipose all the device linked objects
	//---------------------------------------------------------
	virtual void DisposeDeviceObjects() {}

	//Virtual function called just before the render pass, used to update the scene
	//---------------------------------------------------------
	virtual void FrameUpdate() {}

	//Virtual function for render the sceen
	//---------------------------------------------------------
	virtual void Render() {}

	//Delete a release the related and not related device objects
	//--------------------------------------------------------------------------------------
	void OnDisposeDevice();

protected:
	
	//Timer variables
	//--------------------------------------------------------------------------------------------
	CQTimer_ m_Timer;					//Quik timer object to manage the FPS values
	unsigned long dwFrames;				//Number of frames beetwen the render pass
	double prevTime;					//Previus time

	//Initialize the default device asocied objects
	//--------------------------------------------------------------------------------------
	HRESULT OnInitDeviceObjects();

	//Restore the default device asocied objects
	//--------------------------------------------------------------------------------------
	HRESULT OnRestoreDeviceObjects();

	//Restore the default device asocied objects
	//--------------------------------------------------------------------------------------
	HRESULT OnLostDeviceObjects();

private:

	
};

#endif
