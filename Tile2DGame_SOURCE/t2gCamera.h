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
		//typedef std::unordered_set<eObjectTag> RenderTargetTypes;

	public:
		Camera();
		virtual ~Camera() { Release(); }

	public:
		eComponentType GetComponentType() const override { return eComponentType::Camera; }
		void SyncWithOtherComponents() override;

		void BindToScene(SafePtr<Scene> scene) override;

	public:
		void Init(const Rect& viewport, HDC targetTileDC);
		void Release();

	public:
		Point GetPosToCameraView(Point viewportPos);
		//Point CameraViewToViewport(Point cameraViewPos);
		void InsertExcludeTag(eObjectTag tag) { mRenderExcludeTags.insert(tag); }
		void EraseExcludeTag(eObjectTag tag) { mRenderExcludeTags.erase(tag); }
		void ClearViewport(Color color);

	public:
		const unordered_set<eObjectTag>& GetRenderExcludeTags() { return mRenderExcludeTags; }
		const Rect& GetViewportRect() { return mViewportRect; }
		const Rect& GetCameraViewRect() { return mCameraViewRect; }
		SafePtr<Transform> GetTransform() { return mTransform; }
		/*HDC GetCameraDC() { return mCameraDC; }
		Graphics& GetGraphics() { return mGraphics; }*/
		FLOAT GetDistance() { return mDistance; }

		void SetTargetTileDC(HDC hdc) { mTargetTileDC = hdc; }
		void SetAnchor(PointF anchor) { mAnchor = anchor; }
		void SetDistance(FLOAT distance) { mDistance = distance; }

	public:
		eDelegateResult cbCheckTransform();
		eDelegateResult cbSyncCameraView();
		eDelegateResult cbBltToViewport();
		eDelegateResult cbRenderTile();
		eDelegateResult cbRenderTileOnce();
		eDelegateResult cbDrawOutsideTileBuffer();

	private:
		void SyncRenderSize(SafePtr<Sprite> sprite) {};
		void SyncRenderPos(Vector3 location) {};
		//void CreateCameraBuffer();

	private:
		unordered_set<eObjectTag> mRenderExcludeTags;

		Rect mViewportRect;    // 뷰포트
		Rect mCameraViewRect;  // 카메라 시야

		const HDC mMainDC;
		HDC mTargetTileDC;
		//Graphics mGraphics;

		SafePtr<Transform> mTransform;

		PointF mAnchor;
		Point mOffset;

		FLOAT mDistance;
	};
}

