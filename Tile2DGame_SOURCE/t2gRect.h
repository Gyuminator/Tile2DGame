#pragma once
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

namespace t2g::rect
{
	inline RECT MakeRECTByCenter(POINT point, POINT size)
	{
		return RECT(point.x - size.x / 2, point.y - size.y / 2, point.x + size.x / 2, point.y + size.y / 2);
	}

	inline void ScalingRect(Rect& rect, PointF scale)
	{
		rect.Width = INT(rect.Width * scale.X);
		rect.Height = INT(rect.Height * scale.Y);
	}

	inline void ModifyRectByAnchor(Rect& rect, PointF anchor)
	{
		rect.X = rect.X - INT(rect.Width * anchor.X);
		rect.Y = rect.Y - INT(rect.Height * anchor.Y);
	}
}
