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

	private:
		void SyncRenderSize();
		void SyncRenderPos(Vector3 location);

	private:
		ImageFrame mImageFrame;

		SafePtr<Transform> mTransform;

		Rect mRenderRect;

		PointF mAnchor;
		POINT mOffset;

	};
}

