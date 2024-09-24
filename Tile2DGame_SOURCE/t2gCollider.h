#pragma once
#include "t2gComponent.h"

#include "t2gFunc.h"

namespace t2g
{
	class Transform;

	class Collider : public Component
	{
	public:
		Collider();
		virtual ~Collider() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::Collider; }
		void SyncWithOtherComponents() override;

	public:
		void Init(const Size size, const PointF anchor, const Point offset);

	public:
		eDelegateResult cbCheckTransform() { return func::CheckTransform(GetOwnerObj(), mTransform); }
		eDelegateResult cbAdjustRect();
		eDelegateResult cbCheckCollisionByTiles();
		eDelegateResult cbCheckCollisionBySceneRect();

	private:
		void TileBlockingByVertex(Size sceneSize, Point vertex, function<Point(const Rect&)> deltaPosGetter);

	private:
		SafePtr<Transform> mTransform;

		Rect mRect;
		Size mSize;
		PointF mAnchor;
		Point mOffset;

		bool mAddXFlag;
		bool mAddYFlag;
	};
}

