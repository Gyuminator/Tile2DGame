#pragma once
#include "CommonHeaders.h"
#include "t2gSingletonBase.h"

namespace t2g
{
	class Application : public SingletonBase<Application>
	{
	public:
		Application();
		~Application() {}

	public:
		void GameLoop();

		void Input();
		virtual void Init();
		virtual void Update();
		virtual void Render();

		void CreateBackBuffer();
	public:
		HDC GetBackDC() { return mBackHdc; }
	private:
		HINSTANCE mHinstance;
		HWND mHwnd;
		HDC mHdc;
		HDC mBackHdc;
		HBITMAP mBackBitmap;

		RECT mWndRect;
		POINT mWndSize;
	};
}

