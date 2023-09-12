#pragma once
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonCore/IEngine.h"
#include "../ParagonCore/Singleton.h"
#include "../ParagonUtil/Log.h"
#include "InputSystem.h"
#include "TimeSystem.h"

/// <summary>
/// 2023.09.08
/// </summary>

namespace Pg::Engine
{
	class EngineMain : public Pg::Core::IEngine
	{
	public:
		PARAGON_ENGINE_DLL EngineMain();

		PARAGON_ENGINE_DLL void Initialize();
		PARAGON_ENGINE_DLL void Update();
		PARAGON_ENGINE_DLL void Finalize();

	private:

		Input::InputSystem* _inputSystem;
		Time::TimeSystem* _timeSystem;
	};
}

