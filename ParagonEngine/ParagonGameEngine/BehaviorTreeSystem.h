#pragma once

#include <vector>
#include <memory>
#include <behaviortree_cpp_v3/bt_factory.h>

namespace Pg::Engine::BTree
{
	class BehaviorTreeSystem
	{
	public:
		BehaviorTreeSystem();
		~BehaviorTreeSystem();

		void Initialize();
		void Update();

	private:
		//모든 Leaf Node들은 내부적으로 이 함수에서 Register되어야 한다.
		void InitAllLeafNodes();

	private:
		std::unique_ptr<BT::BehaviorTreeFactory> _factory;

		//일단은 SoundSystem처럼 자체적인 노드를 관리.
		
		
	};
}


