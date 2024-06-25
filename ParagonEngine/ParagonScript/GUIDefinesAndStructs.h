#pragma once
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase2D.h"
#include "ISingleGUI.h"
#include "TransformSimpleStorage.h"

//GUI의 초기 상태를 기록하기 위해.
namespace Pg::DataScript
{
	struct GUIAggregate
	{
		Pg::Data::GameObject* _guiObj{ nullptr };
		bool _isObjActive{ true };
		bool _isComponentActive{ true };
		TransformSimpleStorage _transStorage;
		Pg::Data::RendererBase2D* _rendererBase2D{ nullptr };
		ISingleGUI* _guiComponent{ nullptr }; 
		// 얘는 예외적으로 없어도 된다. 그냥 보여주는 목적일수도 있기에.
		// 자체 리셋은 없다. -> 외부 포인터에 의존할 것이기에.
	};
}
