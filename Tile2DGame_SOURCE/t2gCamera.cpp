#include "stdafx.h"
#include "t2gCamera.h"

#include "t2gMacro.h"
#include "t2gApplication.h"
#include "t2gTransform.h"
#include "t2gScene.h"

using namespace t2g::rect;

t2g::Camera::Camera()
	: mRenderTargetTypes{}
	, mViewportRect{}
	, mCameraViewRect{}
	, mAnchor{}
	, mOffset{}
	, mMainDC(GET_SINGLETON(Application).GetMainDC())
	, mCameraDC(CreateCompatibleDC(mMainDC))
	, mGraphics(mCameraDC)
	, mTransform{}
	, mDistance(1.f)
{

}

void t2g::Camera::SyncWithOtherComponents()
{
	mTransform = GetOwner()->GetComponent(eComponentType::Transform);
}

void t2g::Camera::BindToScene(SafePtr<Scene> scene)
{
	scene->BindComponent(this);
	scene->PushCamera(this);
}

//void t2g::Camera::render()
//{
//	BitBlt(mMainDC, mViewportRect.X, mViewportRect.Y,
//		mViewportRect.Width, mViewportRect.Height,
//		mCameraDC, mViewportRect.X, mViewportRect.Y, SRCCOPY);
//}

void t2g::Camera::Init(const Rect& viewport)
{
	mViewportRect = viewport;
	CreateCameraBuffer();

	BindToUpdates(&Camera::cbCheckTransform);
	BindToUpdates(&Camera::cbSyncCameraView);
	BindToUpdates(&Camera::cbRenderTile);

	BindToRenders(&Camera::cbCheckTransform);
	BindToRenders(&Camera::cbBltToViewport);
}

void t2g::Camera::Release()
{
	DeleteDC(mCameraDC);
}

eDelegateResult t2g::Camera::cbRenderTile()
{
	StretchBlt(GET_SINGLETON(Application).GetBackDC(),
		mViewportRect.X, mViewportRect.Y,
		mViewportRect.Width, mViewportRect.Height,
		GET_SINGLETON(Application).GetTileDC(),
		mCameraViewRect.X, mCameraViewRect.Y,
		mCameraViewRect.Width, mCameraViewRect.Height,
		SRCCOPY);
	/*StretchBlt(mCameraDC, 0, 0,
		mViewportRect.Width, mViewportRect.Height,
		GET_SINGLETON(Application).GetTileDC(),
		mCameraViewRect.X, mCameraViewRect.Y,
		mCameraViewRect.Width, mCameraViewRect.Height,
		SRCCOPY);*/

	return eDelegateResult::OK;
}



eDelegateResult t2g::Camera::cbCheckTransform()
{
	if (mTransform.IsEmpty())
		return eDelegateResult::Return;

	return eDelegateResult::Erase;
}

eDelegateResult t2g::Camera::cbSyncCameraView()
{
	Vector3 location = GetTransform()->GetLocation();
	mCameraViewRect = MakeRectByCenter(Point(INT(location.x), INT(location.y)),
		Size(INT(FLOAT(mViewportRect.Width) * mDistance),
			INT(FLOAT(mViewportRect.Height) * mDistance)));

	return eDelegateResult::OK;
}

eDelegateResult t2g::Camera::cbBltToViewport()
{
	/*BitBlt(GET_SINGLETON(Application).GetBackDC(),
		mViewportRect.X, mViewportRect.Y,
		mViewportRect.Width, mViewportRect.Height,
		mCameraDC, 0, 0, SRCCOPY);*/

	return eDelegateResult::OK;
}

void t2g::Camera::CreateCameraBuffer()
{
	HBITMAP bitmap = CreateCompatibleBitmap(mMainDC, mViewportRect.Width, mViewportRect.Height);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(mCameraDC, bitmap);
	DeleteObject(oldBitmap);
}

