#pragma once
#include "t2gComponent.h"
#include "t2gRect.h"
#include "t2gSprite.h"
#include "t2gEnums.h"

using namespace t2g::math;
using namespace t2g::enums;

namespace t2g
{
	using namespace Gdiplus;
	class Transform;
	class Component;

	class Camera : public Component
	{
	public:
		//typedef std::unordered_set<eObjectType> RenderTargetTypes;

	public:
		Camera();
		virtual ~Camera() { Release(); }

	public:
		eComponentType GetComponentType() const override { return eComponentType::Camera; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::EnumEnd; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::EnumEnd; }
		void SyncWithOtherComponents() override;

		void BindToScene(SafePtr<Scene> scene) override;

	public:
		void Init(const Rect& viewport);
		void Release();

	public:

	public:
		const Rect& GetViewportRect() { return mViewportRect; }
		const Rect& GetCameraViewRect() { return mCameraViewRect; }
		SafePtr<Transform> GetTransform() { return mTransform; }
		HDC GetCameraDC() { return mCameraDC; }
		Graphics& GetGraphics() { return mGraphics; }
		FLOAT GetDistance() { return mDistance; }

		void SetDistance(FLOAT distance) { mDistance = distance; }

	protected:
		eDelegateResult cbCheckTransform();
		eDelegateResult cbSyncCameraView();
		eDelegateResult cbBltToViewport();
		eDelegateResult cbRenderTile();

	private:
		void SyncRenderSize(SafePtr<Sprite> sprite) {};
		void SyncRenderPos(Vector3 location) {};
		void CreateCameraBuffer();

	private:
		unordered_set<eObjectType> mRenderTargetTypes;

		Rect mViewportRect;    // 뷰포트
		Rect mCameraViewRect;  // 카메라 시야

		const HDC mMainDC;
		HDC mCameraDC;
		Graphics mGraphics;

		SafePtr<Transform> mTransform;

		PointF mAnchor;
		Point mOffset;

		FLOAT mDistance;
	};
}

