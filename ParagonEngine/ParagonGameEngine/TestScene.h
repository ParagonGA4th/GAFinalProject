#pragma once
#include "../ParagonData/Scene.h"
//#include "CameraScriptTest.h"

/// <summary>
/// 테스트용 씬
/// </summary>
/// 

namespace Pg::Core
{
	class ProcessMain;
}

namespace Pg::Engine
{
	class TestScene
	{
	public:
		TestScene();

	public:
		void Initialize();
		Pg::Data::Scene* GetCurrentScene();

		// Editor와 Engine가 연결이 되는지 확인하기 위한
		void SetScenesData(Pg::Data::Scene* currentScene);

	private:
		//testScene
		Pg::Data::Scene* tCurrentScene = nullptr;

		//CameraScriptTest* tCameraScript = nullptr;
	};
}

