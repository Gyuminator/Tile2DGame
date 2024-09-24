#pragma once
#include "t2gSingletonBase.h"
#include "t2gInput.h"
#include "t2gMacro.h"
#include "t2gSafePtr.h"
#include "t2gImageManager.h"
#include "t2gObject.h"
#include "t2gTransform.h"

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

	inline INT GetTileIndexSafety(Size sceneSize, UINT posX, UINT posY)
	{
		posX /= GetTileSize();
		posY /= GetTileSize();
		if (posX >= (UINT)sceneSize.Width) return -1;
		if (posY >= (UINT)sceneSize.Height) return -1;
		return posY * (UINT)sceneSize.Width + posX;
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

	inline eDelegateResult CheckTransform(SafePtr<Object> obj, SafePtr<Transform>& sptr)
	{
		sptr = obj->GET_COMPONENT(Transform);
		if (sptr.IsEmpty())
			return eDelegateResult::Return;

		return eDelegateResult::Erase;
	}

	/*Point MakePosByTileIndex(INT numOfTileX, INT tileIndex)
	{
		return { (tileIndex % numOfTileX) * GetTileSize(), (tileIndex / numOfTileX) * GetTileSize() };
	}*/

	/*unique_ptr<Object> CreateUnitObj(eUnitType type)
	{
		unique_ptr<Object> uptr = Object::CreateObject();
		uptr->SetTag(eObjectTag::Unit);

		return uptr;
	}*/
}
