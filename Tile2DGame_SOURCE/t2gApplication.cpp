#include "t2gApplication.h"
#include <string>
#include "t2gTime.h"
#include "t2gSingletonBase.h"

extern HINSTANCE hInst;
extern HWND hWnd;
extern RECT DesktopRect;

namespace t2g
{
	Application::Application()
		: SingletonBase<Application>()
		, mHinstance(hInst)
		, mHwnd(hWnd)
		, mHdc(nullptr)
		, mBackHdc(nullptr)
		, mWndRect(DesktopRect)
		, mWndSize{}
	{
		Init();
	}

	void Application::Init()
	{
		mHdc = GetDC(mHwnd);
		mWndSize = POINT(mWndRect.right - mWndRect.left, mWndRect.bottom - mWndRect.top);

		CreateBackBuffer();
		
		GET_SINGLETON(Time).Init();
	}
	void Application::GameLoop()
	{
		GET_SINGLETON(Time).Update();
		Input();

		Update();

		Render();
	}
	void Application::Input()
	{
	}
	void Application::Update()
	{
		
	}
	void Application::Render()
	{
		Rectangle(mBackHdc, mWndRect.left, mWndRect.top, mWndRect.right, mWndRect.bottom);

		GET_SINGLETON(Time).Render();
		

		BitBlt(mHdc, mWndRect.left, mWndRect.top, mWndSize.x, mWndSize.y,
			mBackHdc, mWndRect.left, mWndRect.top, SRCCOPY);
	}
	void Application::CreateBackBuffer()
	{
		mBackHdc = CreateCompatibleDC(mHdc);
		mBackBitmap = CreateCompatibleBitmap(mHdc, mWndSize.x, mWndSize.y);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(mBackHdc, mBackBitmap);
		DeleteObject(oldBitmap);
	}
}