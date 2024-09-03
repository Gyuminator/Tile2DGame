#include "t2gApplication.h"
#include <string>
#include "t2gTime.h"
#include "t2gEnums.h"
#include "t2gInput.h"
#include "t2gFunc.h"
#include "t2gObject.h"
#include "t2gSafePtr.h"

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
		, mBackBitmap(nullptr)
		, mWndRect{}
		, mWndSize{}
	{
	}
	void Application::Initialize(HINSTANCE hInst, HWND hWnd, RECT desktopRect)
	{
		mHinstance = hInst;
		mHwnd = hWnd;
		mWndRect = desktopRect;

		mHdc = GetDC(mHwnd);
		mWndSize = POINT(mWndRect.right - mWndRect.left, mWndRect.bottom - mWndRect.top);

		CreateBackBuffer();
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

		if (CheckKey(eKeys::Esc, eKeyState::Down))
		{
			PostQuitMessage(0);
		}

	}
	void Application::Render()
	{
		Rectangle(mBackHdc, mWndRect.left, mWndRect.top, mWndRect.right, mWndRect.bottom);

		GET_SINGLETON(Time).Render();
		

		BitBlt(mHdc, mWndRect.left, mWndRect.top, mWndSize.x, mWndSize.y,
			mBackHdc, mWndRect.left, mWndRect.top, SRCCOPY);
	}
	void Application::Release()
	{
		DeleteDC(mBackHdc);
		ReleaseDC(mHwnd, mHdc);
	}
	void Application::CreateBackBuffer()
	{
		mBackHdc = CreateCompatibleDC(mHdc);
		mBackBitmap = CreateCompatibleBitmap(mHdc, mWndSize.x, mWndSize.y);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mBackHdc, mBackBitmap);
		DeleteObject(oldBitmap);

		SelectObject(mBackHdc, GetStockObject(WHITE_PEN));
	}
}