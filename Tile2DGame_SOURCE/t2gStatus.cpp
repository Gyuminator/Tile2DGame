#include "stdafx.h"
#include "t2gStatus.h"

t2g::Status::Status()
	: mBaseStatus{}
	, mRateStatus{}
	, mIncStatus{}
	, mDecStatus{}
{
}

void t2g::Status::SyncWithOtherComponents()
{
	for (auto& stat : mBaseStatus)
	{
		stat = InitialValue;
	}
	for (auto& stat : mRateStatus)
	{
		stat = 1.f;
	}

}

void t2g::Status::Init()
{
}

void t2g::Status::ApplyDamage(DmgStatus damages)
{
	for (const auto& pair : damages)
	{
		mDecStatus[(UINT)pair.first] += pair.second;
	}
}

void t2g::Status::checkDeath()
{
	//GetCurStatus(eStatus::HP) < 0;
}
