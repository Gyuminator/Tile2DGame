#pragma once
#include "t2gSingletonBase.h"
#include "t2gInput.h"
#include "t2gMacro.h"
#include "t2gSafePtr.h"
#include "t2gImageManager.h"

using std::unique_ptr;

using t2g::enums::eKeys;
using t2g::enums::eKeyState;

namespace t2g::func
{

	inline bool CheckKey(eKeys key, eKeyState state)
	{
		return GET_SINGLETON(Input).CheckKey(key, state);
	}

	inline Point GetMousePos()
	{
		return GET_SINGLETON(Input).GetMousePos();
	}

	inline SafePtr<Sprite> FindImage(eImageName eName)
	{
		return GET_SINGLETON(ImageManager).FindImage(eName);
	}

	inline const wchar_t* GetAppPath()
	{
		return GET_SINGLETON(Application).GetAppPath();
	}

	inline const HWND GetHWnd()
	{
		return GET_SINGLETON(Application).GetHWnd();
	}

	inline const RECT& GetWndRect()
	{
		return GET_SINGLETON(Application).GetWindowRect();
	}

	inline HDC GetBackDC()
	{
		return GET_SINGLETON(Application).GetBackDC();
	}

	inline HDC GetMainDC()
	{
		return GET_SINGLETON(Application).GetMainDC();
	}

	inline HDC GetTileDC()
	{
		return GET_SINGLETON(Application).GetTileDC();
	}

	inline HDC GetBlackTilePieceDC()
	{
		return GET_SINGLETON(Application).GetBlackTilePieceDC();
	}

	inline INT GetTileSize()
	{
		return Application::TileSize;
	}

	inline INT GetTileIndex(INT numOfBufferTileX, Point pos)
	{
		INT x = pos.X / GetTileSize();
		INT y = pos.Y / GetTileSize();
		return (x < numOfBufferTileX) ? y * numOfBufferTileX + x : -1;
	}

	inline Rect GetTileRectByIndex(INT numOfBufferTileX, INT index)
	{
		return { index % numOfBufferTileX * GetTileSize(),
			index / numOfBufferTileX * GetTileSize(),
			GetTileSize() , GetTileSize() };
	}

	inline SIZE GetTileBufferSize()
	{
		return GET_SINGLETON(Application).GetTileBufferSize();
	}


	/*template<typename T>
	inline std::unique_ptr<T> SafeCreate()
	{
		unique_ptr<T> uptr(new T);
		return std::move(uptr);
	}*/
}