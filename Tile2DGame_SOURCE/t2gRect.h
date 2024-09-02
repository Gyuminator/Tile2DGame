#pragma once
#include <windows.h>

namespace t2g::rect
{
	inline RECT MakeRectByCenter(POINT point, POINT size)
	{
		return RECT(point.x - size.x / 2, point.y - size.y / 2, point.x + size.x / 2, point.y + size.y / 2);
	}
}
