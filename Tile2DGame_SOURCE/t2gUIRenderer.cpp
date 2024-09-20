#include "stdafx.h"
#include "t2gUIRenderer.h"

#include "t2gFunc.h"

using namespace t2g::enums;

t2g::UIRenderer::UIRenderer()
{
}

void t2g::UIRenderer::SyncWithOtherComponents()
{

}

void t2g::UIRenderer::Init(eImageName eName, INT xPos, INT yPos)
{
	SetUpdateLayer(eUpdateLayer::EnumEnd);
	SetRenderLayer(eRenderLayer::UI);

	BindBackToRenders(&UIRenderer::cbCheckImageLoading);
	BindBackToRenders(&UIRenderer::cbCheckTransform);
	BindBackToRenders(&UIRenderer::cbDrawUI);
}

eDelegateResult t2g::UIRenderer::cbDrawUI()
{
	AdjustRenderRect();

	Graphics g(func::GetBackDC());
	GET_SINGLETON(ImageManager).DrawImage(g,
		GetImageName(), GetRenderRect(), GetSrcPos());

	return eDelegateResult::OK;
}
