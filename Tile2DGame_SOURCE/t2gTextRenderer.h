#pragma once
#include "t2gImageRenderer.h"

namespace t2g
{
	class TextRenderer : public ImageRenderer
	{
		public:
			TextRenderer() :mPenColor(255, 0, 255), mText{} {}
			virtual ~TextRenderer() {}

		public:
			eComponentType GetComponentType() const override { return eComponentType::TextRenderer; }
			//void SyncWithOtherComponents() override;

		public:
			void Init(const wstring& text, const Size& size, const Color penColor);

		public:
			eDelegateResult cbDrawText();

		private:
			Color mPenColor;
			wstring mText;

	};
}


