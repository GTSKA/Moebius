#pragma once
#include "MathUtils.h"
#ifdef OPENGL_PROJECT	
#include "OpenGLClass.h"
#else
class ID3D11DeviceContext;
#endif
namespace MoebiusEngine
{
	class Texture;
	class Model;
	class Shader;
	class Camera;
	struct TextureInit
	{
		int m_textureCount;
		unsigned int* m_textureIDs;
	};
	class Object
	{
	public:
		Object();
		~Object();
		virtual void Update(float deltaTime);
#ifdef OPENGL_PROJECT		
		void Draw(Camera* cam, OpenGLClass* openGL);
		void InitVAO(OpenGLClass* OpenGL);
#else
		void Draw(Camera* cam, ID3D11DeviceContext* devcon);
#endif
		virtual bool Init(unsigned int ID, unsigned modelID, TextureInit* textures2D, TextureInit* cubicTextures, unsigned int shaderID,
			Vector3* position, Vector3* rotation, Vector3* scale);
		void Clean();
		void InitLights(int LightCount, unsigned int* Lights, float specularPower = 1.0f);
		void InitTilingFactor(float TilingFactor);
		Vector3 getCenter();
		Vector3 getSize();
		void setFrustum(bool frustum);
		bool shouldCalcFrustum();
	protected:
#ifdef OPENGL_PROJECT
		GLuint m_VAO;
#endif
		int m_TextureCount;
		Texture** m_Textures;
		int m_CubeTexturesCount;
		Texture** m_CubeTextures;
		Model* m_model;
		Shader* m_Shader;
		Vector3 m_position;
		Vector3 m_rotation;
		Vector3 m_scale;
		Vector3 m_center;
		Vector3 m_size;
		Matrix m_worldMatrix;
		unsigned int m_ID;
		void CalcWorldMatrix();
		int m_LightCount;
		unsigned int* m_Lights;
		float m_SpecularPower;
		float m_tilingFactor;
		float DepthAdjustment;
		bool m_frustum;

	};
}
