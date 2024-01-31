#pragma once

#include "RendererBase3D.h"
#include <string>

/// <summary>
/// 앞으로 3D Skinned Mesh 출력을 담당할 렌더러.
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class SkinnedMeshRenderer : public RendererBase3D
	{
	public:
		SkinnedMeshRenderer(GameObject* obj);

		virtual void InitializePathsFromName() override;


	};
}

