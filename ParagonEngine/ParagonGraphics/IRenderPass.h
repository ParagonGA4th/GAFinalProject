#pragma once
#include "RenderObject3DList.h"
#include "../ParagonData/CameraData.h"

/// <summary>
/// 모든 Render Pass들의 부모 클래스이다.
/// 이를 상속받아서 세부적인 패스들은 활용된다.
/// 개별적인 RenderPass당 세부 사항이 다르므로, 인터페이스 형태로 존재.
/// </summary>

namespace Pg::Graphics
{
	class IRenderPass
	{
	public:
		virtual void Initialize() abstract;

		//전 패스에서 필요한 요소들을 패스 나름대로 받는 코드이다.
		virtual void ReceiveRequiredElements(void* place1, void* place2, void* place3, void* place4) abstract;
		virtual void BindPass() abstract;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) abstract;
		virtual void UnbindPass() abstract;
		//다음 패스에 필요할 요소를 
		virtual void PassOnNextRequirements(void** place1, void** place2, void** place3, void** place4) abstract;
	};
}