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
	SetUpdateLayer(eUpdateLayer::EnumEnd);

	mSrcRect.X = xPos;
	mSrcRect.Y = yPos;

	mImageName = eName;

	BindBackToRenders(&ImageRenderer::cbCheckImageLoading);
	BindBackToRenders(&ImageRenderer::cbCheckTransform);
	DrawBindByObjTag(eRenderLayer::Mid);
}

void t2g::ImageRenderer::AdjustRenderRect()
{
	mRenderRect = MakeRectByTransform3DSizeAnchorOffset(mTransform,
		{ mSrcRect.Width, mSrcRect.Height }, mAnchor, mOffset);
	/*SyncRenderPos(mTransform->GetLocation());
	SyncRenderSize();

	std::rect::ScalingRectbyVectorScale(mRenderRect, mTransform->GetScale());
	std::rect::PositioningRectByAnchor(mRenderRect, mAnchor);*/
}

t2g::DataByAdjustCamera t2g::ImageRenderer::MakeDataByAdjustCamera()
{
	DataByAdjustCamera datas;
	datas.isIntersect = false;

	auto camera = GetOwnerObj()->GetOwnerScene()->GetCurCamera();

	if (camera->GetRenderExcludeTags().contains(GetOwnerObj()->GetTag()))
		return datas;

	datas.baseRenderRc = GetRenderRect();

	if (Rect::Intersect(datas.tempRc, camera->GetCameraViewRect(), datas.baseRenderRc))
	{
		datas.isIntersect = true;

		PointF anchorLT = rect::GetAnchorByPos(camera->GetCameraViewRect(),
			Point(datas.tempRc.GetLeft(), datas.tempRc.GetTop()));
		PointF anchorRB = rect::GetAnchorByPos(camera->GetCameraViewRect(),
			Point(datas.tempRc.GetRight(), datas.tempRc.GetBottom()));

		datas.resultRenderRc = MakeRectByAnchors(camera->GetViewportRect(), anchorLT, anchorRB);
	}

	return datas;
}

void t2g::ImageRenderer::DrawBindByObjTag(eRenderLayer defaultRenderLayer)
{
	switch (GetOwnerObj()->GetTag())
	{
	case eObjectTag::UI:
	{
		SetRenderLayer(eRenderLayer::UI);
		BindBackToRenders(&ImageRenderer::cbDrawImageUI);
		break;
	}
	default:
	{
		SetRenderLayer(defaultRenderLayer);
		BindBackToRenders(&ImageRenderer::cbDrawImage);
		break;
	}
	}
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
		Rect resultSrcRc =
		{
			data.tempRc.X - data.baseRenderRc.X + GetSrcRect().X,
			data.tempRc.Y - data.baseRenderRc.Y + GetSrcRect().Y,
			data.tempRc.Width,
			data.tempRc.Height
		};

		Graphics graphics(func::GetBackDC());
		GET_SINGLETON(ImageManager).DrawImage(graphics, mSprite, data.resultRenderRc, resultSrcRc);
	}

	return eDelegateResult::OK;
}

eDelegateResult t2g::ImageRenderer::cbDrawImageUI()
{
	AdjustRenderRect();


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


