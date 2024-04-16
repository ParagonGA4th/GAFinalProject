#pragma once
#include "../ParagonData/Component.h"
#include "../ParagonData/Camera.h"

/// <summary>
/// 카메라 스크립트 테스트
/// -> Editor가 움직이는 카메라에 국한되어야 한다!
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
		//TempCamera의 마우스 조작 방식 활용. -> 동시에 X, Y Rotation 세팅하는데 문제 발생!
		void RotateY(float angle);
		void Pitch(float angle);

		void RotateFix(float yaw, float pitch); // 고친 X,Y Rotation 설정.


	private:
		// 인풋 시스템으로 카메라의 이동을 체크한다
		Pg::Util::Input::InputSystem* tInput = nullptr;
		Pg::Util::Time::TimeSystem* tTime = nullptr;
		Pg::Data::Camera* _camera = nullptr;

		//float _moveSpeed = 0.02f; //FPS는 동일, 카메라의 스피드UP.
		float _setMoveSpeed = 8.f; //(변지상 컴퓨터)FPS는 동일, 카메라의 스피드UP.
	};
}


