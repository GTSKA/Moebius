#pragma once
#include "Model.h"
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
class ID3D11Device;
class ID3D11DeviceContext;
#endif
#ifndef UINT
typedef unsigned int UINT;
#endif
namespace MoebiusEngine
{
	class Rectangle : public Model
	{
	public:
		Rectangle();
		~Rectangle();
#ifdef OPENGL_PROJECT
		Rectangle(OpenGLClass* OpenGL);
		MODEL_ERROR Init(UINT width, UINT height);
		MODEL_ERROR Init(UINT width, UINT height, int Id);
#else
		MODEL_ERROR Init(UINT width, UINT height, ID3D11Device* dev);
		MODEL_ERROR Init(UINT width, UINT height, ID3D11Device* dev, int Id);
#endif
		void disconnect();
#ifdef OPENGL_PROJECT
	protected:
		OpenGLClass* m_OpenGL;
#endif
	
	};
}

