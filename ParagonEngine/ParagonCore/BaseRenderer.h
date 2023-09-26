#pragma once
#include "Component.h"
#include "../ParagonAPI/PgMath.h"
#include <memory>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

/// <summary>
/// 앞으로 렌더할 때 모든 기본 렌더러들이 상속받을 기본 클래스.
/// </summary>

namespace Pg::Core
{
	class GameObject;
	class Transform;
	class IRenderObject;
}

namespace Pg::Core
{
	class BaseRenderer : public Component
	{
	public:
		BaseRenderer(GameObject* obj);
		virtual ~BaseRenderer();
		


	protected:
		std::unique_ptr<IRenderObject> _renderInfo;

	};
}



