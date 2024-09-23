#pragma once

namespace t2g::enums
{
	enum class eApplicationType : UINT8
	{
		Client,
		Engine,
	};

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
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

		MiddleEnd,

		Wheel,

		EnumEnd

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
		ShapeRenderer,
		TileRenderer,
		AnimationRenderer,
		UIRenderer,
		Camera,
		PlayerController,
		Status,
		Collider,

		EnumEnd
	};

	enum class eRenderLayer : UINT8
	{
		BackGround,
		Tile,
		Bottom,
		Mid,
		Top,
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
		Start,
		Loading,
		Title,
		Play,
		TileMapEditing,

		EnumEnd
	};

	enum class eObjectTag : UINT16
	{
		Player,
		Enemy,
		Tile,
		ToolTile,
		Camera,
		TileToolCamera,
		UI,
		Unit,
		Static,

		EnumEnd
	};

	enum class eImageName : UINT32
	{
		Player,
		Plant_00,
		Tile_Outside_A1_png,
		Tile_Outside_A2_png,
		Tile_Dungeon_A1_png,
		Tile_Dungeon_A2_png,

		EnumEnd
	};

	enum class eShapeName : UINT8
	{
		Ractangle,
		Ellipse,
		RactangleBy4Line
	};

	enum class eBufferDCType : UINT8
	{
		BackDC,
		TileDC,

		EnumEnd
	};

	enum class eAnimState : UINT16
	{
		Idle_Right,
		Idle_Down,
		Idle_Left,
		Idle_Up,
		Walk_Left,
		Walk_Right,
		Walk_Up,
		Walk_Down,

		EnumEnd
	};

	enum class eDelegateResult : UINT8
	{
		OK,
		Erase,
		Return,

		EnumEnd
	};

	enum class eEventCallPoint : UINT8
	{
		BeginUpdate,
		EndUpdate,
		cbRenderTile,
		cbSyncCameraView
	};

	enum class eChildrenLayer : UINT8
	{
		Before,
		After,

		EnumEnd
	};

	enum class eStatus : UINT8
	{
		HP,
		MP,
		Defense,
		Attack,
		MovingSpeed,

		EnumEnd
	};

	enum class eUnitType : UINT8
	{


		EnumEnd
	};

	enum class eEditMode : UINT8
	{
		Tiling,
		Blocking,

		EnumEnd
	};

};