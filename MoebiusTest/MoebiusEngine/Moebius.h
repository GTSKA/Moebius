#pragma once
#include "D3DClass.h"


namespace MoebiusEngine 
{
	class Moebius
	{
	protected:
		Moebius();
		~Moebius();

		Moebius(const Moebius&) {}
		Moebius& operator=(const Moebius &) {}
	public:
		static Moebius* getInstance();
		static void destroyInstance();
		bool init(int screenWidth, int screenHeight, HWND hwnd);
		void shutdown();
		bool Frame();
		bool render();
	protected:

		static Moebius* m_pInstance;
		D3DClass* m_Direct3D;
		

	};
}

