#include "BehaviorTreeSystem.h"

namespace Pg::Engine::BTree
{
	BehaviorTreeSystem::BehaviorTreeSystem()
	{
		_factory = std::make_unique<BT::BehaviorTreeFactory>();
	}

	BehaviorTreeSystem::~BehaviorTreeSystem()
	{

	}

	void BehaviorTreeSystem::Initialize()
	{
		InitAllLeafNodes();
	}

	void BehaviorTreeSystem::Update()
	{

	}

	void BehaviorTreeSystem::InitAllLeafNodes()
	{
		//XML Node를 매칭해서 하는 것. 
		
	}

}
