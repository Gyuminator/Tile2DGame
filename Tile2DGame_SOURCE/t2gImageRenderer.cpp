#include "stdafx.h"
#include "t2gImageRenderer.h"

#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gImageManager.h"
#include "t2gScene.h"
#include "t2gCamera.h"
#include "t2gRect.h"
#include "t2gFunc.h"

using namespace t2g::rect;

t2g::ImageRenderer::ImageRenderer()
	: mRenderRect{}
	, mSrcRect{}
	, mAnchor{}
	, mOffset{}
	, mTransform(nullptr)
	, mSprite(nullptr)
	, mImageName(eImageName::EnumEnd)
{}

void t2g::ImageRenderer::SyncWithOtherComponents()
{
	mTransform = GetOwnerObj()->GetComponent(eComponentType::Transform);
}

void t2g::ImageRenderer::Init(eImageName eName, INT xPos, INT yPos)
{
	mSrcRect.X = xPos;
	mSrcRect.Y = yPos;

	mImageName = eName;

	BindBackToRenders(&ImageRenderer::cbCheckImageLoading);
	BindBackToRenders(&ImageRenderer::cbCheckTransform);
	BindBackToRenders(&ImageRenderer::cbDrawImage);
}

void t2g::ImageRenderer::AdjustRenderRect()
{
	SyncRenderPos(mTransform->GetLocation());
	SyncRenderSize();

	std::rect::ScalingRectbyScale(mRenderRect, mTransform->GetScale());
	std::rect::PositioningRectByAnchor(mRenderRect, mAnchor);
}

t2g::DataByAdjustCamera t2g::ImageRenderer::MakeDataByAdjustCamera()
{
	DataByAdjustCamera datas;
	datas.isIntersect = false;

	auto camera = GetOwnerObj()->GetOwnerScene()->GetCurCamera();

	if (camera->GetRenderExcludeTags().contains(GetOwnerObj()->GetTag()))
		return datas;

	datas.tempRenderRc = GetRenderRect();
	rect::ScalingRect(datas.tempRenderRc, 1.f / camera->GetDistance());

	if (Rect::Intersect(datas.tempRc, camera->GetCameraViewRect(), datas.tempRenderRc))
	{
		datas.isIntersect = true;
		datas.resultRenderRc = datas.tempRc;
		PointF renderAnchor = rect::GetAnchorByPos(camera->GetCameraViewRect(), Point(datas.tempRc.X, datas.tempRc.Y));
		Point renderPos = rect::GetPosByAnchor(camera->GetViewportRect(), renderAnchor);
		datas.resultRenderRc.X = renderPos.X;
		datas.resultRenderRc.Y = renderPos.Y;
	}

	return datas;
}


eDelegateResult t2g::ImageRenderer::cbCheckTransform()
{
	mTransform = GetOwnerObj()->GetComponent<Transform>(eComponentType::Transform);
	if (mTransform.IsEmpty())
		return eDelegateResult::Return;
	else
		return eDelegateResult::Erase;
}

eDelegateResult t2g::ImageRenderer::cbCheckImageLoading()
{
	mSprite = GET_SINGLETON(ImageManager).FindImage(mImageName);
	if (mSprite.IsEmpty())
		return eDelegateResult::Return;

	mSrcRect.Width = mSprite->GetFrameWidth();
	mSrcRect.Height = mSprite->GetFrameHeight();

	return eDelegateResult::Erase;
}

eDelegateResult t2g::ImageRenderer::cbDrawImage()
{
	AdjustRenderRect();

	DataByAdjustCamera data = MakeDataByAdjustCamera();

	if (data.isIntersect)
	{
		PointF ltAnchor = rect::GetAnchorByPos(data.tempRenderRc,
			Point(data.tempRc.GetLeft(), data.tempRc.GetTop()));
		PointF rbAnchor = rect::GetAnchorByPos(data.tempRenderRc,
			Point(data.tempRc.GetRight(), data.tempRc.GetBottom()));
		Rect resultSrcRc = rect::MakeRectByAnchors(GetSrcRect(), ltAnchor, rbAnchor);

		Graphics graphics(func::GetBackDC());
		GET_SINGLETON(ImageManager).DrawImage(graphics,
			mSprite, data.resultRenderRc, resultSrcRc);
	}

	/*auto camera = GetOwnerObj()->GetOwnerObj()->GetCurCamera();
	Rect tempRc;
	Rect tempRenderRc = GetRenderRect();
	rect::ScalingRect(tempRenderRc, 1.f / camera->GetDistance());

	if (Rect::Intersect(tempRc, camera->GetCameraViewRect(), tempRenderRc))
	{
		Rect resultRenderRc = tempRc;
		PointF renderAnchor = rect::GetAnchorByPos(camera->GetCameraViewRect(), Point(tempRc.X, tempRc.Y));
		Point renderPos = rect::GetPosByAnchor(camera->GetViewportRect(), renderAnchor);
		resultRenderRc.X = renderPos.X;
		resultRenderRc.Y = renderPos.Y;

		PointF ltAnchor = rect::GetAnchorByPos(tempRenderRc,
			Point(tempRc.GetLeft(), tempRc.GetTop()));
		PointF rbAnchor = rect::GetAnchorByPos(tempRenderRc,
			Point(tempRc.GetRight(), tempRc.GetBottom()));
		Rect resultSrcRc = rect::MakeRectByAnchors(GetSrcRect(), ltAnchor, rbAnchor);

		Graphics graphics(func::GetBackDC());
		GET_SINGLETON(ImageManager).DrawImage(graphics,
			mSprite, resultRenderRc, resultSrcRc);
	}*/

	return eDelegateResult::OK;
}

void t2g::ImageRenderer::SyncRenderSize()
{
	/*mRenderRect.Width = mSprite->GetFrameWidth();
	mRenderRect.Height = mSprite->GetFrameHeight();*/
	mRenderRect.Width = mSrcRect.Width;
	mRenderRect.Height = mSrcRect.Height;
}

void t2g::ImageRenderer::SyncRenderPos(Vector3 location)
{
	mRenderRect.X = INT(location.x);
	mRenderRect.Y = INT(location.y + location.z);
}


