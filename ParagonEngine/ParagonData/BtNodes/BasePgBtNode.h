#pragma once
#include "../ParagonData/GameObject.h"
#include "BTreeShareData.h"
/// <summary>
/// 미리 Initialize 함수가 호출이 필요한 BehaviorTree Node들을 위해
/// 부모 클래스 마련.
/// </summary>
namespace Pg::Data::BTree::Node
{
	class BasePgBtNode
	{
	public:
		//미리 GameObject의 포인터를 소속된 Tree에 따라서 (Instanced) 넣을 수 있게 설정해야 한다.
		void InitializeTreeNode(Pg::Data::GameObject* obj, Pg::Data::BTree::BTreeShareData* sharedData)
		{
			_object = obj; 
			_sharedData = sharedData;
		}

		//구현한다면, InitializeTreeNode 이후 호출. 미리 멤버변수 등을 받아올 수 있다.
		//하부 클래스에 따라 달라진다.
		virtual void InitCustom() {}

	protected:
		Pg::Data::GameObject* GetGameObject() { return _object; }
		Pg::Data::BTree::BTreeShareData* GetSharedData() { return _sharedData; }

	private:
		Pg::Data::GameObject* _object = nullptr;
		Pg::Data::BTree::BTreeShareData* _sharedData = nullptr;
	};
}