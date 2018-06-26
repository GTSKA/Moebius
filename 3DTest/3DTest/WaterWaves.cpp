#include "WaterWaves.h"
#include "DXGI.h"
//#include <cuda_d3d11_interop.h>


WaterWaves::WaterWaves()
{
}


WaterWaves::~WaterWaves()
{
}
bool WaterWaves::Init(unsigned int ID, unsigned modelID, TextureInit* textures2D, TextureInit* cubicTextures, unsigned int shaderID,
	Vector3* position, Vector3* rotation, Vector3* scale)
{
	bool res;
	res = Object::Init(ID, modelID, textures2D, cubicTextures, shaderID, position, rotation, scale);
	if (!res)
		return res;
	/*IDXGIFactory* factory;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	IDXGIAdapter* adapter = NULL;
	for (unsigned int i = 0; !adapter; ++i)
	{
		if (FAILED(factory->EnumAdapters(i, &adapter)))
			break;
		int dev;
		if (cudaD3D11GetDevice(&dev, adapter) == cudaSuccess)
			break;
		adapter->Release();
	}
	factory->Release();*/
}

void WaterWaves::Update(float deltaTime, int pressedKey)
{

}
