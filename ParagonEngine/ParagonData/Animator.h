#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

#include "data_factory.h" // Auto Register를 위한 필수요건.
#include <memory>
#include <string>

/// <summary>
///	Animator Component. Update가 되면서, 내부적으로 BehaviorTree를 동작시킨다.
/// 자체적으로 Update를 돌지 않고, BehaviorTreeSystem 내부 Update() 함수에서 동작 수행.
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

	//														부모			자신		매개변수..
	class Animator : public Component, Pg::Factory::Data::RegisteredInFactory<Component, Animator, GameObject*>
	{
		friend class Pg::Engine::BTree::BehaviorTreeSystem;
	public:
		Animator(GameObject* owner);
		~Animator();

		//자동화된 Auto-Registration 작동 위해 필수.
		static Component* CreateInstance(GameObject* go) { return new Animator(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::Animator"; }

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

	public:
		//VISITABLES
		BEGIN_VISITABLES(Pg::Data::Animator);

		//나중에 직렬화될 부분.
		VISITABLE(std::string, _behaviorTreePath);
		
		//인스턴스된 별개의 Tree로서 동작할지, 
		//게임 엔진 내부에서 유일한 같은 내용의 Tree로 동작할지를 선택한다.
		//만약 Uniform한 Animator인 경우, 객체별로 존재하지 않고, Component가 Object 하위에 있다고 해서 동작하지 않는다.
		//AddUniformRunningAnimator()를 호출해서 (API) -> Update 루프에 포함시키고,
		//RemoveUniformRunningAnimator()를 호출해서 Update 루프에서 제거한다.
		//다르게 말해, _isUniform이 true가 되는 순간 BT::Tree의 ReadOnly만 되는 것이다. 
		VISITABLE(bool, _isUniform);

		//Renderer와 접속, false = AlwaysAnimate <-> true = CullCompletely.
		// Cull되어도 업데이트할 것인가, 아닌가를 보여준다. 
		VISITABLE(bool, _isCulled);

		//ENDVISITABLES
		END_VISITABLES;
	public:
		//일단 별도로 Behavior Tree Path의 세터를 만들어놓았다.
		void SetBehaviorTreePath(const std::string& path);

		//자기 자신이 Groot에 의해 Monitor되고 싶을 때.
		void MonitorSelfInGroot();
	private:
		//실제로 로직에 따라 작동할 BehaviorTree의 예시.
		//Animator는 객체에 따라 다른 로직을 보유해야 한다.
		//다른 XML파일에서 매번 파싱되어 로드되어야 한다.
		BT::Tree* _behavTree = nullptr;
		BT::FileLogger* _logger{ nullptr };
	};
}