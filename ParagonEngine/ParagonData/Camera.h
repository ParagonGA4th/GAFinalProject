#pragma once
#include "Component.h"
#include "CameraData.h"
#include "../ParagonMath/PgMath.h"
#include "data_factory.h" // Auto Register를 위한 필수요건.
#include <memory>

/// <summary>
/// 변지상의 Camera 클래스.
/// 그래픽스 엔진에게 CameraData를 반환해주어야 한다.
/// 2023.09.19
/// </summary>

namespace Pg::Data
{
	using namespace Pg::Math;

	class GameObject;
	class Camera : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Camera, GameObject*>
	{
	public:
		Camera(Pg::Data::GameObject* obj);
		virtual ~Camera() = default;

		//자동화된 Auto-Registration 작동 위해 필수.
		static Component* CreateInstance(GameObject* go) { return new Camera(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Camera"; }

		//실제로 쓰이는 Camera들의 정보를 Scene의 정보와 일치시키기 위해서.
		virtual void Internal_EngineUpdate() override;
		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		void SetScreenSize(float width, float height);

		PGFLOAT4X4 GetViewMatrix() const;
		PGFLOAT4X4 GetProjMatrix() const;
		Pg::Data::CameraData* GetCameraData();

		//유니티의 Camera.ScreenPointToRay와 동일 기능을 할 것.
		//Plane Intersection 기반 구현.
		//[Parameters] : gainput 기준 정규화 Screen 좌표 / Plane의 노말 벡터 / 원점으로부터 해당 노말벡터 기준 거리.
		Pg::Math::PGFLOAT3 ScreenPointToWorldPlanePoint(Pg::Math::PGFLOAT2 screenPointNormalized, Pg::Math::PGFLOAT3 normalVec, float distance);

	private:
		void UpdateViewMatrix();


	public:
		BEGIN_VISITABLES(Pg::Data::Camera);
		VISITABLE(float, _nearZ);
		VISITABLE(float, _farZ);
		VISITABLE(float, _fovY);
		END_VISITABLES;

	private:
		std::unique_ptr<Pg::Data::CameraData> _cameraData;

	private:
		//float _nearZ = 0.0001f;
		////float _farZ = PG_PI / static_cast<float>(4); // 가장 현실적인 시야각도를 베이스로 둔다
		//float _farZ = 1000.0f;

		//float _fovY = PG_PI * 0.4f;

		float _screenWidth;
		float _screenHeight;

		Pg::Math::PGFLOAT4X4 _viewMatrix;
		Pg::Math::PGFLOAT4X4 _projMatrix;

	};

}