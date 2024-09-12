#pragma once
#include "t2gComponent.h"
#include "t2gEnums.h"
#include "t2gMath.h"
#include "t2gSafePtr.h"

namespace t2g
{
	using namespace enums;
	using namespace math;

	class Transform : public Component
	{
	public:
		typedef void(Transform::* Function)();

	public:
		Transform();
		virtual ~Transform() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::Transform; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::EnumEnd; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::EnumEnd; }
		void SyncWithOtherComponents() override;

	public:
		void Init(const Vector3& location, const Vector3& rotation, const Vector3& scale);

	public:
		const Vector3& GetLocation() const { return mLocation; }
		const Vector3& GetRotation() const { return mRotation; }
		const Vector3& GetScale() const { return mScale; }

		void SetLocation(const Vector3& vector3) { mLocation = vector3; }
		void SetRotation(const Vector3& vector3) { mRotation = vector3; }
		void SetScale(const Vector3& vector3) { mScale = vector3; }

	private:
		Vector3 mLocation;
		Vector3 mRotation;
		Vector3 mScale;

		Function mUpdate;
		Function mRender;

	private:
		void showText();

	};
}

