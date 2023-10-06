#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
#include <memory>

/// <summary>
/// 앞으로 렌더할 때 모든 기본 렌더러들이 상속받을 기본 클래스.
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class Transform;
	class IRenderObject;
}

namespace Pg::Data
{
	class BaseRenderer : public Component
	{
	public:
		BaseRenderer(GameObject* obj);
		virtual ~BaseRenderer();

		IRenderObject* GetRenderObject();

	protected:
		IRenderObject* _renderObject = nullptr;
		bool _is3D = NULL;
	};
}



