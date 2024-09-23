#include "stdafx.h"
#include "t2gCollider.h"

#include "t2gScene.h"
#include "t2gMacro.h"
#include "t2gTileRenderer.h"

t2g::Collider::Collider()
	: mTransform(nullptr)
	, mRect{}
	, mSize{}
	, mAnchor{}
	, mOffset{}
{
}

void t2g::Collider::SyncWithOtherComponents()
{

}

void t2g::Collider::Init(const Size size, const PointF anchor, const Point offset)
{
	ClearMDDs();
	SetUpdateLayer(eUpdateLayer::Collision);

	mSize = size;
	mAnchor = anchor;
	mOffset = offset;

	BindBackToUpdates(&Collider::cbCheckTransform);
	BindBackToUpdates(&Collider::cbAdjustRect);
	BindBackToUpdates(&Collider::cbCheckTileCollision);
}

eDelegateResult t2g::Collider::cbAdjustRect()
{
	mRect = rect::MakeRectByTransform2DSizeAnchorOffset(mTransform,
		mSize, mAnchor, mOffset);

	return eDelegateResult::OK;
}

eDelegateResult t2g::Collider::cbCheckTileCollision()
{
	if (mTransform->GetLocation().z > 0.01f)
		return eDelegateResult::OK;

	Point points[4] =
	{
		{ mRect.X, mRect.Y },
		{ mRect.GetRight(), mRect.Y },
		{ mRect.GetRight(), mRect.GetBottom() },
		{ mRect.X, mRect.GetBottom() }
	};

	SIZE sceneSize = GetOwnerObj()->GetOwnerScene()->GetSize();
	const auto& tiles = GetOwnerObj()->GetOwnerScene()->GetTiles();

	for (Point pos : points)
	{
		INT i = func::GetTileIndexSafety({ sceneSize.cx, sceneSize.cy }, pos.X, pos.Y);
		if (i < 0)
			continue; //  여기 바깥 나가는 부분이라 continue가 아니라 따로 블로킹 해줘야함.
		if (tiles[i]->GET_COMPONENT(TileRenderer)->GetIsBlocking())
		{
			Rect tileRect = func::GetTileRectByIndex(sceneSize.cx, i);
			Rect tempRect;
			if (Rect::Intersect(tempRect, mRect, tileRect))
			{
				Point Center = rect::GetCenterOfRect(tempRect);
				Point delta = Center - pos;
				(tempRect.Width < tempRect.Height) ?
					mTransform->AddLocationX(delta.X) : mTransform->AddLocationY(delta.Y);
			}
		}
	}

	return eDelegateResult::OK;
}
