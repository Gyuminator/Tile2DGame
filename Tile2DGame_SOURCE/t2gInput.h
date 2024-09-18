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

	private:
		void virtualKeyMapping();
		void checkKeyInput();
		void applyKeyDown(eKeyState& state);
		void applyKeyUp(eKeyState& state);

	private:
		KeyInfo mKeyInfos[(size_t)eKeys::END];
		POINT mMousePos;
		/*std::bitset<(size_t)enums::eKeys::END> mCurPressed;
		std::bitset<(size_t)enums::eKeys::END> mPrevPressed;*/

	};
}

