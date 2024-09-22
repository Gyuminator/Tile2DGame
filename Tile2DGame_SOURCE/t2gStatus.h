#pragma once
#include "t2gComponent.h"

#include "t2gTypes.h"

namespace t2g
{
	class Transform;

	class Status : public Component
	{
	public:
		//constexpr static StatusData Precision = 10000; // 5자리
		constexpr static StatusData InitialValue = 100.f;

	public:
		Status();
		virtual ~Status() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::Status; }
		void SyncWithOtherComponents() override;

	public:
		void Init();

	public:
		StatusData GetBaseStatus(eStatus eName) { return mBaseStatus[(UINT)eName]; }
		StatusData GetRateStatus(eStatus eName) { return mRateStatus[(UINT)eName]; }
		StatusData GetIncStatus(eStatus eName) { return mIncStatus[(UINT)eName]; }
		StatusData GetDecStatus(eStatus eName) { return mDecStatus[(UINT)eName]; }
		StatusData GetMaxStatus(eStatus eName)
		{
			return mIncStatus[(UINT)eName] + (mBaseStatus[(UINT)eName] * mRateStatus[(UINT)eName]);
		}
		StatusData GetCurStatus(eStatus eName)
		{
			return GetMaxStatus(eName) - mDecStatus[(UINT)eName];
		}

	public:
		void ApplyDamage(DmgStatus damages);

	private:
		void checkDeath();

	private:
		//SafePtr<Transform> mTransform;
		StatusData mBaseStatus[UINT(eStatus::EnumEnd)];
		StatusData mRateStatus[UINT(eStatus::EnumEnd)];
		StatusData mIncStatus[UINT(eStatus::EnumEnd)];
		StatusData mDecStatus[UINT(eStatus::EnumEnd)];

	};
}

