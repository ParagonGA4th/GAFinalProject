#pragma once
#include "../ParagonData/Script.h"
#include "../ParagonData/Camera.h"

// 업데이트에서 임시로 인풋을 돌리기 위해...
#include "../ParagonGameEngine/InputSystem.h"

/// <summary>
/// 23.10.12 오수안
/// 카메라 스크립트 테스트
/// </summary>

namespace Pg::Data
{
	class GameObject;
}

class CameraScriptTest : public Pg::Data::Script
{
public:
	CameraScriptTest(Pg::Data::GameObject* obj);

public:
	void Start() override;
	void Update() override;


private:
	// 인풋 시스템으로 카메라의 이동을 체크한다
	Pg::Engine::Input::InputSystem* tInput = nullptr;

};

