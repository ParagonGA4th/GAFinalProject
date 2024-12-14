#pragma once
#include "../ParagonData/GameObject.h"
#include "BTreeShareData.h"
/// <summary>
/// �̸� Initialize �Լ��� ȣ���� �ʿ��� BehaviorTree Node���� ����
/// �θ� Ŭ���� ����.
/// </summary>
namespace Pg::Data::BTree::Node
{
	class BasePgBtNode
	{
	public:
		//�̸� GameObject�� �����͸� �Ҽӵ� Tree�� ���� (Instanced) ���� �� �ְ� �����ؾ� �Ѵ�.
		void InitializeTreeNode(Pg::Data::GameObject* obj, Pg::Data::BTree::BTreeShareData* sharedData)
		{
			_object = obj; 
			_sharedData = sharedData;
		}

		//�����Ѵٸ�, InitializeTreeNode ���� ȣ��. �̸� ������� ���� �޾ƿ� �� �ִ�.
		//�Ϻ� Ŭ������ ���� �޶�����.
		virtual void InitCustom() {}

	protected:
		Pg::Data::GameObject* GetGameObject() { return _object; }
		Pg::Data::BTree::BTreeShareData* GetSharedData() { return _sharedData; }

	private:
		Pg::Data::GameObject* _object = nullptr;
		Pg::Data::BTree::BTreeShareData* _sharedData = nullptr;
	};
}