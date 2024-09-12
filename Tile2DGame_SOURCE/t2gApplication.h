#pragma once
#include "t2gSingletonBase.h"

namespace t2g
{
	class Application : public SingletonBase<Application>
	{
	public:
		constexpr static INT TileSize = 48;

	public:
		Application();
		~Application() { Release(); }

	public:
		void Update() override;
		void Render() override;
	public:
		void GameLoop();
		void Init(HINSTANCE hInst, HWND hWnd, RECT desktopRect);
		void Release();

		void ChangeTileBitmapSize(SIZE sceneSize);

	private:
		void CreateBackBuffer();
		void CreateTileBuffer();
	public:
		HWND GetHWnd() { return mHwnd; }
		HDC GetMainDC() { return mHdc; }
		HDC GetBackDC() { return mBackHdc; }
		HDC GetTileDC() { return mTileHdc; }

		const RECT& GetWindowRect() { return mWndRect; }

	private:
		HINSTANCE mHinstance;
		HWND mHwnd;
		HDC mHdc;
		HDC mBackHdc;
		HDC mTileHdc;
		HBITMAP mBackBitmap;
		HBITMAP mTileBitmap;

		RECT mWndRect;
		POINT mWndSize;
	};
}

