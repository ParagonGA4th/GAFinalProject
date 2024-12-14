#pragma once
#include "Component.h"
#include "CameraData.h"
#include "../ParagonMath/PgMath.h"
#include "data_factory.h" // Auto Register�� ���� �ʼ����.
#include <memory>

/// <summary>
/// �������� Camera Ŭ����.
/// �׷��Ƚ� �������� CameraData�� ��ȯ���־�� �Ѵ�.
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

		//�ڵ�ȭ�� Auto-Registration �۵� ���� �ʼ�.
		static Component* CreateInstance(GameObject* go) { return new Camera(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Camera"; }

		//������ ���̴� Camera���� ������ Scene�� ������ ��ġ��Ű�� ���ؼ�.
		virtual void Internal_EngineUpdate() override;
		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		void SetScreenSize(float width, float height);

		PGFLOAT4X4 GetViewMatrix() const;
		PGFLOAT4X4 GetProjMatrix() const;
		Pg::Data::CameraData* GetCameraData();

		//����Ƽ�� Camera.ScreenPointToRay�� ���� ����� �� ��.
		//Plane Intersection ��� ����.
		//[Parameters] : gainput ���� ����ȭ Screen ��ǥ / Plane�� �븻 ���� / �������κ��� �ش� �븻���� ���� �Ÿ�.
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
		////float _farZ = PG_PI / static_cast<float>(4); // ���� �������� �þ߰����� ���̽��� �д�
		//float _farZ = 1000.0f;

		//float _fovY = PG_PI * 0.4f;

		float _screenWidth;
		float _screenHeight;

		Pg::Math::PGFLOAT4X4 _viewMatrix;
		Pg::Math::PGFLOAT4X4 _projMatrix;

	};

}