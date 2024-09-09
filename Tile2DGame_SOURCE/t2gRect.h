#pragma once
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "t2gMath.h"


namespace t2g::rect
{
	using namespace math;

	inline RECT MakeRECTByCenter(POINT point, POINT size)
	{
		return RECT(point.x - size.x / 2, point.y - size.y / 2, point.x + size.x / 2, point.y + size.y / 2);
	}

	inline void ScalingRect(Rect& rect, PointF scale)
	{
		rect.Width = INT(rect.Width * scale.X);
		rect.Height = INT(rect.Height * scale.Y);
	}

	inline void ScalingRectbyScale(Rect& rect, Vector3 scale)
	{
		rect.Width = INT(rect.Width * scale.x);
		rect.Height = INT(rect.Height * scale.y);
	}

	inline void PositioningRectByAnchor(Rect& rect, PointF anchor)
	{
		rect.X = rect.X - INT(rect.Width * anchor.X);
		rect.Y = rect.Y - INT(rect.Height * anchor.Y);
	}
}
