#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"

#include "../ParagonCore/Scene.h"
#include "TestScene.h"

/// <summary>
/// 추후에 Scripting이 될 WorkSpace.
/// 실질적인 기능들을 추가하는 곳이다.
/// 2023.09.15
/// </summary>

namespace Pg::Engine
{
	class WorkSpace
	{
	public:
		PARAGON_ENGINE_DLL void Initialize();

		PARAGON_ENGINE_DLL Pg::Core::Scene* GetCurrentScene();

	private:
		TestScene* _testScene;
	};
}

