#pragma once
#include "../ParagonData/Scene.h"
//#include "CameraScriptTest.h"

/// <summary>
/// �׽�Ʈ�� ��
/// </summary>
/// 

namespace Pg::Engine
{

	class TestScene
	{
	public:
		TestScene();

	public:
		void Initialize();
		Pg::Data::Scene* GetCurrentScene();

	private:
		//testScene
		Pg::Data::Scene* tCurrentScene = nullptr;

		//CameraScriptTest* tCameraScript = nullptr;
	};

}

