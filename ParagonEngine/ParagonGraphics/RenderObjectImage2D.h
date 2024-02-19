#pragma once
#include "RenderObject2D.h"

/// <summary>
/// ImageRenderer 컴포넌트와 1대1 대응하는 렌더오브젝트.
/// </summary>

namespace Pg::Graphics
{
	class RenderTexture2D;
}

namespace Pg::Graphics
{
	class RenderObjectImage2D : public RenderObject2D
	{
	public:
		RenderObjectImage2D(Pg::Data::BaseRenderer* baseRenderer);

		virtual void Render(DirectX::SpriteBatch* spriteBatch, Pg::Data::CameraData* camData) override;
	
	
	private:
		RenderTexture2D* _texture2D;

		//일단은 임의로 FILL RATIO 100.0f 로 설정.
		float* _fillRatio = nullptr;

		float* _imageWidth = nullptr;
		float* _imageHeight = nullptr;
	};
}


