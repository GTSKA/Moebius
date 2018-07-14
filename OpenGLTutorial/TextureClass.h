#pragma once
//////////////
// INCLUDES //
//////////////
#include <stdio.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "openglclass.h"

class TextureClass
{
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
public:
	TextureClass();
	~TextureClass();
	bool Initialize(OpenGLClass*, char*, unsigned int, bool);
	void Shutdown();

private:
	bool LoadTarga(OpenGLClass*, char*, unsigned int, bool);

	bool loaded;
	unsigned int m_textureID;
};

