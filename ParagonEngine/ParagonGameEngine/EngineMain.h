#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonCore/IEngine.h"
#include "../ParagonCore/CoreSingleton.h"

/// <summary>
/// 2023.09.08
/// </summary>

namespace Pg::Engine
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::Engine
{
	class EngineMain : public Pg::Core::IEngine
	{
	public:
		PARAGON_ENGINE_DLL EngineMain();

		PARAGON_ENGINE_DLL void Initialize(float width, float height) override;
		PARAGON_ENGINE_DLL void Update() override;
		PARAGON_ENGINE_DLL void Finalize() override;

	private:

		Input::InputSystem* _inputSystem = nullptr;
	};
}

