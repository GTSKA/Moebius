#pragma once
namespace MoebiusEngine 
{
	class InputManager
	{
	protected:
		InputManager();
		~InputManager();
		InputManager(const InputManager &) {}
		InputManager& operator =(const InputManager &) {}

		
	public:
		static InputManager* getInstance();
		static void destroyInstance();
		void keyDown(unsigned int);
		void keyUp(unsigned int);

		bool isKeyDown(unsigned int);
		void init();

	protected:
		static InputManager* m_pInstance;
		bool m_keys[256];

	};
}

