#pragma once
class ID3D11Buffer;
class Shaders;
class ID3D11DeviceContext;
class ID3D11Device;
class Quad
{
public:
	Quad();
	~Quad();
	void Init(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getVertexBuffer();
	void Draw(Shaders* Shader, ID3D11DeviceContext* devcon);
protected:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
};

