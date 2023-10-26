#pragma once

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
}

namespace Pg::Graphics
{
	class Forward2DRenderer
	{
	public:
		void Initialize();
		void Render(RenderObject2DList* render2DList, Pg::Data::CameraData* camData);
		void Finalize();
	};
}


