#pragma once
#include "../ParagonData/Scene.h"
//#include "CameraScriptTest.h"

/// <summary>
/// 테스트용 씬
/// </summary>
/// 

namespace Pg::Engine
{
	class TestScene
	{
	public:
		//Conflict가 너무 많아서 생성자를 헤더에다가 넣었다.
		///TestScene.cpp 안된다면 생성자를 제거해라.
		TestScene()
		{
			tCurrentScene = new Pg::Data::Scene(TESTSCENENAME);
		}

	public:
		void Initialize();
		Pg::Data::Scene* GetCurrentScene();

		// Editor와 Engine가 연결이 되는지 확인하기 위한
		void SetScenesData(Pg::Data::Scene* currentScene);

	private:
		//testScene
		Pg::Data::Scene* tCurrentScene = nullptr;

		//CameraScriptTest* tCameraScript = nullptr;

		inline const static std::string TESTSCENENAME = "SampleScene";
	};
}

