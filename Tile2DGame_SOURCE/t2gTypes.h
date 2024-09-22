#pragma once
#include "t2gEnums.h"

namespace t2g
{
	using namespace enums;

	typedef unordered_map<eEventCallPoint, list<function<eDelegateResult()>>> Events;

	typedef float StatusData;

	typedef unordered_map<eStatus, StatusData> DmgStatus;
}


