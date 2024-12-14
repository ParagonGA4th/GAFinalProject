#pragma once
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase2D.h"
#include "ISingleGUI.h"
#include "TransformSimpleStorage.h"

//GUI�� �ʱ� ���¸� ����ϱ� ����.
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
		// ��� ���������� ��� �ȴ�. �׳� �����ִ� �����ϼ��� �ֱ⿡.
		// ��ü ������ ����. -> �ܺ� �����Ϳ� ������ ���̱⿡.
	};
}
