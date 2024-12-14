#pragma once
#include "RendererBase3D.h"
#include <string>
#include "data_factory.h" // Auto Register를 위한 필수요건.
/// <summary>
/// 앞으로 3D Static Mesh 출력을 담당할 렌더러.
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class StaticMeshRenderer : public RendererBase3D, Pg::Factory::Data::RegisteredInFactory<RendererBase3D, StaticMeshRenderer, GameObject*>
	{
	public:
		StaticMeshRenderer(GameObject* obj);

		//자동화된 Auto-Registration 작동 위해 필수.
		static RendererBase3D* CreateInstance(GameObject* go) { return new StaticMeshRenderer(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::StaticMeshRenderer"; }

		//InitializePath가 호출될 때, 외부에서 Path 연동하는 과정이 있어야 한다.
		virtual void ConvertPotentialUnrealValues() override;
		//혹시 Path만 있고 Name은 없는 상황을 막기 위해. 
		virtual void CheckForPathNameErrors() override;

		virtual void OnSerialize(SerializeVector& sv) override;
		virtual void OnDeserialize(SerializeVector& sv) override;

		void SetAlphaPercentage(float per);
		float GetAlphaPercentage();

	public:
		//일단은 에디터에서 값을 받아서 만들어지지만,
		//해당 Mesh에서,
		//실제로 (에디터에서 접근권한이 없는) Mesh Path / Material Path를 못 찾았을 때 (상위 레이어에서 보관)
		//디폴트 경로/매터리얼로 바뀔 것.
		//Name -> 통해서 Path를 찾고 -> 다시 못 찾거나 등등 상황일시 역으로 Name을 바꿀 수 있음.

		//VISITABLE로 바꿀 목록.
		/// visit_struct 방식
		BEGIN_VISITABLES(StaticMeshRenderer);
		VISITABLE(std::string, _meshName);
		VISITABLE(std::string, _materialName);
		END_VISITABLES;

		//하나가 더 추가되어야 한다. Alpha 일 때만 쓰이는 애. //0-100 범위.
		float _alphaPercentage{ 50.f };
	};
}