#pragma once
class Pass;
class ID3D11DeviceContext;
class Effect
{
public:
	Effect();
	~Effect();
	void Draw(ID3D11DeviceContext* devcon);
	void Clean();
	unsigned int GetId();
	bool Init(unsigned int Id, const char* Name, int passCount);
	Pass* GetPass(int i);
protected:
	unsigned int m_Id;
	char m_Name[25];
	int m_passCount;
	Pass* m_passes;
};

