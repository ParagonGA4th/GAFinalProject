#pragma once
#include "RenderTextData.h"

/// <summary>
/// 모든 BaseRenderer 기반 클래스들이 갖고 있고,
/// GameObject를 렌더하는데 필요한 정보의 제어권을 쥐고 있다.
/// Transform을 포함하지는 않는다. 
/// GOAL: 3DModel, Material 정보를 보관한다.
/// </summary>

namespace Pg::Data
{
	class IRenderObject abstract
	{
	public:

		//실제로 렌더오브젝트들을 렌더하는 함수.
		virtual void Render() abstract;

	};
}
