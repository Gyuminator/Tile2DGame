#pragma once
#include "t2gComponent.h"
#include "t2gRect.h"
#include "t2gSprite.h"

using namespace t2g::math;

namespace t2g
{
	class Transform;

	class Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::Camera; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::EnumEnd; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::EnumEnd; }
		void SyncBindings() override;

	private:
		void update() override {};
		void render() override;

	public:
		void Init(eImageName eName, INT xPos, INT yPos);

	public:
		const Rect& GetWriteRect() { return mWriteRect; }
		SafePtr<Transform> GetTransform() { return mTransform; }

	protected:
		void AdjustRenderRect(SafePtr<Sprite> sprite);

	private:
		void SyncRenderSize(SafePtr<Sprite> sprite);
		void SyncRenderPos(Vector3 location);

	private:
		Rect mWriteRect;
		Rect mReadRect;

		PointF mAnchor;
		POINT mOffset;

		SafePtr<Transform> mTransform;

		FLOAT mDistance;
	};
}

