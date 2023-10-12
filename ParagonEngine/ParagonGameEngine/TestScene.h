#pragma once
#include "../ParagonData/Scene.h"
// 업데이트에서 임시로 인풋을 돌리기 위해...
#include "../ParagonGameEngine/InputSystem.h"

/// <summary>
/// 테스트용 씬
/// </summary>
/// 

namespace Pg::Engine
{
	using namespace Pg::API::Input;

	class TestScene
	{
	public:
		TestScene();

	public:
		void Initialize();
		void Update();

		Pg::Data::Scene* GetCurrentScene();

	private:
		//testScene
		Pg::Data::Scene* tCurrentScene = nullptr;
		Pg::Engine::Input::InputSystem* tInput = nullptr;
	};

}

