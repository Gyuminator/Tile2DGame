#pragma once
#include "t2gMath.h"

#include "t2gTransform.h"

namespace t2g::rect
{
	using namespace math;

	inline bool IsReverseRECT(const RECT& rect)
	{
		return (rect.right - rect.left) < 0 || (rect.bottom - rect.top) < 0;
	}

	inline bool IsReverseRect(const Rect& rect)
	{
		return rect.Width < 0 || rect.Height < 0;
	}

	inline RECT MakeRECTByCenter(POINT point, POINT size)
	{
		return RECT(point.x - size.x / 2, point.y - size.y / 2, point.x + size.x / 2, point.y + size.y / 2);
	}

	inline Rect MakeRectByCenter(Point point, Size size)
	{
		return Rect(point.X - size.Width / 2, point.Y - size.Height / 2, size.Width, size.Height);
	}

	inline Rect MakeRectByAnchors(const Rect& rect, PointF leftTop, PointF rightBottom)
	{
		return Rect(
			rect.X + INT((FLOAT(rect.Width) * leftTop.X)),
			rect.Y + INT((FLOAT(rect.Height) * leftTop.Y)),
			INT(FLOAT(rect.Width) * (rightBottom.X - leftTop.X)),
			INT(FLOAT(rect.Height) * (rightBottom.Y - leftTop.Y))
		);
	}

	inline Rect MakeRectByRECT(const RECT& rect)
	{
		return Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	}
	inline RECT MakeRECTByRect(const Rect& rect)
	{
		return RECT(rect.X, rect.Y, rect.GetRight(), rect.GetBottom());
	}

	inline void ScalingRect(Rect& rect, FLOAT scale)
	{
		rect.Width = INT(rect.Width * scale);
		rect.Height = INT(rect.Height * scale);
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

	inline PointF GetAnchorByPos(const Rect& rect, Point pos)
	{
		return PointF(FLOAT(pos.X - rect.X) / FLOAT(rect.Width),
			FLOAT(pos.Y - rect.Y) / FLOAT(rect.Height));
	}
	inline Point GetPosByAnchor(const Rect& rect, PointF anchor)
	{
		return Point(rect.X + INT(FLOAT(rect.Width) * anchor.X),
			rect.Y + INT(FLOAT(rect.Height) * anchor.Y));
	}
	inline Point GetCenterOfRect(const Rect& rect)
	{
		return Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
	}
	inline Rect MakeRectByTransform3DSizeAnchorOffset(SafePtr<Transform> transform, Size size, PointF anchor, Point offset)
	{
		Rect rect
		{
			INT(transform->GetLocation().x),
			INT(transform->GetLocation().y + transform->GetLocation().z),
			size.Width,
			size.Height
		};
		ScalingRectbyScale(rect, transform->GetScale());
		PositioningRectByAnchor(rect, anchor);
		return rect;
	}
	inline Rect MakeRectByTransform2DSizeAnchorOffset(SafePtr<Transform> transform, Size size, PointF anchor, Point offset)
	{
		Rect rect
		{
			INT(transform->GetLocation().x),
			INT(transform->GetLocation().y),
			size.Width,
			size.Height
		};
		ScalingRectbyScale(rect, transform->GetScale());
		PositioningRectByAnchor(rect, anchor);
		return rect;
	}

}
