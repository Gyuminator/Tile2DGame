#include "stdafx.h"
#include "t2gCamera.h"

#include "t2gMacro.h"
#include "t2gApplication.h"

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

}

void t2g::Camera::BindToScene(SafePtr<Scene> scene)
{

}

//void t2g::Camera::render()
//{
//	BitBlt(mMainDC, mViewportRect.X, mViewportRect.Y,
//		mViewportRect.Width, mViewportRect.Height,
//		mCameraDC, mViewportRect.X, mViewportRect.Y, SRCCOPY);
//}

void t2g::Camera::Init(const Rect& viewport, const Rect& cameraView)
{
	mViewportRect = viewport;
	mCameraViewRect = cameraView;
	CreateCameraBuffer();
}

void t2g::Camera::Release()
{
	DeleteDC(mCameraDC);
}

void t2g::Camera::CreateCameraBuffer()
{
	HBITMAP bitmap = CreateCompatibleBitmap(mMainDC, mViewportRect.Width, mViewportRect.Height);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(mCameraDC, bitmap);
	DeleteObject(oldBitmap);
}

