#pragma once
#include "RenderObject2D.h"
#include <string>

/// <summary>
/// TextRenderer ������Ʈ�� 1��1 �����ϴ� ����������Ʈ.
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

		//���� �������� ����.
		float* _textWidth = nullptr;
		float* _textHeight = nullptr;
		Pg::Math::PGFLOAT4* _textColor = nullptr;

	private:
		RenderFont* _renderFont = nullptr;
	};
}


