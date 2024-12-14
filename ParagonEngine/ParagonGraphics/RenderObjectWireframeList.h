#pragma once
#include "WireframeRenderObject.h"
#include <vector>
#include <memory>

/// <summary>
/// Paragon Renderer���� �ε�Ǿ� �ִ� ��ü���� Wireframe�� ��� �ִ� List.
/// </summary>

namespace Pg::Graphics
{
	struct RenderObjectWireframeList
	{
	public:
		std::vector<std::unique_ptr<WireframeRenderObject>> _list;
	};
}