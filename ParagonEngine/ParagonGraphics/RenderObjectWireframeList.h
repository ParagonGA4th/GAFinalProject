#pragma once
#include "WireframeRenderObject.h"
#include <vector>
#include <memory>

/// <summary>
/// Paragon Renderer에서 로드되어 있는 전체적인 Wireframe을 들고 있는 List.
/// </summary>

namespace Pg::Graphics
{
	struct RenderObjectWireframeList
	{
	public:
		std::vector<std::unique_ptr<WireframeRenderObject>> _list;
	};
}