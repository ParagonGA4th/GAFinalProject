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
		virtual void BindPass() abstract;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) abstract;
		virtual void UnbindPass() abstract;
		virtual void SetupNextRequirements() abstract;
	};
}