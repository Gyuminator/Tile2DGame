#pragma once
#include "t2gComponent.h"
#include "t2gMath.h"
#include "t2gSafePtr.h"
#include "t2gSprite.h"

using namespace t2g::math;

namespace t2g
{
	class Transform;

	struct DataByAdjustCamera
	{
		Rect tempRc;
		Rect tempRenderRc;
		Rect resultRenderRc;
		bool isIntersect;
	};

	class ImageRenderer : public Component
	{
	public:
		ImageRenderer();
		virtual ~ImageRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::ImageRenderer; }
		void SyncWithOtherComponents() override;

	public:
		void Init(eImageName eName, INT xPos, INT yPos);

	public:
		const Rect& GetRenderRect() { return mRenderRect; }
		Rect GetSrcRect() { return mSrcRect; }
		Point GetSrcPos() { return { mSrcRect.X, mSrcRect.Y }; }
		SafePtr<Transform> GetTransform() { return mTransform; }
		eImageName GetImageName() { return mImageName; }

		void SetSrcRect(const Rect& rect) { mSrcRect = rect; }
		void SetSrcPos(const Point& pos)
		{
			mSrcRect.X = pos.X * mSrcRect.Width;
			mSrcRect.Y = pos.Y * mSrcRect.Height;
		}
		void SetAnchor(PointF anchor) { mAnchor = anchor; }
		void SetImageName(const eImageName eName) { mImageName = eName; }

	protected:
		void AdjustRenderRect();
		DataByAdjustCamera MakeDataByAdjustCamera();
		void DrawBindByObjTag(eRenderLayer defaultRenderLayer);

	public:
		eDelegateResult cbCheckTransform();
		eDelegateResult cbCheckImageLoading();
		eDelegateResult cbDrawImage();
		eDelegateResult cbDrawImageUI();

	private:
		void SyncRenderSize();
		void SyncRenderPos(Vector3 location);

	private:
		Rect mRenderRect;
		Rect mSrcRect;

		PointF mAnchor;
		Point mOffset;

		SafePtr<Transform> mTransform;
		SafePtr<Sprite> mSprite;

		eImageName mImageName;

	};
}

