#pragma once
#include "t2gImageManager.h"

using std::vector;

using namespace t2g::enums;

namespace t2g
{
	class Animation
	{
	public:
		void AddSrcPos(Point srcPos) { mSrcRect.push_back(srcPos); }

		size_t GetLength() { return mSrcRect.size(); }

	private:
		vector<Point> mSrcRect;

	};
}


