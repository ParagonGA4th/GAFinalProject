#pragma once

#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonCore/IEngine.h"
#include "../ParagonCore/CoreSingleton.h"

/// <summary>
/// 2023.09.08
/// </summary>

namespace Pg::Core
{
	class CoreMain;
}

namespace Pg::Engine
{
	namespace Input
	{
		class InputSystem;
	}

	namespace Manager
	{
		class EngineResourceManager;
	}
}

namespace Pg::Engine
{
	class EngineMain : public Pg::Core::IEngine
	{
	public:
		PARAGON_ENGINE_DLL EngineMain(Pg::Core::CoreMain* core);
		virtual ~EngineMain();

		PARAGON_ENGINE_DLL void Initialize(float width, float height) override;
		PARAGON_ENGINE_DLL void Update() override;
		PARAGON_ENGINE_DLL void Finalize() override;

		//AssetManager와의 교류를 위해, 엔진 리소스 매니저에 리소스를 로드하는 함수.
		PARAGON_ENGINE_DLL virtual void LoadResource(const std::string& filePath, Pg::Core::Enums::eAssetDefine define) override;

	private:
		Pg::Core::CoreMain* _coreMain = nullptr;
		Input::InputSystem* _inputSystem = nullptr;
		Pg::Engine::Manager::EngineResourceManager* _engineResourceManager = nullptr;
	};
}

