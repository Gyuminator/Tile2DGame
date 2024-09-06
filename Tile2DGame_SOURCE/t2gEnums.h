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
		ImageRenderer,
		Camera,
		Controller,

		EnumEnd
	};
	
	enum class eRenderLayer : UINT8
	{
		BackGround,
		Tile,
		Middle,
		Bullet,
		Effect,
		UI,

		EnumEnd
	};

	enum class eUpdateLayer : UINT8
	{
		Input,
		Physics,
		Transform,
		Collision,
		Script,

		EnumEnd
	};

	enum class eScene : UINT16
	{
		Loading,
		Title,
		Play,

		EnumEnd
	};

	enum class eObjectType : UINT16
	{
		Player,

		EnumEnd
	};
	
	enum class eImageName : UINT32
	{
		Player,

		EnumEnd
	};

	enum class eSceneBufferName : UINT8
	{
		Tile,

		EnumEnd
	};

};