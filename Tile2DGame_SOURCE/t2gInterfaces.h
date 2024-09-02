#pragma once
#include <string>

namespace t2g
{
	__interface IGameLoop
	{
		void Init();
		void Update();
		void Render();
	};

	__interface INamed
	{
		const std::wstring& GetName();
	};

}