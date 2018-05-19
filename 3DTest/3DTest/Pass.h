#pragma once
static enum TEXTURE_TYPE
{
	TEXTURE_NOTUSED = 0,
	TEXTURE_COLOR = 1,
	TEXTURE_DEPTH = 2
};
struct TextureIdentifier
{
	unsigned int m_TextureId;
	TEXTURE_TYPE m_type;
};
class Pass
{
public:
	Pass();
	~Pass();
	bool Init(unsigned int Id, const char* PassName, unsigned int ShaderId);
	void Clean();
	void SetTarget(unsigned int Target);
	void SetOtherData(int OtherDataCount, float* OtherData);
	void SetTextures(int TextureCount, TextureIdentifier* Textures);
	void Draw(ID3D11DeviceContext* devcon);
protected:
	unsigned int m_Id;
	char m_PassName[25];
	unsigned int m_TargetId;
	float *m_otherData;
	int m_otherDataCount;
	unsigned int m_shaderId;
	int m_TextureCount;
	TextureIdentifier* m_Textures;
};

