#pragma once
#include "../ParagonCore/Scene.h"

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

		Pg::Core::Scene* GetCurrentScene();

	private:
		//testScene
		Pg::Core::Scene* tCurrentScene = nullptr;
	};

}

