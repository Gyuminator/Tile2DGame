#pragma once
#include "t2gImageRenderer.h"

namespace t2g
{
	class ShapeRenderer : public ImageRenderer
	{
	public:
		ShapeRenderer() :mPenColor(255, 0, 255), mShape(eShapeName::Ractangle) {}
		virtual ~ShapeRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::ShapeRenderer; }
		//void SyncWithOtherComponents() override;

	public:
		void Init(const eShapeName eShape, const Size& size, const Color penColor);

	public:
		eDelegateResult cbDrawRactangle();
		eDelegateResult cbDrawEllipse();

	private:
		Color mPenColor;
		eShapeName mShape;

	};
}

