#pragma once
#include "MathUtils.h"
class Globals
{
public:
	Globals();
	~Globals();
	static const int screenWidth = 1080;
	static const int screenHeight = 720;
	static const float SPEED_CAM;
	static const float SPEED_CAM_ROT;
	static const float FireAmplitude;
	static const int maxTextureCount = 8;
	static const int TilingFactor = 16;
	static const int maxLightCount = 10;
	static const int maxOtherDataCount = 10;
	static const int maxTextureIdetifierCount = 3;
	static const float WaterDepthAdjustDisplacement;
	static const Vector3 WaterColor;
	static const float FresnelPower;
	static const float WaterMaxReflection;

	static const bool FULL_SCREEN = false;
	static const bool VSYNC_ENABLED = true;
	static const float SCREEN_DEPTH;
	static const float SCREEN_NEAR;

	static enum KEY
	{
		KEY_A = 65,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_0 = 48,
		KEY_1 = 49,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_LEFT = 37,
		KEY_UP = 38,
		KEY_RIGHT = 39,
		KEY_DOWN = 40
	};
};

