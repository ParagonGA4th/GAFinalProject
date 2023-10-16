#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"

#include "../ParagonData/Scene.h"
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
		PARAGON_ENGINE_DLL void Update();

		PARAGON_ENGINE_DLL Pg::Data::Scene* GetCurrentScene();

	private:
		TestScene* _testScene;
	};
}

