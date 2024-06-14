#pragma once
#include "RenderObject2D.h"
#include <vector>
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
	
	
		//일단은 임의로 FILL RATIO 100.0f 로 설정.
		float* _fillRatio = nullptr;

		float* _imageWidth = nullptr;
		float* _imageHeight = nullptr;

		unsigned int GetImageIndex();
		void SetImageIndex(unsigned int val);

	private:
		//RenderTexture2D* _texture2D;

		//이제 인덱스로 바꿀 수 있게 할 것이다.
		std::vector<RenderTexture2D*> _texture2DVector;
		unsigned int _imageIndex{ 0 };
	};
}


