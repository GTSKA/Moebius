#pragma once
#include "Object.h"
class WaterWaves :
	public Object
{
public:
	WaterWaves();
	~WaterWaves();
	bool Init(unsigned int ID, unsigned modelID, TextureInit* textures2D, TextureInit* cubicTextures, unsigned int shaderID,
		Vector3* position, Vector3* rotation, Vector3* scale);
	void Update(float deltaTime, int pressedKey);
};

