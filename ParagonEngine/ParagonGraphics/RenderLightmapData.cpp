#include "RenderLightmapData.h"

namespace Pg::Graphics
{
	RenderLightmapData::RenderLightmapData(const std::string& lightmapPath, const std::string& sceneName)
		: _path(lightmapPath), _sceneName(sceneName), _isAligned(false), _belongScene(nullptr)
	{

	}

	RenderLightmapData::~RenderLightmapData()
	{

	}

}
