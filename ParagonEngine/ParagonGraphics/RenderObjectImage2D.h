#pragma once
#include "RenderObject2D.h"

/// <summary>
/// ImageRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Graphics
{
	class RenderObjectImage2D : public RenderObject2D
	{
	public:
		RenderObjectImage2D(Pg::Data::BaseRenderer* baseRenderer);

		virtual void Render(Pg::Data::CameraData* camData) override;
	};
}


