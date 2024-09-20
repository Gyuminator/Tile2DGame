#pragma once
#include "t2gComponent.h"

namespace t2g
{
	class Transform;

	class PlayerController : public Component
	{
	public:
		PlayerController() : mTransform(nullptr) {}
		virtual ~PlayerController() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::PlayerController; }
		void SyncWithOtherComponents() override;

	public:
		void Init();

	public:
		eDelegateResult cbProcArrowKeys();
		eDelegateResult cbCheckTransform();

	private:
		SafePtr<Transform> mTransform;

	};
}

