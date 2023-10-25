#pragma once
#include "../ParagonData/Script.h"
#include "../ParagonData/Camera.h"

/// <summary>
/// 23.10.12 오수안
/// 카메라 스크립트 테스트
/// -> Editor가 움직이는 카메라에 국한되어야 한다!
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Camera;
}

namespace Pg::Engine
{
	namespace Input
	{
		class InputSystem;
	}
}

class EditorCameraScript : public Pg::Data::Script
{
public:
	EditorCameraScript(Pg::Data::GameObject* obj);

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	//TempCamera의 마우스 조작 방식 활용. -> 동시에 X, Y Rotation 세팅하는데 문제 발생!
	void RotateY(float angle);
	void Pitch(float angle);


private:
	// 인풋 시스템으로 카메라의 이동을 체크한다
	Pg::Engine::Input::InputSystem* tInput = nullptr;
	Pg::Data::Camera* _camera = nullptr;

	float _moveSpeed = 0.1f;
};

