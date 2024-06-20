#pragma once
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase2D.h"
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
	};
}
