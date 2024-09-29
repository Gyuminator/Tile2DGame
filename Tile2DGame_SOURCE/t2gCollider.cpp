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
	, mAddXFlag(false)
	, mAddYFlag(false)
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
	BindBackToUpdates(&Collider::cbCheckCollisionBySceneRect);
	BindBackToUpdates(&Collider::cbCheckCollisionByTiles);
}

eDelegateResult t2g::Collider::cbAdjustRect()
{
	mRect = rect::MakeRectByTransform2DSizeAnchorOffset(mTransform,
		mSize, mAnchor, mOffset);

	return eDelegateResult::OK;
}

eDelegateResult t2g::Collider::cbCheckCollisionByTiles()
{
	if (mTransform->GetLocation().z > 0.01f)
		return eDelegateResult::OK;

	SIZE sceneSIZE = GetOwnerObj()->GetOwnerScene()->GetSIZE();
	Size sceneSize = { sceneSIZE.cx, sceneSIZE.cy };

	mAddXFlag = false;
	mAddYFlag = false;
	TileBlockingByVertex
	(
		sceneSize, { mRect.GetLeft(), mRect.GetTop() },
		[](const Rect& rect) { return Point(rect.Width, rect.Height); }
	);
	TileBlockingByVertex
	(
		sceneSize, { mRect.GetRight(), mRect.GetTop() },
		[](const Rect& rect) {	return Point(-rect.Width, rect.Height); }
	);
	TileBlockingByVertex
	(
		sceneSize, { mRect.GetRight(), mRect.GetBottom() },
		[](const Rect& rect) { return Point(-rect.Width, -rect.Height); }
	);
	TileBlockingByVertex
	(
		sceneSize, { mRect.GetLeft(), mRect.GetBottom() },
		[](const Rect& rect) { return Point(rect.Width, -rect.Height); }
	);

	return eDelegateResult::OK;
}

eDelegateResult t2g::Collider::cbCheckCollisionBySceneRect()
{
	SIZE sceneSize = GetOwnerObj()->GetOwnerScene()->GetSIZE();
	Rect sceneRect = { 0, 0,
				sceneSize.cx * func::GetTileSize(), sceneSize.cy * func::GetTileSize() };
	Rect tempRect;
	if (Rect::Intersect(tempRect, mRect, sceneRect))
	{
		if (tempRect.GetLeft() != mRect.GetLeft())
		{
			mTransform->AddLocationX(tempRect.GetLeft() - mRect.GetLeft());
		}
		else if (tempRect.GetRight() != mRect.GetRight())
		{
			mTransform->AddLocationX(tempRect.GetRight() - mRect.GetRight());
		}

		if (tempRect.GetTop() != mRect.GetTop())
		{
			mTransform->AddLocationY(tempRect.GetTop() - mRect.GetTop());
		}
		else if (tempRect.GetBottom() != mRect.GetBottom())
		{
			mTransform->AddLocationY(tempRect.GetBottom() - mRect.GetBottom());
		}
	}

	return eDelegateResult::OK;
}

void t2g::Collider::TileBlockingByVertex(Size sceneSize, Point vertex, function<Point(const Rect&)> deltaPosGetter)
{
	Rect tempRect;
	INT i = func::GetTileIndexSafety(sceneSize, vertex.X, vertex.Y);
	const auto& tiles = GetOwnerObj()->GetOwnerScene()->GetTiles();
	if (i < 0)
		return;
	if (tiles[i]->GET_COMPONENT(TileRenderer)->GetIsBlocking() == false)
		return;

	Rect tileRect = func::GetTileRectByIndex(sceneSize.Width, i);
	if (Rect::Intersect(tempRect, mRect, tileRect))
	{
		Point delta = deltaPosGetter(tempRect);
		Point deltaAbs =
		{
			((delta.X < 0) ? -delta.X : delta.X),
			((delta.Y < 0) ? -delta.Y : delta.Y)
		};
		if (deltaAbs.X < deltaAbs.Y)
		{
			if (mAddXFlag == false)
			{
				mAddXFlag = true;
				mTransform->AddLocationX(delta.X);
			}
		}
		else if (deltaAbs.X > deltaAbs.Y)
		{
			if (mAddYFlag == false)
			{
				mAddYFlag = true;
				mTransform->AddLocationY(delta.Y);
			}
		}
	}
}
