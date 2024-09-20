#pragma once
#include "t2gImageRenderer.h"

namespace t2g
{
	class UIRenderer : public ImageRenderer
	{
	public:
		UIRenderer();
		virtual ~UIRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::UIRenderer; }
		void SyncWithOtherComponents() override;

	public:
		void Init(eImageName eName, INT xPos, INT yPos);

	public:
		eDelegateResult cbDrawUI();

	private:


	};
}

