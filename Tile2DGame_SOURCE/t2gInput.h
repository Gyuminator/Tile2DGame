#pragma once
#include "t2gInterfaces.h"
#include "t2gSingletonBase.h"
#include "t2gEnums.h"

using t2g::enums::eKeys;
using t2g::enums::eKeyState;

namespace t2g
{
	class Input : public SingletonBase<Input>
	{
		struct KeyInfo
		{
			UINT16 VKey;
			eKeyState State;
		};

	public:
		Input();
		virtual ~Input();

	public:
		void Update() override;
		void Render() override;
	public:
		void Init();

	public:
		bool CheckKey(eKeys key, eKeyState state) { return mKeyInfos[(UINT16)key].State == state; }
		Point GetMousePos() { return { mMousePos.x, mMousePos.y }; }

		void SetWheelDelta(INT delta) { mWheelDelta = delta; }

	private:
		void virtualKeyMapping();
		void checkKeyInput();
		void applyKeyDown(eKeyState& state);
		void applyKeyUp(eKeyState& state);
		void applyWheel();

	private:
		KeyInfo mKeyInfos[(size_t)eKeys::EnumEnd];
		POINT mMousePos;
		INT mWheelDelta;
		/*std::bitset<(size_t)enums::eKeys::EnumEnd> mCurPressed;
		std::bitset<(size_t)enums::eKeys::EnumEnd> mPrevPressed;*/

	};
}

