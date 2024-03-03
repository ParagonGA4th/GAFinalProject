#pragma once
#include "../ParagonData/GameObject.h"
#include "BTreeShareData.h"
/// <summary>
/// 미리 Initialize 함수가 호출이 필요한 BehaviorTree Node들을 위해
/// 부모 클래스 마련.
/// </summary>
namespace Pg::Engine::BTree::Node
{
	class PgBtNode
	{
	public:
		//미리 GameObject의 포인터를 소속된 Tree에 따라서 (Instanced) 넣을 수 있게 설정해야 한다.
		void InitializeTreeNode(Pg::Data::GameObject* obj, Pg::Engine::BTree::BTreeShareData* sharedData)
		{
			_object = obj; 
			_sharedData = sharedData;
		}

	protected:
		Pg::Data::GameObject* GetObject() { return _object; }
		Pg::Engine::BTree::BTreeShareData* GetSharedData() { return _sharedData; }

	private:
		Pg::Data::GameObject* _object = nullptr;
		Pg::Engine::BTree::BTreeShareData* _sharedData = nullptr;
	};
}