#include "stdafx.h"
#include "t2gShapeRenderer.h"

#include "t2gFunc.h"
#include "t2gRect.h"
#include "t2gObject.h"
#include "t2gScene.h"
#include "t2gCamera.h"
#include "t2gTransform.h"

using namespace t2g::enums;

void t2g::ShapeRenderer::Init(const eShapeName eShape, const Size& size, const Color penColor)
{
	mShape = eShape;
	SetSrcRect({ 0, 0, size.Width, size.Height });
	mPenColor = penColor;

	BindBackToUpdates(&ShapeRenderer::cbCheckTransform);
	switch (mShape)
	{
	case eShapeName::Ractangle:
		BindBackToRenders(&ShapeRenderer::cbDrawRactangle);
		break;

	case eShapeName::Ellipse:
		BindBackToRenders(&ShapeRenderer::cbDrawEllipse);
		break;

	}
}

eDelegateResult t2g::ShapeRenderer::cbDrawRactangle()
{
	AdjustRenderRect();

	DataByAdjustCamera data = MakeDataByAdjustCamera();

	if (data.isIntersect)
	{
		Graphics graphics(func::GetBackDC());
		Pen pen(mPenColor);
		graphics.DrawRectangle(&pen, data.resultRenderRc);
	}
	return eDelegateResult::OK;
}

eDelegateResult t2g::ShapeRenderer::cbDrawEllipse()
{
	AdjustRenderRect();

	DataByAdjustCamera data = MakeDataByAdjustCamera();

	if (data.isIntersect)
	{
		Graphics graphics(func::GetBackDC());
		Pen pen(mPenColor);
		graphics.DrawEllipse(&pen, data.resultRenderRc);
	}
	return eDelegateResult::OK;
}
