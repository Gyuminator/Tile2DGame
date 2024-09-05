#pragma once
#include "t2gComponent.h"

namespace t2g
{
	class Transform;

	class PlayerController : public Component
	{
	public:
		PlayerController() : mTransform(nullptr) {}
		~PlayerController() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::Controller; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::Input; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::EnumEnd; }
		void SyncBindings() override;

	private:
		void update() override;
		void render() override {};

	private:
		SafePtr<Transform> mTransform;

	};
}

