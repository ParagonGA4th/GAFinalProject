#pragma once
#include "RenderCubemap.h"
#include <vector>
#include <memory>

/// <summary>
/// Paragon Renderer���� �ε�Ǿ� �ִ� ��ü���� Cubemap�� ��� �ִ� List.
/// </summary>

namespace Pg::Graphics
{
	struct RenderObjectCubemapList
	{
	public:
		std::vector<Pg::Graphics::RenderCubemap*> _list;
	};
}