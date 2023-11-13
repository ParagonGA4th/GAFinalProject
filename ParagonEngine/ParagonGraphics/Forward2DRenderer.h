#pragma once
#include <dxtk/SpriteBatch.h>

#include <memory>
#include <string>
/// <summary>
/// 이미지 / 텍스트 등, 2D 리소스 출력을 맡는 렌더러이다.
/// </summary>

namespace Pg::Data
{
	struct CameraData;
}

namespace Pg::Graphics
{
	struct RenderObject2DList;
	
	class LowDX11Storage;
}

namespace Pg::Graphics
{
	class Forward2DRenderer
	{
	public:
		void Initialize();
		void Render(RenderObject2DList* render2DList, Pg::Data::CameraData* camData);
		void Finalize();

	private:
		//Sprite Rendering을 위해.
		std::unique_ptr<DirectX::SpriteBatch> _spriteBatch;

		Pg::Graphics::LowDX11Storage* _DXStorage;
	};
}


