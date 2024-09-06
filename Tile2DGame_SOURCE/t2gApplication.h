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
		void Update() override;
		void Render() override;
	public:
		void GameLoop();
		void Init(HINSTANCE hInst, HWND hWnd, RECT desktopRect);
		void Release();

		void CreateBackBuffer();
	public:
		HDC GetBackDC() { return mBackHdc; }
		HWND GetHWnd() { return mHwnd; }

		const RECT& GetWindowRect() { return mWndRect; }

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

