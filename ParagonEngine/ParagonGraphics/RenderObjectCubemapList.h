#pragma once
#include "RenderCubemap.h"
#include <vector>
#include <memory>

/// <summary>
/// Paragon Renderer에서 로드되어 있는 전체적인 Cubemap을 들고 있는 List.
/// </summary>

namespace Pg::Graphics
{
	struct RenderObjectCubemapList
	{
	public:
		std::vector<Pg::Graphics::RenderCubemap*> _list;
	};
}