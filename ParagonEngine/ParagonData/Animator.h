#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

#include "data_factory.h" // Auto Register�� ���� �ʼ����.
#include <memory>
#include <string>

/// <summary>
///	Animator Component. Update�� �Ǹ鼭, ���������� BehaviorTree�� ���۽�Ų��.
/// ��ü������ Update�� ���� �ʰ�, BehaviorTreeSystem ���� Update() �Լ����� ���� ����.
/// </summary>

namespace Pg::Engine
{
	namespace BTree
	{
		class BehaviorTreeSystem;
	}
}

namespace BT { class Tree; class FileLogger; }

namespace Pg::Data
{
	using namespace Pg::Math;
	class GameObject;

	//														�θ�			�ڽ�		�Ű�����..
	class Animator : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Animator, GameObject*>
	{
		friend class Pg::Engine::BTree::BehaviorTreeSystem;
	public:
		Animator(GameObject* owner);
		~Animator();

		//�ڵ�ȭ�� Auto-Registration �۵� ���� �ʼ�.
		static Component* CreateInstance(GameObject* go) { return new Animator(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Animator"; }

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

	public:
		//VISITABLES
		BEGIN_VISITABLES(Pg::Data::Animator);

		//���߿� ����ȭ�� �κ�.
		VISITABLE(std::string, _behaviorTreePath);
		
		//�ν��Ͻ��� ������ Tree�μ� ��������, 
		//���� ���� ���ο��� ������ ���� ������ Tree�� ���������� �����Ѵ�.
		//���� Uniform�� Animator�� ���, ��ü���� �������� �ʰ�, Component�� Object ������ �ִٰ� �ؼ� �������� �ʴ´�.
		//AddUniformRunningAnimator()�� ȣ���ؼ� (API) -> Update ������ ���Խ�Ű��,
		//RemoveUniformRunningAnimator()�� ȣ���ؼ� Update �������� �����Ѵ�.
		//�ٸ��� ����, _isUniform�� true�� �Ǵ� ���� BT::Tree�� ReadOnly�� �Ǵ� ���̴�. 
		VISITABLE(bool, _isUniform);

		//Renderer�� ����, false = AlwaysAnimate <-> true = CullCompletely.
		// Cull�Ǿ ������Ʈ�� ���ΰ�, �ƴѰ��� �����ش�. 
		VISITABLE(bool, _isCulled);

		//ENDVISITABLES
		END_VISITABLES;
	public:
		//�ϴ� ������ Behavior Tree Path�� ���͸� �������Ҵ�.
		void SetBehaviorTreePath(const std::string& path);

		//�ڱ� �ڽ��� Groot�� ���� Monitor�ǰ� ���� ��.
		void MonitorSelfInGroot();
	private:
		//������ ������ ���� �۵��� BehaviorTree�� ����.
		//Animator�� ��ü�� ���� �ٸ� ������ �����ؾ� �Ѵ�.
		//�ٸ� XML���Ͽ��� �Ź� �Ľ̵Ǿ� �ε�Ǿ�� �Ѵ�.
		BT::Tree* _behavTree = nullptr;
		BT::FileLogger* _logger{ nullptr };
	};
}