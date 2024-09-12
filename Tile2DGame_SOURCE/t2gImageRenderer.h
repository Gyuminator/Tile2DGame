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
		void SyncWithOtherComponents() override;

	public:
		void Init(eImageName eName, INT xPos, INT yPos);

	public:
		const Rect& GetRenderRect() { return mRenderRect; }
		Rect GetSrcRect() { return mSrcRect; }
		SafePtr<Transform> GetTransform() { return mTransform; }
		eImageName GetImageName() { return mImageName; }

		void SetSrcRect(const Rect& rect) { mSrcRect = rect; }
		void SetSrcPos(const Point& pos)
		{
			mSrcRect.X = pos.X * mSrcRect.Width;
			mSrcRect.Y = pos.Y * mSrcRect.Height;
		}
		void SetImageName(const eImageName eName) { mImageName = eName; }

	protected:
		void AdjustRenderRect();

	protected:
		eDelegateResult cbCheckTransform();
		eDelegateResult cbCheckImageLoading();
		eDelegateResult cbDrawImage();

	private:
		void SyncRenderSize();
		void SyncRenderPos(Vector3 location);

	private:
		Rect mRenderRect;
		Rect mSrcRect;

		PointF mAnchor;
		POINT mOffset;

		SafePtr<Transform> mTransform;
		SafePtr<Sprite> mSprite;

		eImageName mImageName;

	};
}

