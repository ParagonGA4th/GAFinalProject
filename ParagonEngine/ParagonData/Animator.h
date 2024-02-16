#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"

#include <behaviortree_cpp_v3/bt_factory.h>
#include <memory>
#include <string>

/// <summary>
///	Animator Component. Update가 되면서, 내부적으로 BehaviorTree를 동작시킨다.
/// </summary>

namespace Pg::Data
{
	using namespace Pg::Math;
	class GameObject;

	class Animator : public Component
	{
	public:
		Animator(GameObject* owner);
		~Animator();

	public:
		//VISITABLES
		
		//나중에 직렬화될 부분.
		std::string _behaviorTreePath;
		
		//Renderer와 접속, false = AlwaysAnimate <-> true = CullCompletely.
		// Cull되어도 업데이트할 것인가, 아닌가를 보여준다. 
		bool _isCulled = false;

		//ENDVISITABLES
	public:
		void SetBehaviorTree(BT::Tree* behaviorTree);

	private:
		//실제로 로직에 따라 작동할 BehaviorTree의 예시.
		//Animator는 객체에 따라 다른 로직을 보유해야 한다.
		//다른 XML파일에서 매번 파싱되어 로드되어야 한다.
		BT::Tree* _behavTree = nullptr;


		


	};
}