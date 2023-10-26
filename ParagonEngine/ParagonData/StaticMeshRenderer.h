#pragma once
#include "RendererBase3D.h"
#include <string>

/// <summary>
/// 앞으로 3D Static Mesh 출력을 담당할 렌더러.
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class StaticMeshRenderer : public RendererBase3D
	{
	public:
		StaticMeshRenderer(GameObject* obj);

	private:
		
	};
}
