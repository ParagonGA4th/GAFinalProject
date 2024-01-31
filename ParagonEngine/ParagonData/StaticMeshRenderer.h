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

		virtual void InitializePathsFromName() override;
		virtual void OnSerialize(SerializeVector& sv) override;
		virtual void OnDeserialize(SerializeVector& sv) override;

	public:
		//일단은 에디터에서 값을 받아서 만들어지지만,
		//해당 Mesh에서,
		//실제로 (에디터에서 접근권한이 없는) Mesh Path / Material Path를 못 찾았을 때 (상위 레이어에서 보관)
		//디폴트 경로/매터리얼로 바뀔 것.
		//Name -> 통해서 Path를 찾고 -> 다시 못 찾거나 등등 상황일시 역으로 Name을 바꿀 수 있음.

		//VISITABLE로 바꿀 목록.
		/// visit_struct 방식
		BEGIN_VISITABLES(StaticMeshRenderer);
		VISITABLE(bool, _isInstanced);
		VISITABLE(std::string, _meshName);
		VISITABLE(std::string, _materialName);
		END_VISITABLES;
	};
}
