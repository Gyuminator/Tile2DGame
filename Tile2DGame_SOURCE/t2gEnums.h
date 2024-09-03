#pragma once
#include <Windows.h>

namespace t2g::enums
{
	enum class eKeys : UINT16
	{
		_1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		n1, n2, n3, n4, n5, n6, n7, n8, n9, n0,
		Left, Right, Up, Down,
		LBtn, RBtn,
		Ctrl, Alt, LShift, Tab, Esc, Enter,

		END
	};
	enum class eKeyState : UINT8
	{
		None,
		Down,
		Pressed,
		Up
	};

	enum class eComponentType : UINT16
	{
		Transform,
		Script,
		TextureRenderer,
		Camera,

	};
	
	enum class eLayerTag : UINT8
	{
		BackGround,
		Tile,
		Bottom,
		Mid,
		Top,
		Bullet,
		Effect,
		UI,

		END
	};
	
};