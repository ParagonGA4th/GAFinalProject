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
		void InitAllLeafNodes();

	private:
		std::unique_ptr<BT::BehaviorTreeFactory> _factory;
	};
}


