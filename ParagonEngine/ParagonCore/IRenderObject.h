#pragma once
#include "RenderTextData.h"

/// <summary>
/// 모든 BaseRenderer 기반 클래스들이 갖고 있고,
/// GameObject를 렌더하는데 필요한 정보의 제어권을 쥐고 있다.
/// Transform을 포함하지는 않는다. 
/// GOAL: 3DModel, Material 정보를 보관한다.
/// </summary>

namespace Pg::Core
{
	class IRenderObject abstract
	{
	public:
		//실제 RenderObject가 GameObject가 연동되었는지 렌더데이터의 경로 업데이트.
		//매번 호출되는 것은 아니다. (변화가 있을 때만)
		virtual void UpdateRenderData(Pg::Core::RenderTextData rTextData) abstract;


	};
}
