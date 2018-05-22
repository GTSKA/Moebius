#include "Effect.h"
#include "Pass.h"
#include <tchar.h>
#include <d3d11.h>

Effect::Effect()
{
	m_passes = NULL;
}


Effect::~Effect()
{
}

void Effect::Clean()
{
	if (m_passes)
	{
		for (int i = 0; i < m_passCount; ++i)
		{
			m_passes[i].Clean();
		}
		delete[] m_passes;
		m_passes = NULL;
	}
}

unsigned int Effect::GetId()
{
	return m_Id;
}

Pass* Effect::GetPass(int i)
{
	if (i < 0)
	{
		return NULL;
	}
	if (i >= m_passCount)
	{
		return NULL;
	}
	return m_passes + i;
}

bool Effect::Init(unsigned int Id, const char* Name, int passCount)
{
	m_Id = Id;
	if (Name == NULL)
	{
		return false;
	}
	if (strlen(Name) > 25)
	{
		return false;
	}
	strcpy_s(m_Name, Name);
	m_passCount = passCount;
	m_passes = new Pass[passCount];
	return true;
}
void Effect::Draw(ID3D11DeviceContext* devcon)
{
	//To Draw:
	for (int i = 0; i < m_passCount; ++i)
	{
		m_passes[i].Draw(devcon);
	}
}
