#pragma once
#include <Windows.h>
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

		void Initialize(HINSTANCE hInst, HWND hWnd, RECT desktopRect);
		void Init() override { assert(false && "Application use Initialize instead of Init."); }
		void Update() override;
		void Render() override;
		void Release() override;

		void CreateBackBuffer();
	public:
		HDC GetBackDC() { return mBackHdc; }
		HWND GetHWnd() { return mHwnd; }

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

