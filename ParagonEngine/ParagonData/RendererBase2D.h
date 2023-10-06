#pragma once
#include "BaseRenderer.h"

/// <summary>
/// 모든 2D로 렌더할 렌더러 컴포넌트들의 부모.
/// </summary>

namespace Pg::Data
{
	class RendererBase2D : public BaseRenderer
	{
	public:
		RendererBase2D(GameObject* obj);
		virtual ~RendererBase2D();

	};
}


