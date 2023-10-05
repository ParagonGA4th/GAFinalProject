#pragma once
#include "../ParagonData/Scene.h"

/// <summary>
/// ев╫╨ф╝©К ╬ю
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
	};

}

