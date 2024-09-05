#pragma once
#include <Windows.h>

namespace t2g::math
{
	struct Vector2
	{
		float x;
		float y;

		static constexpr Vector2 Zero() { return Vector2(0.f, 0.f); }
		static constexpr Vector2 One() { return Vector2(1.f, 1.f); }
		static constexpr Vector2 UnitX() { return Vector2(1.f, 0.f); }
		static constexpr Vector2 UnitY() { return Vector2(0.f, 1.f); }
	};

	struct Vector3
	{
		float x;
		float y;
		float z;

		static constexpr Vector3 Zero() { return Vector3(0.f, 0.f, 0.f); }
		static constexpr Vector3 One() { return Vector3(1.f, 1.f, 1.f); }
		static constexpr Vector3 UnitX() { return Vector3(1.f, 0.f, 0.f); }
		static constexpr Vector3 UnitY() { return Vector3(0.f, 1.f, 0.f); }
		static constexpr Vector3 UnitZ() { return Vector3(0.f, 0.f, 1.f); }
	};
}