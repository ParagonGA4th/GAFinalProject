#pragma once

/// <summary>
/// ParagonAPI에서 발동할 Graphics API 함수들 모아서 호출하기 위한 클래스.
/// Singleton-CPP로 ParagonAPI와 함께 공유될 예정.
/// </summary>

namespace Pg::Graphics
{
	class GraphicsMain;
}

namespace Pg::Graphics
{
	class GraphicsApiExporter
	{
	public:
		GraphicsApiExporter(GraphicsMain* graphicsMain);
		~GraphicsApiExporter();
		
		//Engine의 SceneSystem에서 미리 호출될 수 있느 함수들.
		
		//Cubemap의 순서를 저장하기 전에, 어느 리소스의 이름을 어느 인덱스에 넣을지 세팅하는 함수.
		void SetLoadCubemapIndex(const std::string& cubemapName, unsigned int index);

		//어느 인덱스의 큐브맵을 현재 렌더되는 큐브맵으로 설정할지 결정.
		void SetRenderedCubemapIndex(unsigned int index);

		//
		//다시 생각해봤는데, 이미지 

	private:
		GraphicsMain* _graphicsMain;
	};
}


