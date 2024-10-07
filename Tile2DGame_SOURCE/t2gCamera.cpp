#include "stdafx.h"
#include "t2gCamera.h"

#include "t2gMacro.h"
#include "t2gApplication.h"
#include "t2gTransform.h"
#include "t2gScene.h"
#include "t2gTileMapEditingScene.h"
#include "t2gInput.h"

using namespace t2g::rect;

t2g::Camera::Camera()
	: mRenderExcludeTags{}
	, mViewportRect{}
	, mCameraViewRect{}
	, mAnchor{ 0.5f, 0.5f }
	, mOffset{}
	, mMainDC(GET_SINGLETON(Application).GetMainDC())
	, mTargetTileDC(GET_SINGLETON(Application).GetTileDC())
	/*, mCameraDC(CreateCompatibleDC(mMainDC))
	, mGraphics(mCameraDC)*/
	, mTransform{}
	, mDistance(1.f)
	, mOutsideColor{}
{

}

void t2g::Camera::SyncWithOtherComponents()
{
	mTransform = GetOwnerObj()->GetComponent(eComponentType::Transform);
}

void t2g::Camera::BindToScene(SafePtr<Scene> scene)
{
	scene->BindComponent(this);
	scene->PushCamera(this);
}

void t2g::Camera::Init(const Rect& viewport, HDC targetTileDC)
{
	SetUpdateLayer(eUpdateLayer::EnumEnd);
	SetRenderLayer(eRenderLayer::EnumEnd);

	mViewportRect = viewport;
	mTargetTileDC = targetTileDC;

	BindBackToUpdates(&Camera::cbCheckTransform);
	BindBackToUpdates(&Camera::cbSyncCameraView);
	if (GetOwnerObj()->GetOwnerScene()->GetIsLoopTileMap())
	{
		BindBackToUpdates(&Camera::cbDrawLoopTileMap);
	}
	else
	{
		if (GetOwnerObj()->GetTag() == eObjectTag::TileToolCamera)
			BindBackToUpdates(&Camera::cbRenderTileOnce);
		else
			BindBackToUpdates(&Camera::cbRenderTile);

		if (mTargetTileDC == func::GetTileDC())
		{
			BindBackToUpdates(&Camera::cbDrawOutsideTileBuffer);
		}
	}
}

void t2g::Camera::Release()
{
	//DeleteDC(mCameraDC);
}

Point t2g::Camera::GetPosToCameraWorld(Point viewportPos)
{
	PointF anchor = GetAnchorByPos(mViewportRect, viewportPos);

	return GetPosByAnchor(mCameraViewRect, anchor);
}

void t2g::Camera::ClearViewport(Color color)
{
	Graphics graphics(func::GetBackDC());
	SolidBrush brush(color);
	graphics.FillRectangle(&brush, mViewportRect);
}

void t2g::Camera::DrawOutsideTileBuffer(SIZE bufferSIZE)
{
	Rect rects[4] =
	{
		{ mCameraViewRect.X, mCameraViewRect.Y,
		mCameraViewRect.Width, -mCameraViewRect.Y },

		{ mCameraViewRect.X, -1,
		-mCameraViewRect.X, bufferSIZE.cy + 1 },

		{ bufferSIZE.cx, 0,
		mCameraViewRect.GetRight() - bufferSIZE.cx, bufferSIZE.cy },

		{ mCameraViewRect.X, bufferSIZE.cy - 1,
		mCameraViewRect.Width, mCameraViewRect.GetBottom() - bufferSIZE.cy + 1 }
	};

	for (Rect& rc : rects)
	{
		if (IsReverseRect(rc) == false)
		{
			PointF anchorLT = GetAnchorByPos(mCameraViewRect, { rc.GetLeft(), rc.GetTop() });
			PointF anchorRB = GetAnchorByPos(mCameraViewRect, { rc.GetRight(), rc.GetBottom() });

			Rect resultRect = MakeRectByAnchors(mViewportRect, anchorLT, anchorRB);

			Graphics g(func::GetBackDC());
			SolidBrush b(mOutsideColor);
			g.FillRectangle(&b, resultRect);
		}
	}
}

eDelegateResult t2g::Camera::cbRenderTile()
{
	StretchBlt(GET_SINGLETON(Application).GetBackDC(),
		mViewportRect.X, mViewportRect.Y,
		mViewportRect.Width, mViewportRect.Height,
		mTargetTileDC,
		mCameraViewRect.X, mCameraViewRect.Y,
		mCameraViewRect.Width, mCameraViewRect.Height,
		SRCCOPY);

	GetOwnerObj()->EventProc(eEventCallPoint::cbRenderTile);

	return eDelegateResult::OK;
}

eDelegateResult t2g::Camera::cbRenderTileOnce()
{
	StretchBlt(GET_SINGLETON(Application).GetBackDC(),
		mViewportRect.X, mViewportRect.Y,
		mViewportRect.Width, mViewportRect.Height,
		mTargetTileDC,
		mCameraViewRect.X, mCameraViewRect.Y,
		mCameraViewRect.Width, mCameraViewRect.Height,
		SRCCOPY);

	GetOwnerObj()->EventProc(eEventCallPoint::cbRenderTile);

	return eDelegateResult::Erase;
}

eDelegateResult t2g::Camera::cbDrawOutsideTileBuffer()
{
	DrawOutsideTileBuffer(func::GetTileBufferSize());

	return eDelegateResult::OK;
}

eDelegateResult t2g::Camera::cbDrawLoopTileMap()
{
	SIZE SceneSIZE = GetOwnerObj()->GetOwnerScene()->GetSIZE();
	Size bufferSize = { SceneSIZE.cx * func::GetTileSize(), SceneSIZE.cy * func::GetTileSize() };
	DrawLoopTileMap(bufferSize);

	return eDelegateResult::OK;
}

void t2g::Camera::DrawLoopTileMap(Size bufferSize)
{
	Point coordLT = GetCoordByPosOnTileSize(bufferSize, { mCameraViewRect.GetLeft(), mCameraViewRect.GetTop() });
	Point coordRT = GetCoordByPosOnTileSize(bufferSize, { mCameraViewRect.GetRight(), mCameraViewRect.GetTop() });
	Point coordLB = GetCoordByPosOnTileSize(bufferSize, { mCameraViewRect.GetLeft(), mCameraViewRect.GetBottom() });

	INT bx = coordLT.X;
	INT ex = coordRT.X + 1;
	INT by = coordLT.Y;
	INT ey = coordLB.Y + 1;

	Rect temp;
	for (int iy = by; iy < ey; ++iy)
	{
		for (int ix = bx; ix < ex; ++ix)
		{
			Rect containedBufferTile = rect::MakeRectByCoordOnTileSize(bufferSize, { ix, iy });
			Rect::Intersect(temp, mCameraViewRect, containedBufferTile);
			Rect readRect = rect::MakeRectMovedToOriginTileRect(temp, { ix, iy }, bufferSize);
			Rect writeRect = rect::MakeInnerRectInTargetRectByInnerRectInOtherRect(
				mViewportRect, mCameraViewRect, temp);
			StretchBlt(GET_SINGLETON(Application).GetBackDC(),
				writeRect.X, writeRect.Y,
				writeRect.Width, writeRect.Height,
				mTargetTileDC,
				readRect.X, readRect.Y,
				readRect.Width, readRect.Height,
				SRCCOPY);
		}
	}
}

eDelegateResult t2g::Camera::cbSyncCameraView()
{
	Vector3 location = mTransform->GetLocation();

	mCameraViewRect.X = INT(location.x);
	mCameraViewRect.Y = INT(location.y);
	mCameraViewRect.Width = INT(FLOAT(mViewportRect.Width) * mDistance);
	mCameraViewRect.Height = INT(FLOAT(mViewportRect.Height) * mDistance);

	PositioningRectByAnchor(mCameraViewRect, mAnchor);

	GetOwnerObj()->EventProc(eEventCallPoint::cbSyncCameraView);

	return eDelegateResult::OK;
}

eDelegateResult t2g::Camera::cbBltToViewport()
{
	/*BitBlt(GET_SINGLETON(Application).GetBackDC(),
		mViewportRect.X, mViewportRect.Y,
		mViewportRect.Width, mViewportRect.Height,
		mCameraDC, 0, 0, SRCCOPY);*/
		//Graphics graphics(GET_SINGLETON(Application).GetBackDC());

	return eDelegateResult::OK;
}

//void t2g::Camera::CreateCameraBuffer()
//{
//	HBITMAP bitmap = CreateCompatibleBitmap(mMainDC, mViewportRect.Width, mViewportRect.Height);
//	HBITMAP oldBitmap = (HBITMAP)SelectObject(mCameraDC, bitmap);
//	DeleteObject(oldBitmap);
//}

