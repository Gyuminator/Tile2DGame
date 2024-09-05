#pragma once
#include <string>
#include "t2gEnums.h"
#include "t2gSafePtr.h"

namespace t2g
{
	using namespace enums;

	__interface IGameLoop
	{
		void Update();
		void Render();
	};

	__interface INamed
	{
		const std::wstring& GetName() const;
	};

}