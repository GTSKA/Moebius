#pragma once
#include <d3d11.h>
namespace MoebiusEngine
{
	class D3DClass
	{
	public:
		D3DClass();
		~D3DClass();
		bool init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
		void shutdown();

		void beginScene(float, float, float, float);
		void endScene();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		void getVideoCardInfo(char*, int&);

	protected:
		bool m_vsync_enabled;
		int m_videoCardMemory;
		char m_videoCardDesc[128];
		IDXGISwapChain *m_swapChain;			//pointer to the swap chain interface
		ID3D11Device *m_device;					//pointer to de Direct3D device interface
		ID3D11DeviceContext *m_devcon;		//pointer to the Direct3D device context
		ID3D11RenderTargetView *m_backbuffer;	//pointer to the back buffer
		ID3D11Texture2D* m_depthStencil; //pointer to the depth stencil
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterState;
	};
}
