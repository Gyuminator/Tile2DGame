#include "stdafx.h"
#include "t2gTextRenderer.h"
#include "t2gFunc.h"

void t2g::TextRenderer::Init(const wstring& text, const Size& size, const Color penColor)
{
	SetUpdateLayer(eUpdateLayer::EnumEnd);
	SetRenderLayer(eRenderLayer::Mid);

	mText = text;
	SetSrcRect({ 0, 0, size.Width, size.Height });
	mPenColor = penColor;

	BindBackToUpdates(&TextRenderer::cbCheckTransform);
	BindBackToUpdates(&TextRenderer::cbDrawText);
}

eDelegateResult t2g::TextRenderer::cbDrawText()
{
	AdjustRenderRect();

	DataByAdjustCamera data = MakeDataByAdjustCamera();

	if (data.isIntersect)
	{
		Graphics graphics(func::GetBackDC());
		SolidBrush b(mPenColor);
		graphics.DrawString(
			mText.c_str(), mText.length(), nullptr, { 0.f, 0.f }, &b);
	}

	return eDelegateResult::OK;
}
