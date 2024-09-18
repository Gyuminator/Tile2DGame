#include "stdafx.h"
#include "t2gApplication.h"

#include "t2gTime.h"
#include "t2gEnums.h"
#include "t2gInput.h"
#include "t2gFunc.h"
#include "t2gObject.h"
#include "t2gSafePtr.h"
#include "t2gSceneManager.h"

using t2g::enums::eKeys;
using t2g::enums::eKeyState;
using t2g::func::CheckKey;
using t2g::SafePtr;

namespace t2g
{
	Application::Application()
		: SingletonBase<Application>()
		, mHinstance(nullptr)
		, mHwnd(nullptr)
		, mHdc(nullptr)
		, mBackHdc(nullptr)
		, mTileHdc(nullptr)
		, mBackBitmap(nullptr)
		, mTileBitmap(nullptr)
		, mWndRect{}
		, mWndSize{}
	{
	}
	void Application::Init(HINSTANCE hInst, HWND hWnd, RECT desktopRect)
	{
		mHinstance = hInst;
		mHwnd = hWnd;
		mWndRect = desktopRect;

		mHdc = GetDC(mHwnd);
		mWndSize = POINT(mWndRect.right - mWndRect.left, mWndRect.bottom - mWndRect.top);

		CreateBackBuffer();
		CreateTileBuffer();

		GET_SINGLETON(SceneManager).Init();
	}
	void Application::GameLoop()
	{
		GET_SINGLETON(Input).Update();

		Update();

		Render();
	}

	void Application::Update()
	{
		GET_SINGLETON(Time).Update();

		GET_SINGLETON(SceneManager).Update();

		if (CheckKey(eKeys::Esc, eKeyState::Down))
		{
			PostQuitMessage(0);
		}

	}

	void Application::Render()
	{
		//Rectangle(mTileHdc, mWndRect.left, mWndRect.top, mWndRect.right, mWndRect.bottom);
		/*BitBlt(mBackHdc, mWndRect.left, mWndRect.top, mWndSize.x, mWndSize.y,
			mTileHdc, mWndRect.left, mWndRect.top, SRCCOPY);*/

		GET_SINGLETON(SceneManager).Render();

		GET_SINGLETON(Time).Render();

		if (GET_SINGLETON(Input).CheckKey(eKeys::LBtn, eKeyState::Pressed))
		{
			GET_SINGLETON(Input).Render();
		}

		BitBlt(mHdc, mWndRect.left, mWndRect.top, mWndSize.x, mWndSize.y,
			mBackHdc, mWndRect.left, mWndRect.top, SRCCOPY);
	}
	void Application::Release()
	{
		DeleteDC(mTileHdc);
		DeleteDC(mBackHdc);
		ReleaseDC(mHwnd, mHdc);
	}
	void Application::CreateBackBuffer()
	{
		mBackHdc = CreateCompatibleDC(mHdc);
		mBackBitmap = CreateCompatibleBitmap(mHdc, mWndSize.x, mWndSize.y);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mBackHdc, mBackBitmap);
		DeleteObject(oldBitmap);

		HBRUSH oldBrush = (HBRUSH)SelectObject(mBackHdc, GetStockObject(HOLLOW_BRUSH));
		HPEN oldPen = (HPEN)SelectObject(mBackHdc, CreatePen(PS_SOLID, 1, RGB(255, 0, 255)));

		DeleteObject(oldBrush);
		DeleteObject(oldPen);
	}

	void Application::CreateTileBuffer()
	{
		mTileHdc = CreateCompatibleDC(mBackHdc);

		mBlackTilePieceHdc = CreateCompatibleDC(mTileHdc);
		HBITMAP bitmap = CreateCompatibleBitmap(mTileHdc, TileSize, TileSize);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mBlackTilePieceHdc, bitmap);
		DeleteObject(oldBitmap);
	}

	void Application::ChangeTileBitmapSize(SIZE sceneSize)
	{
		HBITMAP tileBitmap = CreateCompatibleBitmap
		(
			GET_SINGLETON(Application).GetBackDC(),
			sceneSize.cx * Application::TileSize, sceneSize.cy * Application::TileSize
		);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mTileHdc, tileBitmap);
		DeleteObject(oldBitmap);
	}
}