#pragma once
#include "RenderObject2D.h"
#include <string>

/// <summary>
/// TextRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Graphics
{
	class RenderFont;
}

namespace Pg::Graphics
{
	class RenderObjectText2D : public RenderObject2D
	{
	public:
		RenderObjectText2D(Pg::Data::BaseRenderer* baseRenderer);

		virtual void Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData) override;

		//연동 렌더링을 위해.
		float* _textWidth = nullptr;
		float* _textHeight = nullptr;
		Pg::Math::PGFLOAT4* _textColor = nullptr;

	private:
		RenderFont* _renderFont = nullptr;
	};
}


