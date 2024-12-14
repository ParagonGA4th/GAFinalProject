#pragma once
#include "RendererBase3D.h"
#include <string>
#include "data_factory.h" // Auto Register�� ���� �ʼ����.
/// <summary>
/// ������ 3D Static Mesh ����� ����� ������.
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class StaticMeshRenderer : public RendererBase3D, Pg::Factory::Data::RegisteredInFactory<RendererBase3D, StaticMeshRenderer, GameObject*>
	{
	public:
		StaticMeshRenderer(GameObject* obj);

		//�ڵ�ȭ�� Auto-Registration �۵� ���� �ʼ�.
		static RendererBase3D* CreateInstance(GameObject* go) { return new StaticMeshRenderer(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::StaticMeshRenderer"; }

		//InitializePath�� ȣ��� ��, �ܺο��� Path �����ϴ� ������ �־�� �Ѵ�.
		virtual void ConvertPotentialUnrealValues() override;
		//Ȥ�� Path�� �ְ� Name�� ���� ��Ȳ�� ���� ����. 
		virtual void CheckForPathNameErrors() override;

		virtual void OnSerialize(SerializeVector& sv) override;
		virtual void OnDeserialize(SerializeVector& sv) override;

		void SetAlphaPercentage(float per);
		float GetAlphaPercentage();

	public:
		//�ϴ��� �����Ϳ��� ���� �޾Ƽ� �����������,
		//�ش� Mesh����,
		//������ (�����Ϳ��� ���ٱ����� ����) Mesh Path / Material Path�� �� ã���� �� (���� ���̾�� ����)
		//����Ʈ ���/���͸���� �ٲ� ��.
		//Name -> ���ؼ� Path�� ã�� -> �ٽ� �� ã�ų� ��� ��Ȳ�Ͻ� ������ Name�� �ٲ� �� ����.

		//VISITABLE�� �ٲ� ���.
		/// visit_struct ���
		BEGIN_VISITABLES(StaticMeshRenderer);
		VISITABLE(std::string, _meshName);
		VISITABLE(std::string, _materialName);
		END_VISITABLES;

		//�ϳ��� �� �߰��Ǿ�� �Ѵ�. Alpha �� ���� ���̴� ��. //0-100 ����.
		float _alphaPercentage{ 50.f };
	};
}