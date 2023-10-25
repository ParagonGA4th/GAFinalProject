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

		//구체화된 Render 함수, 호출되면 실제 기능 수행해야!
		virtual void Render() override;

	private:
		
	};
}
