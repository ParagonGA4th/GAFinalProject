#pragma once
#include "../ParagonGameEngine/EngineDLLExporter.h"

/// <summary>
/// 2023.09.08
/// </summary>

namespace Pg::Engine
{
	class EngineMain
	{
	public:
		PARAGON_ENGINE_DLL void Initialize();
		PARAGON_ENGINE_DLL void Finalize();
	};
}

