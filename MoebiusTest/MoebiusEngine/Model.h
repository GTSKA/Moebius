#pragma once
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
class ID3D11Buffer;
class ID3D11Device;
class ID3D11DeviceContext;
#endif
#include "MathUtils.h"
namespace MoebiusEngine
{
	static enum MODEL_ERROR
	{
		MODEL_SUCCESS = 0,
		MODEL_NAME_ERROR = 1,
		MODEL_FILE_ERROR,
		MODEL_VERTEX_ERROR,
		MODEL_INDEX_ERROR
	};
	class Model
	{
	public:
		Model();
		~Model();
#ifdef OPENGL_PROJECT
		Model(OpenGLClass* openGL);
		MODEL_ERROR InitModel(char* filename);
		MODEL_ERROR InitModel(char* filename, unsigned int Id);
		MODEL_ERROR InitTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector4 color);
		GLuint getIndexBuffer();
		GLuint getVertexBuffer();
		
	protected:
		GLuint m_vertexBuffer;
		GLuint m_indexBuffer;
		OpenGLClass* m_OpenGL;
#else
		MODEL_ERROR InitModel(char* filename, ID3D11Device* dev);
		MODEL_ERROR InitModel(char* filename, unsigned int Id, ID3D11Device* dev);
		MODEL_ERROR InitTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector4 color, ID3D11Device* dev);
		ID3D11Buffer* getIndexBuffer();
		ID3D11Buffer* getVertexBuffer();
	protected:
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
#endif
	public:
		unsigned int getIndexSize();
		unsigned int getVertexSize();
		unsigned int getId();
		float getMaxU();
		void shutdown();
		Vector3 getCenter();
		Vector3 getSize();
	protected:

		unsigned int m_numVertex;
		unsigned int m_numIndices;
		unsigned int m_id;
		float m_MaxU;
		Vector3 m_center;
		Vector3 m_size;
	};
}
