#pragma once
#include "t2gImageFrame.h"
#include "t2gComponent.h"
#include "t2gMath.h"

using namespace t2g::math;

namespace t2g
{
	class Transform;

	class ImageRenderer : public Component
	{
	public:
		ImageRenderer();
		virtual ~ImageRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::ImageRenderer; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::EnumEnd; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::Middle; }
		void SyncBindings() override;

	private:
		void update() override {};
		void render() override;

	public:
		void Init(eImageName eName, INT xPos, INT yPos);

	public:
		const Rect& GetRenderRect() { return mRenderRect; }
		Point GetSrcPos() { return mSrcPos; }
		SafePtr<Transform> GetTransform() { return mTransform; }
		eImageName GetImageName() { return mImageName; }

		void SetSrcPos(const Point& pos) { mSrcPos = pos; }
		void SetImageName(const eImageName eName) { mImageName = eName; }

	protected:
		void AdjustRenderRect(SafePtr<Sprite> sprite);

	private:
		void SyncRenderSize(SafePtr<Sprite> sprite);
		void SyncRenderPos(Vector3 location);

	private:
		Rect mRenderRect;

		Point mSrcPos;
		PointF mAnchor;
		POINT mOffset;

		SafePtr<Transform> mTransform;

		eImageName mImageName;

	};
}

