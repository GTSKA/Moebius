#pragma once
#include "MathUtil.h"
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define FPSLIMIT 60
class Globals
{
public:
	Globals();
	~Globals();
	static const int screenWidth = SCREEN_WIDTH;
	static const int screenHeight = SCREEN_HEIGHT;
	static const float SPEED_CAM;
	static const float SPEED_CAM_ROT;

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
		_KEY_A = 1,
		_KEY_W = 2,
		_KEY_D = 4,
		_KEY_S = 8,
		_KEY_LEFT = 16,
		_KEY_UP = 32,
		_KEY_RIGHT = 64,
		_KEY_DOWN = 128
	};
};

