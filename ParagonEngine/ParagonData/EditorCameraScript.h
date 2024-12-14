#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/Camera.h"

/// <summary>
/// ī�޶� ��ũ��Ʈ �׽�Ʈ
/// -> Editor�� �����̴� ī�޶� ���ѵǾ�� �Ѵ�!
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Camera;
}

namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

namespace Pg::Data
{
	class EditorCameraScript : public Pg::Data::Component
	{
	public:
		EditorCameraScript(Pg::Data::GameObject* obj);

	public:
		virtual void Internal_EngineAwake() override;
		virtual void Internal_EngineUpdate() override;

	private:
		//TempCamera�� ���콺 ���� ��� Ȱ��. -> ���ÿ� X, Y Rotation �����ϴµ� ���� �߻�!
		void RotateY(float angle);
		void Pitch(float angle);

		void RotateFix(float yaw, float pitch); // ��ģ X,Y Rotation ����.


	private:
		// ��ǲ �ý������� ī�޶��� �̵��� üũ�Ѵ�
		Pg::Util::Input::InputSystem* tInput = nullptr;
		Pg::Util::Time::TimeSystem* tTime = nullptr;
		Pg::Data::Camera* _camera = nullptr;

		//float _moveSpeed = 0.02f; //FPS�� ����, ī�޶��� ���ǵ�UP.
		float _setMoveSpeed = 8.f; //(������ ��ǻ��)FPS�� ����, ī�޶��� ���ǵ�UP.
	};
}


