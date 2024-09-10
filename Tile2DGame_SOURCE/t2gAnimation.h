#pragma once
#include <vector>
//#include "t2gEnums.h"
#include "t2gImageManager.h"

using std::vector;

using namespace t2g::enums;

namespace t2g
{
	class Animation
	{
	public:
		void AddSrcPos(Point srcPos) { mSrcPos.push_back(srcPos); }

		size_t GetLength() { return mSrcPos.size(); }

	private:
		vector<Point> mSrcPos;

	};
}


