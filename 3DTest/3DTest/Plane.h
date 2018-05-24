#pragma once
#include "Model.h"
class ID3D11Device;
class ID3D11DeviceContext;
#ifndef UINT
typedef unsigned int UINT;
#endif
class Plane :
	public Model
{
public:
	Plane();
	~Plane();
	MODEL_ERROR Init(UINT width, UINT height, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	MODEL_ERROR Init(UINT width, UINT height, ID3D11Device* dev, ID3D11DeviceContext* devcon, int Id);
	void disconnect();
};

