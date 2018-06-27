#include "InputManager.h"
namespace MoebiusEngine
{
	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	InputManager* InputManager::m_pInstance = 0;

	InputManager* InputManager::getInstance()
	{
		if (!m_pInstance)
			m_pInstance = new InputManager;
		return m_pInstance;
	}

	void InputManager::destroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = 0;
		}
	}

	void InputManager::keyDown(unsigned int key)
	{
		if (key > 255)
			return;
		m_keys[key] = true;
	}
	void InputManager::keyUp(unsigned int key)
	{
		if (key > 255)
			return;
		m_keys[key] = false;
	}
	bool InputManager::isKeyDown(unsigned int key)
	{
		if (key > 255)
			return false;
		return m_keys[key];
	}
	void InputManager::init()
	{
		for (int i = 0; i < 256; ++i)
			m_keys[i] = false;
	}
}