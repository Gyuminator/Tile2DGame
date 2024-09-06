#pragma once
#include "t2gImageManager.h"
#include "t2gComponent.h"

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
		void Init(eImageName eName);

	public:
		void SetImageName(eImageName eName) { mImageName = eName; }

		void SetFrame(INT xCount, INT yCount, INT xPos, INT yPos);

	private:
		eImageName mImageName;
		SafePtr<Transform> mTransform;

		Rect mFrameRect;
		POINT mOffset;

	};
}

