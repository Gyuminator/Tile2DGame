#pragma once
#include "t2gEnums.h"

using namespace t2g::enums;

namespace t2g
{
	__interface IEvent
	{
		eDelegateResult EventProc();
	};

	// 예가 가변 템플릿, std::function과 튜플들고 있는다.
	template<typename... Types>
	class Event : public IEvent
	{

	private:
		tuple<Types...> mDatas;
	};
}

