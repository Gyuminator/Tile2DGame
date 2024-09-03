#pragma once
#include "t2gSingletonBase.h"
#include "t2gInput.h"
#include "t2gMacro.h"

namespace t2g::func
{
	using t2g::enums::eKeys;
	using t2g::enums::eKeyState;

	inline bool CheckKey(eKeys key, eKeyState state)
	{
		return GET_SINGLETON(Input).CheckKey(key, state);
	}
}