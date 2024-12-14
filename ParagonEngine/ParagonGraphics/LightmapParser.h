#pragma once
#include "RenderLightmapData.h"
#include <string>
#include <memory>

/// <summary>
/// .pglightmap�� �Ľ��ؼ� ó��.
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


