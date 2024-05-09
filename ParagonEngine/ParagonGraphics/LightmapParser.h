#pragma once
#include "RenderLightmapData.h"
#include <string>
#include <memory>

/// <summary>
/// .pglightmap을 파싱해서 처리.
/// </summary>

namespace Pg::Graphics
{
	class LightmapParser
	{
	public:
		LightmapParser();
		~LightmapParser();

		std::unique_ptr<RenderLightmapData> LoadPgLightmap(const std::string& lightmapPath, const std::string& sceneName);

	};
}


