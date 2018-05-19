#pragma once
#include <d3d11.h>
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
	MODEL_ERROR InitModel(char* filename, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	MODEL_ERROR InitModel(char* filename, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getVertexBuffer();
	unsigned int getIndexSize();
	unsigned int getVertexSize();
	unsigned int getId();
	float getMaxU();
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
protected:
	
	unsigned int m_numVertex;
	unsigned int m_numIndices;
	unsigned int m_id;
	float m_MaxU;
};

